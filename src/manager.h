//
// Created by lulaz on 3/13/23.
//

#ifndef PROJ_DA_1_MANAGER_H
#define PROJ_DA_1_MANAGER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

#include "utils/csv_reader.h"
#include "graph.h"

class Manager {
public:
    Manager (const char *network_file, const char *stations_file);

    void initialize_all();

    void initialize_selected(std::vector<std::string> lines); // TODO implement

    int max_simultaneous_trains_between(std::string station1, std::string station2);

    std::vector<std::pair<std::string, std::string>> most_trains_for_network_cap();

    std::vector<std::pair<std::string, int>> top_k_municipalities_capacity(int k);

    std::vector<std::pair<std::string, int>> top_k_districts_capacity(int k);

    std::vector<std::pair<std::string, int>> top_k_districts_flow(int k);

    std::vector<std::pair<std::string, int>> top_k_municipalities_flow(int k);

    int max_trains_arriving_at(std::string station);

    void initialize_subgraph(std::vector<std::pair<std::string, std::string>> segments);

    int sub_graph_max_simultaneous_trains_between(std::string station1, std::string station2);

    std::vector<std::pair<std::string, int>> top_k_stations_affected(int k);

    std::pair<int, int> min_cost_max_flow(std::string station1, std::string station2);

    void printGraph();

    void test();

private:
    CsvReader network_reader;
    CsvReader stations_reader;

    // graph structure
    Graph train_graph;
    
    // subgraph structure for use without select segments
    Graph sub_graph;

    // hash map of strings to vertex numbers
    std::unordered_map<std::string, int> vertex_map;
};

#endif //PROJ_DA_1_MANAGER_H
