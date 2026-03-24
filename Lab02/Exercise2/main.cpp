#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

struct Record {
    int id;
    long long money;
    char surname[31];
    char name[31];
    int age;
};

int main(int argc, char ** argv) {
    char option;
    int n;

    // Open file_2.bin
    string filename2("file_2.bin");
    fstream file(filename2, ios::binary | ios::in | ios::out | ios::ate);
    streampos pos;


    while (true) {
        // Display the Menu
        cout << "------ Menu ------" << endl;
        cout << "- R n: Where R is the character 'R', and n is an integer value." << endl;
        cout << "\t\tRead from the file the record representing student number n and" << endl;
        cout << "\t\tdisplay this information on standard output." << endl;
        cout << "- W n: Where W is the character 'W', and n is an integer value." << endl;
        cout << "\t\tRead from standard input all data fields for the student number" << endl;
        cout << "\t\tn and write them in the file in position n." << endl;
        cout << "- E: End the program." << endl;

        // Get user input
        cout << "Select an option from the menu: ";
        cin >> option;

        if (option == 'E') break;

        // Execute selected option
        switch (option) {
            case 'W':
                cin >> n;
                Record r_out;
                cout << "Enter id, money, surname, name, age" << endl;
                cin >> r_out.id >> r_out.money >> r_out.surname >> r_out.name >> r_out.age;
                pos = (n-1) * sizeof(Record);
                file.seekp(pos); // Set the position to write in the file
                file.write(reinterpret_cast<char*>(&r_out), sizeof(Record));
                break;
            case 'R':
                cin >> n;
                Record r_in;
                pos = (n-1) * sizeof(Record);
                file.seekg(pos); // Set the position to read in the file
                file.read(reinterpret_cast<char*>(&r_in), sizeof(Record));
                cout << r_in.id << " " << r_in.money << " " << r_in.surname << " " << r_in.name << " " << r_in.age << endl;
                break;
            default:
                cout << "Command: " << option << " is not in the Menu!" << endl;
                break;
        }
    }

    file.close();
    cout << "Program ended!" << endl;

    return 0;
}