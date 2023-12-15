//
// Created by lulaz on 3/13/23.
//

#include "graph.h"

/** Construtor da classe Graph.
 * Este método tem complexidade de tempo O(1).
 * @param dir true se o grafo for direcionado, false caso contrário.
*/
Graph::Graph(bool dir) : num_vertices(0), num_edges(0), directed(dir) {
    vertices = std::vector<vertexNode>();
}      

// getters
/** Retorna o número de vértices do grafo.
 * Este método tem complexidade de tempo O(1).
 * @return número de vértices do grafo.
*/
int Graph::getNumVertices() const {
    return num_vertices;
}

/** Retorna o número de arestas do grafo.
 * Este método tem complexidade de tempo O(1).
 * @return número de arestas do grafo.
*/
int Graph::getNumEdges() const {
    return num_edges;
}

/** Retorna se o grafo é direcionado ou não.
 * Este método tem complexidade de tempo O(1).
 * @return true se o grafo for direcionado, false caso contrário.
*/
bool Graph::isDirected() const {
    return directed;
}

/** Devolve os possiveis caminhos onde podem circular o maior número de comboios em simultâneo.
 * Este método tem complexidade de tempo O(V^2/2 * f * E), onde V é o número de vértices, f é o número de fluxo máximo
 * e E é o número de arestas.
 * @return vector de pares de strings com os nomes das estações de partida e chegada.
 */
std::vector<std::pair<std::string, std::string>> Graph::most_trains_for_network_cap() {
    // Run the ford fulkerson algorithm for all possible pairs of stations
    // and return the pairs with the maximum flow
    std::vector<std::pair<std::string, std::string>> most_trains;
    int max_trains = 0;
    for(int i = 0; i < num_vertices; i++) {
        // std::cout << "i: " << i << vertices[i].name << std::endl;
        for(int j = i+1; j < num_vertices; j++) {
            // std::cout << "j: " << j << std::endl;
            if(i != j) {
                int trains = ford_fulkerson(i, j);
                if(trains > max_trains) {
                    max_trains = trains;
                    most_trains.clear();
                    most_trains.push_back(std::make_pair(vertices[i].name, vertices[j].name));
                } else if(trains == max_trains) {
                    most_trains.push_back(std::make_pair(vertices[i].name, vertices[j].name));
                }
            }
        }
    }
    // std::cout << "maximum simultaneous trains: " << max_trains << std::endl;
    return most_trains;
}

// todo change this to use flow instead of capacity

/** Devolve os k municípios com maior necessidade de capacidade de transporte de comboios, em termos de capacidade.
 * Este método tem complexidade de tempo O(V * E + V * log(V)), onde V é o número de vértices e E é o número de arestas.
 * @param k: número de municípios a devolver.
 * @return vector de pares de strings com os nomes dos municípios e a sua capacidade.
 */
std::vector<std::pair<std::string, int>> Graph::top_k_municipalities_capacity() {
    // get the total capacity for each municipality
    std::unordered_map<std::string, int> municipality_capacities;
    for(int i = 0; i < num_vertices; i++) {
        if(municipality_capacities.find(vertices[i].municipality) == municipality_capacities.end()) {
            municipality_capacities[vertices[i].municipality] = 0;
        }
        for(auto edge : vertices[i].adj) {
            municipality_capacities[vertices[i].municipality] += edge.capacity;
        }
    }

    std::vector<std::pair<std::string, int>> sorted_municipalities;
    for(auto entry : municipality_capacities) {
        sorted_municipalities.push_back(entry);
    }
    std::sort(sorted_municipalities.begin(), sorted_municipalities.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
                  return a.second > b.second;
              });

    return sorted_municipalities;
}

// todo change this to use flow instead of capacity

/** Devolve os k distritos com maior necessidade de capacidade de transporte de comboios, em termos de capacidade.
 * Este método tem complexidade de tempo O(V * E + V * log(V)), onde V é o número de vértices e E é o número de arestas.
 * @param k: número de distritos a devolver.
 * @return vector de pares de strings com os nomes dos distritos e a sua capacidade.
 */
std::vector<std::pair<std::string, int>> Graph::top_k_districts_capacity() {
    // get the total capacity for each district
    std::unordered_map<std::string, int> district_capacities;
    for(int i = 0; i < num_vertices; i++) {
        if(district_capacities.find(vertices[i].district) == district_capacities.end()) {
            district_capacities[vertices[i].district] = 0;
        }
        for(auto edge : vertices[i].adj) {
            district_capacities[vertices[i].district] += edge.capacity;
        }
    }

    std::vector<std::pair<std::string, int>> sorted_districts;
    for(auto entry : district_capacities) {
        sorted_districts.push_back(entry);
    }
    std::sort(sorted_districts.begin(), sorted_districts.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
                  return a.second > b.second;
              });
    return sorted_districts;
}

/// Struct para ser usada como hash para pares de strings.
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};

/** Devolve os k distritos com maior necessidade de capacidade de transporte de comboios em termos de fluxo.
 * Este método tem complexidade de tempo O(V^2 * E + V * log(V)), onde V é o número de vértices e E é o número de arestas.
 * @param k: número de distritos a devolver.
 * @return vector de pares de strings com os nomes dos distritos e o seu fluxo.
 */
std::vector<std::pair<std::string, int>> Graph::top_k_districts_flow() {
   
    // get the max flow between all pairs of districts
    std::unordered_map<std::pair<std::string, std::string>, int, pair_hash> district_flows;
    for(int i = 0; i < num_vertices; i++) {
        for(int j = i+1; j < num_vertices; j++) {
            int flow = ford_fulkerson(i, j);
            district_flows[{vertices[i].district, vertices[j].district}] = flow;
            district_flows[{vertices[j].district, vertices[i].district}] = flow;
        }
    }

    // get the total flow for each district
    std::unordered_map<std::string, int> district_flows_sum;
    for(auto entry : district_flows) {
        std::string district1 = entry.first.first;
        std::string district2 = entry.first.second;
        int flow = entry.second;
        district_flows_sum[district1] += flow;
        district_flows_sum[district2] += flow;
    }

    // sort districts based on their flow values and select the top-k districts
    std::vector<std::pair<std::string, int>> sorted_districts;
    for(auto entry : district_flows_sum) {
        sorted_districts.push_back(entry);
    }
    std::sort(sorted_districts.begin(), sorted_districts.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
                  return a.second > b.second;
              });

    return sorted_districts;
}

/** Devolve os k municípios com maior necessidade de capacidade de transporte de comboios em termos de fluxo.
 * Este método tem complexidade de tempo O(V^2 * E + V * log(V)), onde V é o número de vértices e E é o número de arestas.
 * @param k: número de municípios a devolver.
 * @return vector de pares de strings com os nomes dos municípios e o seu fluxo.
 */
std::vector<std::pair<std::string, int>> Graph::top_k_municipalities_flow() {
    // get the max flow between all pairs of municipalities
    std::unordered_map<std::pair<std::string, std::string>, int, pair_hash> municipality_flows;
    for(int i = 0; i < num_vertices; i++) {
        for(int j = i+1; j < num_vertices; j++) {
            int flow = ford_fulkerson(i, j);
            municipality_flows[{vertices[i].municipality, vertices[j].municipality}] = flow;
            municipality_flows[{vertices[j].municipality, vertices[i].municipality}] = flow;
        }
    }

    // get the total flow for each municipality
    std::unordered_map<std::string, int> municipality_flows_sum;
    for(auto entry : municipality_flows) {
        std::string municipality1 = entry.first.first;
        std::string municipality2 = entry.first.second;
        int flow = entry.second;
        municipality_flows_sum[municipality1] += flow;
        municipality_flows_sum[municipality2] += flow;
    }

    // sort municipalities based on their flow values and select the top-k municipalities
    std::vector<std::pair<std::string, int>> sorted_municipalities;
    for(auto entry : municipality_flows_sum) {
        sorted_municipalities.push_back(entry);
    }
    std::sort(sorted_municipalities.begin(), sorted_municipalities.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
                  return a.second > b.second;
              });

    return sorted_municipalities;
}

/** Devolve o número máximo de comboios que podem chegar ao mesmo tempo a uma estação.
 * Este método tem complexidade de tempo O(V * E), onde V é o número de vértices e E é o número de arestas.
 * @param vertex: vértice da estação.
 * @return número máximo de comboios que podem chegar ao mesmo tempo a uma estação.
 */
int Graph::max_trains_arriving_at(int vertex) {
    //We can then run the Ford-Fulkerson algorithm to find the maximum 
    //flow between the source (a virtual node representing all incoming trains) 
    //and the destination (the station we are interested in). 
    //The maximum flow will represent the maximum number of trains that 
    //can simultaneously arrive at the given station.

    //with capacity
    int sum = 0;

    // loop through all vertices in the graph
    for (const auto& v : vertices[vertex].adj) {
        sum += v.capacity;
    }

    return sum;


}

/** Cria um novo grafo com os vértices e arestas do grafo original, mas sem as arestas que ligam os vértices dos segmentos.
 * Este método tem complexidade de tempo O(V + E), onde V é o número de vértices e E é o número de arestas.
 * @param segments: vector de pares de inteiros com os vértices que ligam os segmentos.
 * @return novo grafo com os vértices e arestas do grafo original, mas sem as arestas que ligam os vértices dos segmentos.
 */
Graph Graph::sub_graph_without_segments(std::vector<std::pair<int, int>> segments) {
    Graph sub_graph(false);
    // add all vertices to the subgraph
    // exclude edges that go between the
    // vertices in the segments vector

    sub_graph.vertices = vertices;
    sub_graph.num_edges = num_edges;
    sub_graph.num_vertices = num_vertices;

    // go remove edges that go between the vertices in the segments vector
    for (auto segment : segments) {
        int v1 = segment.first;
        int v2 = segment.second;
        sub_graph.vertices[v1].adj.remove_if([v2](edgeNode& edge) {
            return edge.vertex == v2;
        });
        sub_graph.vertices[v2].adj.remove_if([v1](edgeNode& edge) {
            return edge.vertex == v1;
        });
        num_edges -= 2;
    }

    return sub_graph;
}

// approximation of the flow between every two stations
/** Devolve um mapa com o fluxo máximo entre cada estação.
 * Este método tem complexidade de tempo O(V^2 * E), onde V é o número de vértices e E é o número de arestas.
 * @return mapa com o fluxo máximo entre cada estação.
 */
std::map<std::string, int> Graph::max_flow_per_station() {
    std::map<std::string, int> flow_per_station;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = i+1; j < num_vertices; j++) {
            if(i == j) continue; 
            int flow = ford_fulkerson(i, j);
            flow_per_station[vertices[i].name] += flow;
            flow_per_station[vertices[j].name] += flow;
        }
    }

    return flow_per_station;
}

/** Algoritmo de Dijkstra modificado para encontrar o caminho menos dispendioso entre dois vértices.
 * Este método tem em conta os serviços de comboios, optando por um caminho mais barato, mesmo que seja mais longo.
 * Este método tem complexidade de tempo O(V^2), onde V é o número de vértices.
 * @param vertex1: vértice de partida.
 * @param vertex2: vértice de chegada.
 * @param dist: vetor com as distâncias entre o vértice de partida e todos os outros vértices.
 * @param parent: vetor com os pais de cada vértice.
 */
void Graph::dijkstra(int vertex1, int vertex2, std::vector<int> &dist, std::vector<int> &parent){
    // find the cheapest path between the two stations
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    pq.push({0, vertex1});
    dist[vertex1] = 0;
    std::vector<bool> visited(num_vertices, false);

    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();

        if(visited[u]) continue;
        visited[u] = true;

        for(auto edge : vertices[u].adj){
            int v = edge.vertex;
            int weight = 2;

            // // remove \r from the end of the string
            // edge.service.erase(std::remove(edge.service.begin(), edge.service.end(), '\r'), edge.service.end());
            
            if(edge.service.compare("STANDARD") == 0) {
                // std::cout << "STANDARD" << std::endl;
                weight = 2;
            } else if (edge.service.compare("ALFA PENDULAR") == 0) {
                // std::cout << "ALFA PENDULAR" << std::endl;
                weight = 4;
            }
            if(dist[v] > dist[u] + weight){
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

/** Devolve o fluxo máximo entre dois vértices pertencentes a um caminho menos dispendioso.
 * Este método tem complexidade de tempo O(V^2 * E), onde V é o número de vértices e E é o número de arestas.
 * @param vertex1: vértice de partida.
 * @param vertex2: vértice de chegada.
 * @return fluxo máximo entre dois vértices.
 */
std::pair<int, int> Graph::min_cost_max_flow(int vertex1, int vertex2){
    // first pass to find the cheapest path between the two stations
    std::vector<int> dist(num_vertices, INT_MAX);
    std::vector<int> parent(num_vertices, -1);
    dijkstra(vertex1, vertex2, dist, parent);

    // second pass to find the maximum flow between the two stations
    // using the cheapest path found in the first pass
    // go through the path and find the minimum capacity
    int min_capacity = INT_MAX;

    int v = vertex2;
    while(v != vertex1){
        int u = parent[v];
        for(auto edge : vertices[u].adj){
            if(edge.vertex == v){
                std::cout << edge.service << std::endl;
                min_capacity = std::min(min_capacity, edge.capacity);
                break;
            }
        }
        v = u;
    }

    return {min_capacity, dist[vertex2]*min_capacity};
}

/** Adiciona um vertice ao grafo.
 * Este método tem complexidade de tempo O(1).
 * @param vertex
 * @param name
 * @param district
 * @param municipality
 * @param township
 * @param line
 */
bool Graph::addVertex(int vertex, std::string name, std::string district, std::string municipality, std::string township, std::string line) {
    // check if name already exists
    for(auto v : vertices){
        if(v.name.compare(name) == 0){
            std::cout << "Vertex already exists" << std::endl;
            std::cout << name << std::endl;
            return false;
        }
    }
    vertices.push_back({
        vertex,
        name,
        district,
        municipality,
        township,
        line,
        std::list<edgeNode>()
    });
    num_vertices++;
    return true;
}

/** Muda as informações de um vertice no grafo.
 * Este método tem complexidade de tempo O(1).
 * @param vertex
 * @param name
 * @param district
 * @param municipality
 * @param township
 * @param line
 */
void Graph::setVertexInfo(int vertex, std::string name, std::string district, std::string municipality, std::string township, std::string line) {
    vertices[vertex].name = name;
    vertices[vertex].district = district;
    vertices[vertex].municipality = municipality;
    vertices[vertex].township = township;
    vertices[vertex].line = line;

    vertices[vertex].vertex = vertex;
}

/** Adiciona uma aresta ao grafo.
 * Este método tem complexidade de tempo O(E), onde E é o número de arestas.
 * @param v1
 * @param v2
 * @param capacity
 * @param service
 */
void Graph::addEdge(int v1, int v2, int capacity, std::string service) {
    if(v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices) {
        std::cout << "Invalid vertex" << std::endl;
        return;
    }
    // check if edge already exists
    for(auto e : vertices[v1].adj) {
        if(e.vertex == v2) {
            std::cout << "Edge already exists" << std::endl;
            return;
        }
    }

    vertices[v1].adj.push_back(edgeNode{v2, capacity, 0, service});

    if(!directed) {
        vertices[v2].adj.push_back(edgeNode{v1, capacity, 0, service});
        num_edges++;
    }

    num_edges++;
}

/** Remove uma aresta do grafo.
 * Este método tem complexidade de tempo O(d_v1 + d_v2), onde d_v1 e d_v2 são os graus dos vértices v1 e v2.
 * @param v1
 * @param v2
 */
void Graph::removeEdge(int v1, int v2) {
    if (v1 < 0 || v1 >= num_vertices || v2 < 0 || v2 >= num_vertices) {
        std::cout << "Invalid vertex" << std::endl;
        return;
    }

    // check if edge exists
    bool edge_exists = false;
    for (auto e: vertices[v1].adj) {
        if (e.vertex == v2) {
            edge_exists = true;
            break;
        }
    }
    if (!edge_exists) {
        std::cout << "Edge does not exist" << std::endl;
        return;
    }

    // remove edge
    vertices[v1].adj.remove_if([v2](edgeNode e) { return e.vertex == v2; });
    if (!directed) {
        vertices[v2].adj.remove_if([v1](edgeNode e) { return e.vertex == v1; });
        num_edges--;
    }

    num_edges--;
}

/** Remove todas as arestas adjacentes a um vértice.
 * Este método tem complexidade de tempo O(d_v), onde d_v é o grau do vértice v.
 * @param v
 */
void Graph::removeAdjEdges(int v) {
    // Remove all adjacent edges going from and to v
    for (auto e: vertices[v].adj) {
        vertices[e.vertex].adj.remove_if([v](edgeNode e) { return e.vertex == v; });
        num_edges--;
    }
    vertices[v].adj.clear();
}

/** Imprime o grafo.
 * Este método tem complexidade de tempo O(V + E), onde V é o número de vértices e E é o número de arestas.
 */
void Graph::printGraph()
{
    for(auto v : vertices) {
        std::cout << v.name << ":" << v.vertex << std::endl;
        for(auto e : v.adj) {
            std::cout << vertices[e.vertex].name << " " << e.capacity << std::endl;
        }
    }
}

void Graph::test() {
    int nedges = 0;
    for(int i = -0; i < num_vertices; i++) {
        if(vertices[i].vertex != i) {
            std::cout << "vertex: " << vertices[i].vertex << " i: " << i << std::endl;
        }
        for(auto e : vertices[i].adj) {
            if(e.vertex < 0 || e.vertex >= num_vertices) {
                std::cout << "vertex: " << e.vertex << std::endl;
            }
            nedges++;
        }
    }
    if(nedges != num_edges) {
        std::cout << "nedges: " << nedges << " num_edges: " << num_edges << std::endl;
    }
    // std::cout << nedges << std::endl;
}
