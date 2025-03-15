#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

#define N 200 // Number of nodes
#define MAX_ID 200 // Limit for ID and values
#define MAX_NEIGHBORS 50 // Max number of neighbors

// Function to generate a unique random ID
int generate_unique_id(set<int>& used_ids) {
    int id;
    do {
        id = rand() % MAX_ID + 1;
    } while (used_ids.find(id) != used_ids.end());
    
    used_ids.insert(id);
    return id;
}

int main() {
    srand(time(0));

    set<int> used_ids; // To store unique IDs
    vector<int> ids;   // Store generated IDs for easy access

    // Generate unique IDs
    for (int i = 0; i < N; i++) {
        ids.push_back(generate_unique_id(used_ids));
    }

    // Generate M (queries)
    int M = rand() % 20 + 1; // Random M between 1 and 20

    // Print first line: N and M queries
    cout << N << " " << ids[rand() % N] << endl; // Querying a random node

    // Generate and print graph
    for (int i = 0; i < N; i++) {
        int value = rand() % MAX_ID + 1; // Random value between 1 and 20
        cout << ids[i] << " " << value;

        // Generate random neighbors
        set<int> neighbors;
        int num_neighbors = rand() % (MAX_NEIGHBORS + 1);
        for (int j = 0; j < num_neighbors; j++) {
            int neighbor_id;
            do {
                neighbor_id = ids[rand() % N]; // Pick a random node ID
            } while (neighbor_id == ids[i] || neighbors.find(neighbor_id) != neighbors.end());

            neighbors.insert(neighbor_id);
        }

        // Print neighbors
        for (int neighbor : neighbors) {
            cout << " " << neighbor;
        }
        cout << endl;
    }

    return 0;
}
