#include <iostream>
#include <future>

using namespace std;

int Fibonacci(int);

int main(int argc, char *argv[]) {
    int num = atoi(argv[1]);
    int result = Fibonacci(num);
    cout << "The Fibonacci number " << num << " is " << result << endl;

    return 0;
}

int Fibonacci(int num) {
    if (num < 2)
        return 1;
    else {
        auto future1 = async(launch::deferred, Fibonacci, num-1);
        auto future2 = async(launch::deferred, Fibonacci, num-2);

        return future1.get() + future2.get();
    }
}