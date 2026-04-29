#include <iostream>
#include <thread>
#include <vector>
#include <future>

using namespace std;

void take(promise<int>&);
void check(future<int>&, promise<bool>&);
void give (future<bool>&);

int main() {

}

// Method for the reading thread
void take(promise<int> &p_in) {
    int in;
    cout << "Insert a number" << endl;
    cin >> in;
    p_in.set_value(in); // Set promise "in"
}