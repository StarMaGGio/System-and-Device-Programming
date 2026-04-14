#include <functional>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

class C {
private:
    int i;
public:
    C () { // CONSTRUCTOR
        cout << "[C]";
    }
    ~C() { // DESTRUCTOR
        cout << "[D]";
    }
    C (const C &n) { // COPY CONSTRUCTOR
        cout << "[CC]";
    }
    C &operator=(const C &n) { // COPY ASSIGNMENT OPERATOR
        cout << "[CAO]";
        return *this;
    }
    C (C&& n) noexcept { // MOVE CONSTRUCTOR
        cout << "[MC]";
    }
    C &operator=(C&&n) noexcept { // MOVE ASSIGNMENT OPERATOR
        cout << "[MAO]";
        return *this;
    }
    void set(int n)  {
        i = n;
    };
    int get () {
        return i;
    }
};

void swap (C &e1, C &e2) {
  C tmp;                                                    // CONSTRUCTOR
  tmp=e1;                                                   // COPY ASSIGNMENT OPERATOR
  e1=e2;                                                    // COPY ASSIGNMENT OPERATOR
  e2=tmp;                                                   // COPY ASSIGNMENT OPERATOR
  return;                                                   // DESTRUCTOR
}

int main() {
  cout << endl << "{01}"; C e1;                             // CONSTRUCTOR
  cout << endl << "{02}"; C e2[5];                          // CONSTRUCTOR 5 times
  cout << endl << "{03}"; C e3 = *new (std::nothrow) C;     // CONSTRUCTOR + COPY CONSTRUCTOR
  cout << endl << "{04}"; C *e4 = new C;                    // CONSTRUCTOR
  cout << endl << "{05}"; C *e5 = new C[10];                // CONSTRUCTOR 10 times
  
  cout << endl << "{06}"; C v1 = e1;                        // COPY CONSTRUCTOR
  cout << endl << "{07}"; C v2 = (std::move(e1));           // MOVE CONSTRUCTOR
  cout << endl << "{08}"; C v3;                             // CONSTRUCTOR
  cout << endl << "{09}"; v3 = (std::move(e1));             // MOVE ASSIGNMENT OPERATOR

  cout << endl << "{10}"; swap (e1, v3);                    

  cout << endl << "{11}"; return 0;                         // DESTRUCTOR 9 times
}

/* OUTPUT:
    {01}[C]
    {02}[C][C][C][C][C]
    {03}[C][CC]
    {04}[C]
    {05}[C][C][C][C][C][C][C][C][C][C]
    {06}[CC]
    {07}[MC]
    {08}[C]
    {09}[MAO]
    {10}[C][CAO][CAO][CAO][D]
    {11}[D]*9
*/