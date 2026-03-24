#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <list>

using namespace std;

struct MyQueue {
    list<string> data;

    void enqueue(string s) {
        data.push_back(s);
    }

    string dequeue() {
        if(!data.empty()) {
            string res = data.front();
            data.pop_front();
            return res;
        }

        return NULL;
    }

    void visit() {
        for (const string& s : data) {
            cout << s << " ";
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

    MyQueue myQueue;
    myQueue.enqueue("ciao");
    myQueue.enqueue("hello");

    myQueue.visit();
    cout << myQueue.getSize() << endl;
    cout << myQueue.empty() << endl;

    return 0;
}