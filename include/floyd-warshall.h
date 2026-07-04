#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include "graph.h"

template <typename T>
struct FloydWarshallResult {
  bool no_negative_cycles;
  std::vector<std::vector<T> > distances;
  std::vector<std::vector<size_t> > predecessors;
  T INFTY = std::numeric_limits<T>::max();

  void print_distances(bool zero_indexed = true) {
    auto print_idx = zero_indexed ? 0 : 1;

    for (auto i = 0; i < distances.size(); i++) {
      std::cout << "v_" << print_idx << ": \t";
      for (auto j = 0; j < distances.size(); j++) {
        if (distances[i][j] == INFTY)
          std::cout << "INF\t";
        else
          std::cout << distances[i][j] << '\t';
      }
      std::cout << std::endl;
      print_idx++;
    }
  }

  void print_predecessors(bool zero_indexed = true) {
    auto print_idx = zero_indexed ? 0 : 1;

    for (auto i = 0; i < predecessors.size(); i++) {
      std::cout << "v_" << print_idx << ": \t";
      for (auto j = 0; j < predecessors.size(); j++) {
        if (predecessors[i][j] == NO_PREDECESSOR)
          std::cout << "NIL\t";
        else
          std::cout << predecessors[i][j] << '\t';
      }
      std::cout << std::endl;
      print_idx++;
    }
  }
};

template <typename T>
FloydWarshallResult<T> Floyd_Warshall(Graph<T>& graph) {
  // Como en cualquier momento sólo se utilizan recursiones nivel k-1 y k, sólo se necesitan 2 matrices adicionales, en vez de guardar todas desde D^(0) hasta D^(n).
  // En realidad, sólo se necesita una matriz de predecesores.
  // Y más aún, sólo se necesita una matriz adicional de distancias, pues las escrituras son siempre a (i,j)..
  // https://stackoverflow.com/questions/30836663/why-does-floyd-warshall-just-use-one-distance-matrix

  size_t n_verts = graph.size();
  std::vector<std::vector<T> > distances_k;
  std::vector<std::vector<size_t> > predecessors_k;
  T INFTY = std::numeric_limits<T>::max();

  distances_k = graph.adj_mat;
  predecessors_k.resize(n_verts);

  // Inicialización de \Pi^(0) (predecessors_k)
  for (size_t i = 0; i < n_verts; i++) {
    predecessors_k[i].resize(n_verts);
    for (size_t j = 0; j < n_verts; j++) {
      if (i == j || graph[i][j] == INFTY)
        predecessors_k[i][j] = NO_PREDECESSOR;
      else
        predecessors_k[i][j] = i;
    }
  }

  // Loop principal
  for (size_t k = 0; k < n_verts; k++) {
    for (size_t i = 0; i < n_verts; i++) {
      for (size_t j = 0; j < n_verts; j++) {
        // evitar overflow, si uno de los dos es infinito no hay update
        if (distances_k[i][k] == INFTY || distances_k[k][j] == INFTY)
          continue;

        if (distances_k[i][j] > distances_k[i][k] + distances_k[k][j]) {
          distances_k[i][j] = distances_k[i][k] + distances_k[k][j];
          predecessors_k[i][j] = predecessors_k[k][j]; 
        }
        // else, no hay mejora por el detour, no hay cambio
      }

      // Detección de ciclos negativos
      if (distances_k[i][i] < T())
        return FloydWarshallResult<T>{false, distances_k, predecessors_k};
    }
  }

  return FloydWarshallResult<T>{true, distances_k, predecessors_k};
}

#endif