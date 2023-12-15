#include "graph.h"
#include <queue>
#include <climits>
#include <iostream>
#include <cstring>

using namespace std;
// Edges //

// void Graph::add_residual_edge(int v1, int v2)
// {
//     bool reverse_edge_exists = false;
//     for (const auto &e : vertices[v2].adj)
//     {
//         if (e.vertex == v1)
//         {
//             reverse_edge_exists = true;
//             break;
//         }
//     }
//     if (!reverse_edge_exists)
//     {
//         vertices[v2].adj.push_back({v1, 0, 0, ""});
//     }
// }
// Edges End //

/** Algoritmo Breadth First Search
 * Realiza uma busca em largura no grafo a partir do vertice s
 * Este algomritmo tem complexidade de tempo O(V + E)
 * @param s: vertice de partida
 * @param t: vertice de destino
 * @param parent: vetor de pais
 * @return true se existe um caminho de s a t, false caso contrario
 */
bool Graph::bfs(int s, int t, vector<int> &parent) {
    // Create a visited array and mark all vertices as not visited
    bool visited[num_vertices];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source vertex as visited
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (auto &e : vertices[u].adj)
        {
            if (visited[e.vertex] == false && e.flow < e.capacity)
            {
                q.push(e.vertex);
                parent[e.vertex] = u;
                visited[e.vertex] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    return (visited[t] == true);
}

/** Algoritmo Ford Fulkerson.
 * Percorre o grafo a partir do vertice source ate o vertice sink, e encontra o fluxo maximo.
 * Este algoritmo tem complexidade de tempo O(V * E^2).
 * @param source: vertice de partida.
 * @param sink: vertice de destino.
 * @return o fluxo maximo.
 */
int Graph::ford_fulkerson(int source, int sink) {

    //set flow to 0
    for (int u = 0; u < num_vertices; u++)
    {
        for (auto &e : vertices[u].adj)
        {
            e.flow = 0;
        }
    }

    int u, v;

    // This array is filled by BFS and to store path
    vector<int> parent(num_vertices);

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while there is path from source to sink
    while (bfs(source, sink, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        int path_flow = INT_MAX;
        for (v = sink; v != source; v = parent[v])
        {
            u = parent[v];
            for (auto &e : vertices[u].adj)
            {
                if (e.vertex == v)
                {
                    path_flow = min(path_flow, e.capacity - e.flow);
                    break;
                }
            }
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v = sink; v != source; v = parent[v])
        {
            u = parent[v];
            for (auto &e : vertices[u].adj)
            {
                if (e.vertex == v)
                {
                    e.flow += path_flow;
                    break;
                }
            }
            for (auto &e : vertices[v].adj)
            {
                if (e.vertex == u)
                {
                    e.flow -= path_flow;
                    break;
                }
            }
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow
    return max_flow;
}

// // Ford-Fulkerson algorithm
// int Graph::ford_fulkerson(int source, int sink)
// {
//     int max_flow = 0;
//     // set flows to 0
//     for (auto &v : vertices)
//     {
//         for (auto &e : v.adj)
//         {
//             e.flow = 0;
//         }
//     }
//     while (true)
//     {
//         vector<int> parent(num_vertices, -1);
//         queue<pair<int, int>> q;
//         q.push({source, INT_MAX});

//         while (!q.empty())
//         {
//             int current_vertex = q.front().first;
//             int flow = q.front().second;
//             q.pop();

//             for (const auto &edge : vertices[current_vertex].adj)
//             {
//                 if (parent[edge.vertex] == -1 && edge.vertex != source && edge.flow < edge.capacity)
//                 {
//                     int new_flow = min(flow, edge.capacity - edge.flow);
//                     parent[edge.vertex] = current_vertex;

//                     if (edge.vertex == sink)
//                     {
//                         max_flow += new_flow;
//                         int v = edge.vertex;
//                         while (v != source)
//                         {
//                             int u = parent[v];
//                             for (auto &e : vertices[u].adj)
//                             {
//                                 if (e.vertex == v)
//                                 {
//                                     e.flow += new_flow;
//                                     break;
//                                 }
//                             }
//                             for (auto &e : vertices[v].adj)
//                             {
//                                 if (e.vertex == u)
//                                 {
//                                     e.flow -= new_flow;
//                                     break;
//                                 }
//                             }
//                             v = u;
//                         }
//                         break;
//                     }
//                     q.push({edge.vertex, new_flow});
//                 }
//             }
//         }
//         if (parent[sink] == -1)
//         {
//             break;
//         }
//     }
//     return max_flow;
// }

// // Ford-Fulkerson algorithm
// int GraphList::ford_fulkerson(int source, int sink)
// {
//     int max_flow = 0;
//     while (true)
//     {
//         vector<int> parent(numVertices, -1);
//         queue<pair<int, int>> q;
//         q.push({source, INT_MAX});

//         while (!q.empty())
//         {
//             int current_vertex = q.front().first;
//             int flow = q.front().second;
//             q.pop();

//             for (const auto &edge : graph[current_vertex].adj)
//             {
//                 if (parent[edge.vertex] == -1 && edge.vertex != source && edge.flow < edge.weight)
//                 {
//                     int new_flow = min(flow, edge.weight - edge.flow);
//                     parent[edge.vertex] = current_vertex;

//                     if (edge.vertex == sink
//                                            ate agora so cuspiu mais isso)
//                     {
//                         max_flow += new_flow;
//                         int v = edge.vertex;
//                         while (v != source)
//                         {
//                             int u = parent[v];
//                             for (auto &e : graph[u].adj)
//                             {
//                                 if (e.vertex == v)
//                                 {
//                                     e.flow += new_flow;
//                                     break;
//                                 }
//                             }
//                             for (auto &e : graph[v].adj)
//                             {
//                                 if (e.vertex == u)
//                                 {
//                                     e.flow -= new_flow;
//                                     break;
//                                 }
//                             }
//                             v = u;
//                         }
//                         break;
//                     }
//                     q.push({edge.vertex, new_flow});
//                 }
//             }
//         }

//         if (parent[sink] == -1)
//         {
//             break;
//         }
//     }

//     return max_flow;
// }
