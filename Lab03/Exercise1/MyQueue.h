#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <string>
#include <list>

using namespace std;

template <typename T>
class MyQueue {
    list<T> data;

public:
    void enqueue(T val) { data.push_back(val); }

    T dequeue() {
        if(!data.empty()) {
            T res = data.front();
            data.pop_front();
            return res;
        }

        return T();
    }

    void visit() {
        for (const auto& val : data) {
            cout << val << " ";
        }
        cout << endl;
    }

    int getSize() { return data.size(); }

    bool empty() { return data.size() == 0; }

};

#endif