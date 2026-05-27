#include <iostream>
#include <thread>
#include <vector>
#include <future>

using namespace std;

/**
 * Methods protorypes
 */
void take(promise<int>&);
void check(future<int>&, promise<bool>&);
void give (future<bool>&);

/**
 * Main function
 */
int main(int argc, char const *argv[]) {
    promise<int> p_in;
    future<int> f_in = p_in.get_future();

    promise<bool> p_res;
    future<bool> f_res = p_res.get_future();

    thread t1(take, ref(p_in));
    thread t2(check, ref(f_in), ref(p_res));
    thread t3(give, ref(f_res));

    t1.join(); t2.join(); t3.join();

    return 0;
}

/**
 * Function for the reading thread
 */
void take (promise<int> &p_in) {
    int in;
    cout << "Insert a number" << endl;
    cin >> in;
    p_in.set_value(in); // Set the value of the promise in
}

/**
 * Function for the writing thread
 */
void give (future<bool>& f_res) {
    bool answer = f_res.get(); // Get the value of the future res
    string s0 (" ");
    if (!answer) 
        s0 = " NOT";
    cout << "Number is" << s0 << "prime";
}

/**
 * Function for the checking thread
 */
void check (future<int> &f_in, promise<bool>& p_res) {
    int n = f_in.get(); // Get the value of the future in
    bool prime = true;
    if (n <= 1) {
        prime = false;
    }
    for (int j = 2; j < n; j++) {
        if (n % j == 0) {
            prime = false;
            break;
        }
    }
    p_res.set_value(prime); // Set the value of the promise res
}