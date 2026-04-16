#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <algorithm>

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
    string individualA = "Maggioni_Mattia";
    string individualB = "Mori_Dario";

    // Breadth-First-Search algorithm initialization
    queue<string> q;
    map<string, string> parent; // To track visited nodes and the path
    bool found = false;

    q.push(individualA);
    parent[individualA] = ""; // Mark start node with empty parent

    // Perform BFS algorithm
    while (!q.empty()) {
        string current = q.front();
        q.pop();

        if (current == individualB) {
            found = true;
            break; // Exit the while because individualB is reached
        }

        for (const auto& friend_name : social_network[current]) {
            if (parent.find(friend_name) == parent.end()) { // If the friend_name is not in the already visited list
                parent[friend_name] = current; // Record the path (current -> friend_name)
                q.push(friend_name);
            }
        }
    }

    // Reconstruct and print the path
    if (found) {
        vector<string> path;
        for (string step = individualB; step != ""; step = parent[step]) {
            path.push_back(step);
        }

        for (auto it = path.rbegin(); it != path.rend(); ++it) {
            cout << *it << (it + 1 == path.rend() ? "\n" : " -> ");
        }
    } else {
        cout << individualA << " and " << individualB << " are NOT related." << endl;
    }

    return 0;
};
