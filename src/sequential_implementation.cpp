#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <unordered_map>

using namespace std;

int n;
vector<int> queries;

unordered_map<int, float> id_to_value;
unordered_map<int, pair<float, int>> max_neighbor; 
unordered_map<int, vector<int>> indegree_list;
unordered_map<int, float> id_answer;

void read_input() {
    string line;
    
    getline(cin, line);
    stringstream headerStream(line);

    headerStream >> n;
    
    int k;
    while(headerStream >> k) {
        queries.push_back(k);
    }

    while(getline(cin, line)) {
        stringstream ss(line);

        int id; float value;
        ss >> id >> value;

        id_to_value[id] = value;
                
        for (auto v : indegree_list[id]) {
            if (max_neighbor[v].first < value) {
                max_neighbor[v] = {value, id};
            }
        }

        int neighbor;
        while(ss >> neighbor) {
            if (max_neighbor[id].first < id_to_value[neighbor]) {
                max_neighbor[id] = {id_to_value[neighbor], neighbor};
            }
            indegree_list[neighbor].push_back(id);
        }
    }

}

float dfs_answer(int query) {
    if (id_answer.count(query)) {
        return id_answer[query];
    }

    if(!max_neighbor.count(query)) {
        return id_to_value[query];
    }

    return id_answer[query] = id_to_value[query] + dfs_answer(max_neighbor[query].second);
}

void process_queries() {
    for(int i = 0; i < (int)queries.size(); i++) {
        cout << queries[i] << ": " << dfs_answer(queries[i]) << "\n";
    }
}

int main() {
    read_input();
    process_queries();

    return 0;
}