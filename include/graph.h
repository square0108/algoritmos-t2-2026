#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <forward_list>
#include <sstream>

const size_t NO_PREDECESSOR = std::numeric_limits<size_t>::max();

template <typename T>
struct Graph{
  std::vector<std::vector<T>> adj_mat;

  Graph(void) {
  }

  Graph(const size_t n_verts) {
    // Fill de todo el grafo con "distancia" infinita
    adj_mat.resize(
      n_verts, 
      std::vector<T>(
        n_verts, 
        std::numeric_limits<T>::max()
      )
    );

    for (size_t i = 0; i < n_verts; i++) {
      adj_mat[i][i] = T(); // Todos los nodos tienen dist 0 consigos mismos por defecto
      // (podrían haber self loops?)
    }
  }

  // Overload para parsear grafo desde archivo
  Graph(const std::string filename, bool is_directed = true, bool zero_indexed = true)
  {
    std::ifstream input(filename);
    std::string line;

    std::getline(input, line);
    std::stringstream ss(line);

    size_t n_verts,n_edges,u,v;
    T weight;

    ss >> n_verts >> n_verts >> n_edges;

    adj_mat.resize(
      n_verts,
      std::vector<T>(
        n_verts,
        std::numeric_limits<T>::max()
      )
    );

    for (size_t i = 0; i < n_verts; i++) {
      adj_mat[i][i] = T(); // Todos los nodos tienen dist 0 consigos mismos por defecto
    }

    // Parsear grafo arista por arista, en tuplas (vert1, vert2, peso arista)
    while (n_edges--) {
      std::getline(input, line);
      ss.clear();
      ss.str(std::string());
      ss.str(line);

      ss >> u >> v >> weight;

      if (!zero_indexed) {
        u--;
        v--;
      }

      // Evitar loops
      if (u == v) {
        continue;
      }

      adj_mat[u][v] = std::min(adj_mat[u][v], weight);
      if (!is_directed)
        adj_mat[v][u] = std::min(adj_mat[v][u], weight); 
    }
  }

  // Acceso a fila de matriz
  std::vector<T>& operator[](size_t i) {
    return adj_mat[i];
  }

  size_t size(void) {
    return adj_mat.size();
  }

  void print(void) {
    size_t n_verts = size();
    const T T_max = std::numeric_limits<T>::max();

    for (size_t i = 0; i < n_verts; i++) {

      for (size_t j = 0; j < n_verts; j++) {
        if (adj_mat[i][j] == T_max)
          std::cout << "INF";
        else
          std::cout << adj_mat[i][j];
        std::cout << ' ' << '\t';
      }

      std::cout << std::endl;
    }
  }
};

template <typename T>
struct AdjacencyList{
  // "Cola" de una arista (pues lista ady. sólo enlaza vecinos)
  struct EdgeHead {
    size_t head_vertex; // head --> tail
    T weight;
  };

  std::vector<std::forward_list<EdgeHead>> adj_list;

  AdjacencyList(){
  }

  AdjacencyList(Graph<T>& g) {
    size_t n_verts = g.size();
    adj_list.resize(
      n_verts,
      std::forward_list<EdgeHead>()
    );

    T T_max = std::numeric_limits<T>::max();

    // Insertar vecinos de cada vértice en la matriz de adyacencia dentro de la lista de adyacencia
    for (size_t i = 0; i < n_verts; i++) {
      for (size_t j = 0; j < n_verts; j++) {
        if (g[i][j] != T() && g[i][j] != T_max) {
          EdgeHead new_neighbor = {j,g[i][j]};
          (adj_list[i]).push_front(new_neighbor);
        }
      }
    }
  }

  // Get the list of vertex i's neighbors
  std::forward_list<EdgeHead> &operator[](size_t i) {
    return adj_list[i];
  }

  size_t size(void) {
    return adj_list.size();
  }

  void print(void) {
    size_t n_verts = adj_list.size();
    const T T_max = std::numeric_limits<T>::max();

    for (size_t i = 0; i < n_verts; i++) {
      std::cout << i << ": ";
      std::forward_list<EdgeHead> list = adj_list[i];

      for (auto& tail_edge : list) {
        std::cout << "(" 
        << tail_edge.head_vertex 
        << ", "
        << tail_edge.weight
        << ")"
        << '\t';
      }

      std::cout << std::endl;
    }
  }
};

#endif