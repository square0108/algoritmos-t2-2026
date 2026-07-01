#include <vector>
#include <limits>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

template <typename weight_T>
struct Graph{
  std::vector<std::vector<weight_T>> adj_mat;

  Graph(void) {
  }

  Graph(const size_t n_verts) {
    // Fill de todo el grafo con "distancia" infinita
    adj_mat.resize(
      n_verts, 
      std::vector<weight_T>(
        n_verts, 
        std::numeric_limits<weight_T>::max()
      )
    );

    for (size_t i = 0; i < n_verts; i++) {
      adj_mat[i][i] = weight_T(); // Todos los nodos tienen dist 0 consigos mismos por defecto
      // (podrían haber self loops?)
    }
  }

  // Overload para parsear grafo desde archivo
  Graph(const std::string filename, bool zero_indexed = true) {
    std::ifstream input(filename);
    std::string line;

    std::getline(input, line);
    std::stringstream ss(line);

    size_t n_verts,n_edges,u,v;
    weight_T weight;

    ss >> n_verts >> n_verts >> n_edges;

    adj_mat.resize(
      n_verts,
      std::vector<weight_T>(
        n_verts,
        std::numeric_limits<weight_T>::max();
      )
    );

    for (size_t i = 0; i < n_verts; i++) {
      adj_mat[i][i] = weight_T(); // Todos los nodos tienen dist 0 consigos mismos por defecto
    }

    // Parsear grafo arista por arista, en tuplas (vert1, vert2, peso arista)
    while (n_edges--) {
      std::getline(input, line);
      ss.clear();
      ss.str(std::string());
      ss.str(line);

      ss >> u >> v >> weight;

      if (zero_indexed) {
        u--;
        v--;
      }

      // Evitar loops
      if (u == v) {
        continue;
      }

      adj_mat[u][v] = std::min(adj[u][v], weight);
      adj_mat[v][u] = std::min(adj[v][u], weight);
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
    const weight_T T_max = std::numeric_limits<weight_T>::max();

    for (size_t i = 0; i < n_verts; i++) {
      // Te quiero mucho operador ternario
      std::cout << (adj_mat[i][0] == T_max ? -1 : adj_mat[i][0]);

      for (size_t j = 1; j < n_verts; j++) {
        std::cout << ' ' << (adj_mat[i][j] == T_max ? -1 : adj_mat[i][j]);
      }

      std::cout << std::endl;
    }
  }
};
