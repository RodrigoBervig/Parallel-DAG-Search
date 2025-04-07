#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

const int MAX_NEIGHBORS = 50;

// Generate a random float between min and max
float rand_float(float min = 1.0, float max = 100.0) {
    return ((float) rand() / RAND_MAX) * (max - min) + min;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <number_of_nodes>\n";
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        cerr << "Number of nodes must be > 0\n";
        return 1;
    }

    srand(time(nullptr));

    // Generate unique node IDs from 1 to n and shuffle for randomness
    vector<int> ids(n);
    for (int i = 0; i < n; ++i)
        ids[i] = i + 1;

    // Pick compute list IDs randomly (about n/2 nodes)
    int compute_count = rand() % (n / 2 + 1) + 1;
    vector<int> compute_list = ids;
    random_shuffle(compute_list.begin(), compute_list.end());

    // Output: header line
    cout << n << " ";
    for (int i = 0; i < compute_count; ++i)
        cout << compute_list[i] << " ";
    cout << "\n";

    // Output: each node's data
    for (int i = 0; i < n; ++i) {
        int from = ids[i];
        int val = (int)rand_float();
        cout << from << " " << fixed << setprecision(2) << val;

        // Each node can only point to nodes that come *after* it to ensure DAG
        int max_targets = n - i - 1;
        int num_neighbors = max_targets > 0 ? rand() % min(MAX_NEIGHBORS, max_targets + 1) : 0;

        vector<int> neighbors;
        for (int j = i + 1; j < n; ++j)
            neighbors.push_back(ids[j]);

        random_shuffle(neighbors.begin(), neighbors.end());

        if (!num_neighbors) {
            cout << " 0"; // No neighbors
        }

        for (int j = 0; j < num_neighbors; ++j)
            cout << " " << neighbors[j];
        cout << "\n";
    }

    return 0;
}
