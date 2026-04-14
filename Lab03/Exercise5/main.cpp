#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <set>

using namespace std;

struct friendship_rel {
    string person1;
    string person2;
};

int main(int argc, char ** argv) {
    string filename("data");
    
    // Adjacency list to store the social network
    map<string, set<string>> social_network;

    // Open data file
    ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: " << filename << endl;
        return 0;
    }

    // Read data file and fill social_network map
    string line;
    while (getline(infile, line)) {
        stringstream sl(line);
        friendship_rel rel;

        sl >> rel.person1 >> rel.person2;

        social_network[rel.person1].insert(rel.person2);
        social_network[rel.person2].insert(rel.person1);
    }

    infile.close();

    // Display list of all enrolled individuals in lexicographical order
    cout << "List of all enrolled individuals:" << endl << "[ ";
    for (const auto& person : social_network) {
        cout << person.first << " ";
    }
    cout << "]" << endl << endl;

    // Given individual name -> display all friends in lexicographical order
    string individual = "Riboni_Mattia";
    cout << "List of friends of " << individual << ": " << endl << "[ ";
    for (const auto& friend_name : social_network[individual]) {
        cout << friend_name << " ";
    }
    cout << "]" << endl;

    // Given two individuals, X and Y, display whether they are related in the social network

    return 0;
};
