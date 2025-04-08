#include <omp.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int n;
vector<int> queries;

unordered_map<int, float> id_to_value;
unordered_map<int, pair<float, int>> max_neighbor;
unordered_map<int, vector<int>> indegree_list;
unordered_map<int, float> id_answer;

// Add this:
vector<string> raw_lines;
mutex id_to_value_mutex, max_neighbor_mutex, indegree_mutex;

// Updated read_input:
void read_input() {
  string line;
  getline(cin, line);

  stringstream headerStream(line);
  headerStream >> n;

  int k;
  while (headerStream >> k) {
    queries.push_back(k);
  }

  // Step 1: read all lines (sequential)
  while (getline(cin, line)) {
    raw_lines.push_back(line);
  }

  // Step 2: reserve space to reduce reallocations
  id_to_value.reserve(n);
  max_neighbor.reserve(n);
  indegree_list.reserve(n);
  id_answer.reserve(n);

// Step 3: parallel parse/process
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < (int)raw_lines.size(); i++) {
    stringstream ss(raw_lines[i]);

    int id;
    float value;
    ss >> id >> value;

    vector<int> local_indegree;
    vector<pair<int, float>> local_max_updates;

    // Parse neighbors
    int neighbor;
    while (ss >> neighbor) {
      local_indegree.push_back(neighbor);

      // Read-only access to id_to_value, can't be done here — we'll do it later
    }

// Step 4: critical section to update shared maps
#pragma omp critical
    {
      id_to_value[id] = value;

      for (int neighbor : local_indegree) {
        indegree_list[neighbor].push_back(id);

        // Update max_neighbor[id] based on known neighbor's value
        if (id_to_value.count(neighbor) &&
            max_neighbor[id].first < id_to_value[neighbor]) {
          max_neighbor[id] = {id_to_value[neighbor], neighbor};
        }
      }

      // Update reverse links
      for (int source : indegree_list[id]) {
        if (max_neighbor[source].first < value) {
          max_neighbor[source] = {value, id};
        }
      }
    }
  }
}

float dfs_answer(int query) {
  if (id_answer.count(query)) {
    return id_answer[query];
  }

  if (!max_neighbor.count(query)) {
    return id_answer[query] = id_to_value[query];
  }

  return id_answer[query] =
             id_to_value[query] + dfs_answer(max_neighbor[query].second);
}

void process_queries() {
  for (int i = 0; i < (int)queries.size(); i++) {
    cout << queries[i] << ": " << dfs_answer(queries[i]) << "\n";
  }
}

int main() {
  cout << setprecision(6) << fixed;

  auto start = std::chrono::high_resolution_clock::now();
  read_input();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  
  process_queries();

  cout << "Elapsed time: " << elapsed.count() << " seconds\n";
  return 0;
}