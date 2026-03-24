#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

struct MyStack {
    vector<int> data;

    void push(int n) {
        data.push_back(n);
    }

    int pop() {
        if(!data.empty()) {
            int res = data.back();
            data.pop_back();
            return res;
        }

        return NULL;
    }

    void visit() {
        for (int i = 0; i < data.size(); i++) {
            cout << data[i] << " ";
        }

        cout << endl;
    }

    int getSize() {
        return data.size();
    }

    bool empty() {
        return data.size() == 0;
    }
};

int main(int argc, char ** argv) {
    MyStack myStack;

    cout << myStack.getSize() << endl;
    cout << myStack.empty() << endl;

    return 0;
}