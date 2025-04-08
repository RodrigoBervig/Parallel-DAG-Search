#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <unordered_set>

using namespace std;

const int MAX_NEIGHBORS = 50;

// Generate a random float between min and max
float rand_float(float min = 1.0, float max = 1000.0) {
    return ((float) rand() / RAND_MAX) * (max - min) + min;
}

// Sample `count` unique integers between low (inclusive) and high (exclusive)
vector<int> sample_unique(int low, int high, int count) {
    unordered_set<int> seen;
    vector<int> result;
    count = min(count, high - low);
    while ((int)result.size() < count) {
        int x = rand() % (high - low) + low;
        if (seen.insert(x).second) {
            result.push_back(x);
        }
    }
    return result;
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
    random_shuffle(ids.begin(), ids.end());

    // Pick compute list IDs randomly (about n/2 nodes)
    int compute_count = n / 2;
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
        float val = rand_float();
        cout << from << " " << fixed << setprecision(6) << val;

        int remaining = n - i - 1;
        if (remaining <= 0) {
            cout << " 0\n";
            continue;
        }

        int num_neighbors = rand() % (min(MAX_NEIGHBORS, remaining) + 1);
        vector<int> neighbors = sample_unique(i + 1, n, num_neighbors);

        for (int to_idx : neighbors)
            cout << " " << ids[to_idx];
        cout << "\n";
    }

    return 0;
}
