#include <omp.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int n;
vector<int> queries;

unordered_map<int, float> id_to_value;
unordered_map<int, vector<int>> neighbors;
unordered_map<int, float> id_answer;

void read_input() {
  string line;

  getline(cin, line);
  stringstream headerStream(line);

  headerStream >> n;
  int k;
  while (headerStream >> k) {
    queries.push_back(k);
  }

  while (getline(cin, line)) {
    stringstream ss(line);

    int id;
    float value;
    ss >> id >> value;

    id_to_value[id] = value;
    
    vector<int> &id_neighbor_list = neighbors[id];

    int neighbor;
    while (ss >> neighbor) {
      id_neighbor_list.push_back(neighbor);
    }
  }
}

float dfs_answer(int query) {
  if (id_answer.count(query)) {
    return id_answer[query];
  }

  float max_value = -1;
  int max_id = 0;

  for (auto v : neighbors[query]) {
    if (id_to_value[v] > max_value) {
      max_value = id_to_value[v];
      max_id = v;
    }
  }

  if (max_id == 0) {
    return id_answer[query] = id_to_value[query];
  }

  return id_answer[query] = id_to_value[query] + dfs_answer(max_id);
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
  std::chrono::duration<double> reading_elapsed = end - start;

  start = std::chrono::high_resolution_clock::now();
  process_queries();
  end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> processing_elapsed = end - start;

  cout << "Reading elapsed time: " << reading_elapsed.count() << " seconds\n";
  cout << "Processing elapsed time: " << processing_elapsed.count() << " seconds\n";
  cout << "Total elapsed time: " << (reading_elapsed + processing_elapsed).count() << " seconds\n";
 
  return 0;
}