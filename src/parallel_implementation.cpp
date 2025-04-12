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

  // Read the header (n and queries)
  getline(cin, line);
  stringstream headerStream(line);
  headerStream >> n;
  int k;
  while (headerStream >> k) {
    queries.push_back(k);
  }

  // Buffer all lines first
  vector<string> lines;
  while (getline(cin, line)) {
    lines.push_back(line);
  }

  // Resize maps ahead of time for better performance
  id_to_value.reserve(lines.size());
  neighbors.reserve(lines.size());

  // Parallel parsing using OpenMP
  #pragma omp parallel for schedule(static)
  for (int i = 0; i < (int)lines.size(); i++) {
    stringstream ss(lines[i]);

    int id;
    float value;
    ss >> id >> value;

    vector<int> id_neighbor_list;
    int neighbor;
    while (ss >> neighbor) {
      id_neighbor_list.push_back(neighbor);
    }

    // Safe direct write since each id is unique
    // but unordered_map is not thread safe to multipe writes
    #pragma omp critical
    {
      id_to_value[id] = value;
      neighbors[id] = std::move(id_neighbor_list);
    }
  }
}

float dfs_answer(int query) {
  bool cached = false;
  float cached_result;

  // Check if result is already computed — inside a critical section
  #pragma omp critical(id_answer_access)
  {
    auto it = id_answer.find(query);
    if (it != id_answer.end()) {
      cached = true;
      cached_result = it->second;
    }
  }

  if (cached) return cached_result;

  // Find max-valued neighbor
  float max_val = -1.0f;
  int max_id = 0;
  for (int v : neighbors[query]) {
    if (id_to_value[v] > max_val) {
      max_val = id_to_value[v];
      max_id = v;
    }
  }

  float result = id_to_value[query];
  if (max_id != 0) {
    result += dfs_answer(max_id);  // recursive call, safe
  }

  // Store the result — inside another critical section
  #pragma omp critical(id_answer_access)
  {
    id_answer[query] = result;
  }

  return result;
}



void process_queries() {
  vector<string> output(queries.size());

  #pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < (int)queries.size(); i++) {
    int q = queries[i];
    float ans = dfs_answer(q);

    stringstream ss;
    ss << fixed << setprecision(6) << q << ": " << ans << "\n";
    output[i] = ss.str();
  }

  for (const auto& line : output) {
    cout << line;
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

 /*  cout << "Reading elapsed time: " << reading_elapsed.count() << " seconds\n";
  cout << "Processing elapsed time: " << processing_elapsed.count() << " seconds\n";
  cout << "Total elapsed time: " << (reading_elapsed + processing_elapsed).count() << " seconds\n"; */

  return 0;
}