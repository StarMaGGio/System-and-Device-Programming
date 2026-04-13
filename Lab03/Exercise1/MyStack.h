#ifndef MYSTACK_H
#define MYSTACK_H

#include <string>
#include <vector>

using namespace std;

template <typename T>
class MyStack {
    vector<T> data;

public:
    void push(T val) { data.push_back(val); }

    T pop() {
        if(!data.empty()) {
            T res = data.back();
            data.pop_back();
            return res;
        }

        return T();
    }

    void visit() {
        for (int i = data.size() - 1; i >= 0 ; i--) {
            cout << data[i] << " ";
        }

        cout << endl;
    }

    int getSize() { return data.size(); }

    bool empty() {  return data.size() == 0; }
};

#endif