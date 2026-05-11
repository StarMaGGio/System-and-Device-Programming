#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>

using namespace std;

/*
* Structure to hold file data and results
*/
struct FileData {
    string filename;
    int count = 0;
    long long sum = 0;
};

/*
* Function to launch in each thread
*/
void processFile(FileData& data) { // param& -> pass the parameter by-reference. Do not create a copy, work directly on the original variable
    ifstream file(data.filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << data.filename << endl;
        return;
    }
    int value;
    while (file >> value) { // read integers from the file
        data.sum += value;
        data.count++;
    }
    file.close();

    printf("File: %s | Count: %d | Sum: %lld\n", data.filename.c_str(), data.count, data.sum);
}

/*
* Main
*/
int main(int argc, char const *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file1> <file2> ... <fileN>" << endl;
        return 1;
    }

    int n = argc-1;
    vector<FileData> results(n);    // create a vector of FileData structures to hold the results for each file
    vector<thread> threads;         // create a vector of threads

    for (int i = 0; i < n; i++) {
        results[i].filename = argv[i+1];
        threads.emplace_back(processFile, ref(results[i])); // Launch a thread passing the fileData structure by reference
    }
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();               // Wait for the thread to finish
        }
    }
    
    // Aggregate results
    long long totalCount = 0, totalSum = 0;
    for (const auto& res : results) {
        totalCount += res.count;
        totalSum += res.sum;
    }

    // Print total results
    cout << "Total Count: " << totalCount << endl;
    cout << "Total Sum: " << totalSum << endl;

    return 0;
}
