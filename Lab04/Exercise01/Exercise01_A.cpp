#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <algorithm>
#include <time.h>

using namespace std;

/**
 * Data structure to pass to the thread
 */
struct FileData {
    string filename;
    int num_values;
    int *values_array;
};

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
}

int main(int argc, char ** argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input1> [<input2> ...] <output>\n", argv[0]);
        return 1;
    }

    clock_t start_time = clock();

    int n = argc - 2;
    string output_filename = argv[argc - 1];

    vector<FileData> results(n);
    vector<thread> threads;

    for (int i = 0; i < n; i++) {
        results[i].filename = argv[i+1];
        threads.emplace_back(orderFile, ref(results[i]));
    }
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();               
        }
    }

    int total_num_values = 0;
    for (auto& r : results) {
        total_num_values += r.num_values;
    }
    cout << total_num_values << " ";

    int *final_ordered_array = new int[total_num_values];
    int current_size = 0;
    for (auto& r : results) {
        for (int j = 0; j < r.num_values; j++) {
            final_ordered_array[current_size + j] = r.values_array[j];
        }
        inplace_merge(final_ordered_array, final_ordered_array + current_size, final_ordered_array + current_size + r.num_values);
        current_size += r.num_values;
    }

    clock_t end_time = clock();
    double elapsed_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "\nExecution CPU time: " << elapsed_time << " seconds\n";

    // for (int j = 0; j < total_num_values; j++) {
    //     cout << final_ordered_array[j] << " ";
    // }

    ofstream out_file(output_filename, ios::binary);
    if (out_file.is_open()) {
        out_file.write(reinterpret_cast<char*>(&total_num_values), sizeof(int));
        out_file.write(reinterpret_cast<char*>(final_ordered_array), total_num_values*sizeof(int));
        out_file.close();
    }
    
    for (auto& r : results) {
        delete[] r.values_array;
    }
    delete[] final_ordered_array;

    return 0;
}
