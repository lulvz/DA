//
// Created by lulaz on 3/13/23.
//

#include "manager.h"

/** Construtor da classe Manager.
 * @param network_file path to network.csv.
 * @param stations_file path to stations.csv.
 */
Manager::Manager(const char *network_file, const char *stations_file) : 
    network_reader(network_file), 
    stations_reader(stations_file), 
    train_graph(false),
    sub_graph(false) {}

/** Lê os ficheiros CSV e inicializa o grafo com todos os vértices e arestas.
 * Esta função tem complexidade de tempo O(n + m) onde n é o número de linhas do ficheiro stations.csv e
 * m é o número de linhas do ficheiro network.csv.
 */
void Manager::initialize_all() {
    std::vector<std::string> line;
    int i = 0;
    while(!stations_reader.is_eof()) {
        line = stations_reader.read_line();
        if(line.size() == 5) {
//            std::cout<< line[0] << " " << line[1] << " " << line[2] << " " << line[3] << " " << line[4] << std::endl;
            if(train_graph.addVertex(i, line[0], line[1], line[2], line[3], line[4]))
                vertex_map[line[0]] = i;
        }
        i++;
    }
    while(!network_reader.is_eof()) {
        line = network_reader.read_line();
        if(line.size() == 4) {
            // remove \r from line[3]
            line[3].erase(std::remove(line[3].begin(), line[3].end(), '\r'), line[3].end());
            train_graph.addEdge(vertex_map[line[0]], vertex_map[line[1]], stoi(line[2]), line[3]);
        }
    }

    sub_graph = train_graph;
}

/** Devolve o número máximo de comboios que podem circular entre duas estações dadas como input.
 * @param station1 nome da estação de partida.
 * @param station2 nome da estação de chegada.
 * @return número máximo de comboios que podem circular entre as duas estações.
 */
int Manager::max_simultaneous_trains_between(std::string station1, std::string station2) {
    return train_graph.ford_fulkerson(vertex_map[station1], vertex_map[station2]);
}

/** Devolve os possiveis caminhos onde podem circular o maior número de comboios em simultâneo.
 * @return vetor de pares (nome da estação de partida, nome da estação de chegada).
 */
std::vector<std::pair<std::string, std::string>> Manager::most_trains_for_network_cap() {
    return train_graph.most_trains_for_network_cap();
}

/** Devolve os top k municípios com maiores necessidades de transporte ferroviário em termos de capacidade.
 * @param k
 * @return vetor de pares (nome do município, número de comboios necessários).
 */
std::vector<std::pair<std::string, int>> Manager::top_k_municipalities_capacity(int k) {
    std::vector<std::pair<std::string, int>> top_k = train_graph.top_k_municipalities_capacity();
    // return only top k
    return std::vector<std::pair<std::string, int>>(top_k.begin(), top_k.begin() + k);
}

/** Devolve os top k distritos com maiores necessidades de transporte ferroviário em termos de capacidade.
 * @param k
 * @return vetor de pares (nome do distrito, número de comboios necessários).
 */
std::vector<std::pair<std::string, int>> Manager::top_k_districts_capacity(int k) {
    std::vector<std::pair<std::string, int>> top_k = train_graph.top_k_districts_capacity();
    // return only top k
    return std::vector<std::pair<std::string, int>>(top_k.begin(), top_k.begin() + k);
}


/** Devolve os top k distritos com maiores necessidades de transporte ferroviário em termos de fluxo.
 * @param k
 * @return vetor de pares (nome do distrito, número de comboios necessários).
 */
std::vector<std::pair<std::string, int>> Manager::top_k_districts_flow(int k) {
    std::vector<std::pair<std::string, int>> top_k = train_graph.top_k_districts_flow();
    // return only top k
    return std::vector<std::pair<std::string, int>>(top_k.begin(), top_k.begin() + k);
}

/** Devolve os top k municípios com maiores necessidades de transporte ferroviário em termos de fluxo.
 * @param k
 * @return vetor de pares (nome do município, número de comboios necessários).
 */
std::vector<std::pair<std::string, int>> Manager::top_k_municipalities_flow(int k) {
    std::vector<std::pair<std::string, int>> top_k = train_graph.top_k_municipalities_flow();
    // return only top k
    return std::vector<std::pair<std::string, int>>(top_k.begin(), top_k.begin() + k);
}

/** Devolve o número máximo de comboios que podem simultaneamente chegar a uma dada estação.
 * @param station nome da estação.
 * @return número máximo de comboios que podem simultaneamente chegar à estação dada como input.
 */
int Manager::max_trains_arriving_at(std::string station){
    return train_graph.max_trains_arriving_at(vertex_map[station]);
}

/** Inicializa um subgrafo do grafo original, em que as arestas representam os segmentos de comboio dados como input.
 * @param segments vetor de pares (nome da estação de partida, nome da estação de chegada).
 */
void Manager::initialize_subgraph(std::vector<std::pair<std::string, std::string>> segments) {
    std::vector<std::pair<int, int>> edges;
    for(auto e: segments) {
        edges.push_back(std::make_pair(vertex_map[e.first], vertex_map[e.second]));
    }

    sub_graph = train_graph.sub_graph_without_segments(edges);
}

/** Devolve o número máximo de comboios que podem circular entre duas estações dadas como input em simultâneo, no subgrafo.
 * @param station1 nome da estação de partida.
 * @param station2 nome da estação de chegada.
 * @return número máximo de comboios que podem circular entre as duas estações em simultâneo, no subgrafo.
 */
int Manager::sub_graph_max_simultaneous_trains_between(std::string station1, std::string station2) {
    return sub_graph.ford_fulkerson(vertex_map[station1], vertex_map[station2]);
}

// maps name of the station to the amount of flow that is reduced
/** Devolve os top k estações que sofreram maior redução de fluxo no subgrafo, em comparação com a rede original.
 * @param k
 * @return vetor de pares (nome da estação, redução de fluxo).
 */
std::vector<std::pair<std::string, int>> Manager::top_k_stations_affected(int k) {
    // calculate max flow for each station pair in graph and subgraph and check
    // the ones that got reduced the most
    std::vector<std::pair<std::string, int>> top_k;
    std::map<std::string, int> flow_per_station = train_graph.max_flow_per_station();
    std::map<std::string, int> sub_flow_per_station = sub_graph.max_flow_per_station();

    for(auto e: flow_per_station) {
        top_k.push_back(std::make_pair(e.first, e.second - sub_flow_per_station[e.first]));
    }

    std::sort(top_k.begin(), top_k.end(), [](std::pair<std::string, int> a, std::pair<std::string, int> b) {
        return a.second > b.second;
    });

    return std::vector<std::pair<std::string, int>>(top_k.begin(), top_k.begin() + k);
}

/** Devolve o número máximo de comboios que podem circular entre duas estações dadas como input, tendo em consideração
 * o custo mínimo para a empresa.
 * @param station1 Estação de partida.
 * @param station2 Estação de chegada.
 * @return Retorna um par em que o primeiro elemento é o número máximo de comboios que podem circular entre as duas
 * estações, e o segundo elemento é o custo para a empresa.
 */
std::pair<int, int> Manager::min_cost_max_flow(std::string station1, std::string station2) {
    // get the max flow between the two stations first
    int max_flow = train_graph.ford_fulkerson(vertex_map[station1], vertex_map[station2]);

    // now get the min cost max flow
    return train_graph.min_cost_max_flow(vertex_map[station1], vertex_map[station2]);
}


/** Imprime o grafo
 */
void Manager::printGraph() {
    train_graph.printGraph();
}

void Manager::test() {
    train_graph.test();

//     cout << "Max simultaneous trains between Casa Branca and Évora: " << train_graph.ford_fulkerson(vertex_map["Casa Branca"], vertex_map["Évora"]) << endl;
//     cout << "Pairs that require the most trains: " << endl;
//     for(auto e: train_graph.most_trains_for_network_cap()) {
//         cout << e.first << " " << e.second << endl;
//     }

    int res = max_trains_arriving_at("Viana do Castelo");
    cout << "Max number of trains arriving at given station: " << res << "." << endl;
}