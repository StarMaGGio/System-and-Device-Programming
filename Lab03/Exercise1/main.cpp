#include <iostream>
#include <string>
#include "MyStack.h"
#include "MyQueue.h"

using namespace std;

int main(int argc, char ** argv) {

    MyStack<int> integerStack;
    integerStack.push(1);
    integerStack.push(2);
    integerStack.push(3);
    integerStack.pop();
    cout << "integerStack: ";
    integerStack.visit();

    MyQueue<string> stringQueue;
    stringQueue.enqueue("One");
    stringQueue.enqueue("Two");
    stringQueue.enqueue("Three");
    stringQueue.dequeue();
    cout << "stringQueue: ";
    stringQueue.visit();

    return 0;
}