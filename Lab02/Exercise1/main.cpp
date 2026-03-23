#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

struct record {
    int id;
    long long money;
    char surname[31];
    char name[31];
    int age;
};

int main(int argc, char ** argv) {
    string filename1("file_1.txt");
    string filename2("file_2.bin");
    string filename3("file_3.txt");

    ifstream infile1(filename1);
    if (!infile1.is_open()) {
        cerr << "Error:" << filename1 << endl;
        return 0;
    }

    ofstream outfile2(filename2, ios::binary);
    if (!outfile2.is_open()) {
        cerr << "Error:" << filename2 << endl;
        return 0;
    }

    // PROCESS file_1
    string s;
    while(getline(infile1, s)) {
        stringstream ss(s);

        record r;
        memset(r.name, 0, 31);
        memset(r.surname, 0, 31);


        ss >> r.id >> r.money >> r.surname >> r.name >> r.age;

        outfile2.write(reinterpret_cast<char*>(&r), sizeof(r));
    }

    infile1.close();
    outfile2.close();

    ifstream infile2(filename2, ios::binary);
    ofstream outfile3(filename3);
    record r2;
    while(infile2.read(reinterpret_cast<char*>(&r2), sizeof(r2))) {
        outfile3 << r2.id << " " << r2.money << " "  << r2.surname << " " << r2.name << " " << r2.age << "\n";
    }

    infile2.close();
    outfile3.close();
    
    return 0;
}