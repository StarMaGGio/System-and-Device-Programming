#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

/**
 * Data structure to pass to the thread
 */
struct FileData {
    string filename;
    int num_values;
    int *values_array;
};

mutex mtx;                          // mutex for synchronizing access to the finished_queue
condition_variable cv;              // condition variable for signaling when a thread has finished processing
queue<FileData*> finished_queue;    // queue to store the results from the threads

/**
 * Thread ordering function
 */
void orderFile(FileData& data) {
    ifstream file(data.filename);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << data.filename << endl;
        return;
    }
    file >> data.num_values;
    data.values_array = new int[data.num_values]; 
    int value, i = 0;
    while (file >> value) {
        data.values_array[i] = value;
        i++;
    }
    file.close();

    sort(data.values_array, data.values_array + data.num_values);

    {
        lock_guard<mutex> lock(mtx);    // lock_guard will automatically release the mutex when it goes out of scope
        finished_queue.push(&data);     // push the result to the queue
    }
    cv.notify_one();                    // notify the main thread that a thread has finished processing
}

int main(int argc, char ** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input1> [<input2> ...] <output>\n", argv[0]);
        return 1;
    }

    clock_t start_time = clock();

    int n = argc - 2;
    string output_filename = argv[argc - 1];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    vector<FileData> results(n);
    vector<thread> threads;

    for (int i = 0; i < n; i++) {
        results[i].filename = argv[i+1];
        threads.emplace_back(orderFile, ref(results[i]));
    }

    vector<int> final_ordered_array;
    int completed_threads = 0;

    while (completed_threads < n) {
        FileData* data = nullptr;
        {
            unique_lock<mutex> lock(mtx);                           // unique_lock is used here because we need to wait on the condition variable
            cv.wait(lock, [] { return !finished_queue.empty(); });  // wait until there is a finished thread in the queue, mutex will be released while waiting and re-acquired when the thread is notified
            data = finished_queue.front();
            finished_queue.pop();
        }

        int current_size = final_ordered_array.size();
        final_ordered_array.resize(current_size + data->num_values); // resize the final array to accommodate the new values

        for (int j = 0; j < data->num_values; j++) {
            final_ordered_array[current_size + j] = data->values_array[j];
        }

        inplace_merge(final_ordered_array.begin(), final_ordered_array.begin() + current_size, final_ordered_array.end());
        completed_threads++;
    }
    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();               
        }
    }

    clock_t end_time = clock();
    double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "\nExecution CPU time: " << elapsed_time << " seconds\n";

    int total_num_values = final_ordered_array.size();
    // cout << total_num_values << " ";
    // for (int j = 0; j < total_num_values; j++) {
    //     cout << final_ordered_array[j] << " ";
    // }

    ofstream out_file(output_filename, ios::binary);
    if (out_file.is_open()) {
        out_file.write(reinterpret_cast<char*>(&total_num_values), sizeof(int));
        out_file.write(reinterpret_cast<char*>(final_ordered_array.data()), total_num_values*sizeof(int));
        out_file.close();
    }
    
    for (auto& r : results) {
        delete[] r.values_array;
    }

    return 0;
}
