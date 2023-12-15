//
// Created by lulaz on 3/13/23.
//

#ifndef PROJ_DA_1_GRAPH_H
#define PROJ_DA_1_GRAPH_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <climits>

struct Edge{
    int vertex1;
    int vertex2;
    int capacity;
    std::string service;
};

class Graph {
    public:
        Graph(bool dir);

        int getNumVertices() const;

        int getNumEdges() const;

        bool isDirected() const;

        void setVertexInfo(int vertex, std::string name, std::string district, std::string municipality, std::string township, std::string line);

        bool addVertex(int vertex, std::string name, std::string district, std::string municipality, std::string township, std::string line);

        // add edge from v1 to v2, and from v2 to v1 if directed
        void addEdge(int v1, int v2, int capacity, std::string service);

        void removeEdge(int v1, int v2);

        void removeAdjEdges(int v);

        // void add_residual_edge(int v1, int v2);

        std::vector<std::pair<std::string, std::string>> most_trains_for_network_cap();

        std::vector<std::pair<std::string, int>> top_k_municipalities_capacity();

        std::vector<std::pair<std::string, int>> top_k_districts_capacity();

        std::vector<std::pair<std::string, int>> top_k_districts_flow();

        std::vector<std::pair<std::string, int>> top_k_municipalities_flow();

        int max_trains_arriving_at(int vertex);

        bool bfs(int source, int sink, std::vector<int> &parent);

        int ford_fulkerson(int source, int sink);

        // return a subgraph without the segments between the pairs of stations
        Graph sub_graph_without_segments(std::vector<std::pair<int, int>> segments);

        std::map<std::string, int> max_flow_per_station();

        void dijkstra(int vertex1, int vertex2, std::vector<int> &dist, std::vector<int> &parent);

        std::pair<int, int> min_cost_max_flow(int vertex1, int vertex2);

        void printGraph();

        void test();

    protected:
        int num_vertices;
        int num_edges;
        bool directed;

        struct edgeNode{
            int vertex;
            int capacity;
            int flow;
            std::string service;
        };

        struct vertexNode{
            int vertex;

            std::string name;
            std::string district;
            std::string municipality;
            std::string township;
            std::string line;

            std::list<edgeNode> adj;
        };

        std::vector<vertexNode> vertices; 
};

#endif //PROJ_DA_1_GRAPH_H
