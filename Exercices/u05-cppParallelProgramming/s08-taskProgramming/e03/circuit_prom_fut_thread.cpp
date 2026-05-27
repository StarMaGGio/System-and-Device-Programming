#include <iostream>
#include <future>

using namespace std;

void and_f(bool a, bool b, promise<bool> res) {
    res.set_value(a && b);
}

void or_f(bool a, bool b, promise<bool> res) {
    res.set_value(a || b);
}

int main(int argc, char const *argv[]) {
    
    bool i[8] = {true, false, true, true, true, true, false, false};
    promise<bool> pr[7];
    future<bool> o[7];
    for (int j=0; j<7; ++j) {
        o[j] = pr[j].get_future();
    }

    thread t[7];
    t[0] = thread(and_f, i[0], i[1], move(pr[0]));
    t[1] = thread(and_f, i[2], i[3], move(pr[1]));
    t[2] = thread(and_f, i[4], i[5], move(pr[2]));
    t[3] = thread(and_f, i[6], i[7], move(pr[3]));
    t[4] = thread(or_f, o[0].get(), o[1].get(), move(pr[4]));
    t[5] = thread(or_f, o[2].get(), o[3].get(), move(pr[5]));
    t[6] = thread(and_f, o[4].get(), o[5].get(), move(pr[6]));
    cout << "result is " << o[6].get() << endl;
    for (auto &j : t) {
        j.join();
    }
    return 0;
}
