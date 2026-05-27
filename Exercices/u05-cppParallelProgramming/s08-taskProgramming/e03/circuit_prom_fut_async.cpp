#include <iostream>
#include <future>
#include <vector>

using namespace std;

/**
 * This function performs a logical AND operation on two boolean inputs and prints the result.
 */
bool f_and (bool in1, bool in2) {
    bool out = in1 & in2;
    cout << in1 << " AND " << in2 << " = " << out << endl;
    return out;
}

/**
 * This function performs a logical OR operation on two boolean inputs obtained from futures and prints the result.
 */
bool f_or (future<bool> s1f, future<bool> s2f) {
    bool in1 = s1f.get();
    bool in2 = s2f.get();
    bool out = in1 | in2;
    cout << in1 << " OR " << in2 << " = " << out << endl;
    return out;
}

int main(int argc, char const *argv[]) {
    bool in1 = false, in2 = true, in3 = true, in4 = true, in5 = false, in6 = false, in7 = false, in8 = false;

    future<bool> s1f = async (f_and, in1, in2);
    future<bool> s2f = async (f_and, in3, in4);
    future<bool> s3f = async (f_and, in5, in6);
    future<bool> s4f = async (f_and, in7, in8);

    future<bool> s5f = async (f_or, move(s1f), move(s2f));
    future<bool> s6f = async (f_or, move(s3f), move(s4f));

    future<bool> s7f = async (f_and, s5f.get(), s6f.get());

    bool out = s7f.get();
    cout << "OUT = " << out << endl;

    return 0;
}


