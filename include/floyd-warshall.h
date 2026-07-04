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
  // Como en cualquier momento sólo se utilizan recursiones nivel (k-1) y (k), 
  // sólo se necesitan 2 matrices adicionales, en vez de guardar todas desde D^(0) hasta D^(n).

  size_t n_verts = graph.size();
  std::vector<std::vector<T> > distances_k1, distances_k2;
  std::vector<std::vector<size_t> > predecessors_k1, predecessors_k2;
  T INFTY = std::numeric_limits<T>::max();

  distances_k1 = graph.adj_mat; // D^(0)
  distances_k2.resize(n_verts, std::vector<T>(n_verts, INFTY));
  predecessors_k1.resize(n_verts);
  predecessors_k2.resize(n_verts, std::vector<size_t>(n_verts, NO_PREDECESSOR));

  // Inicialización de \Pi^(0) (predecessors_k1)
  for (size_t i = 0; i < n_verts; i++) {
    predecessors_k1[i].resize(n_verts);
    for (size_t j = 0; j < n_verts; j++) {
      if (i == j || graph[i][j] == INFTY)
        predecessors_k1[i][j] = NO_PREDECESSOR;
      else
        predecessors_k1[i][j] = i;
    }
  }

  // Loop principal
  // Alternar entre distances_k1 y distances_k2 para elegir cuál es (k-1) y cuál es (k)
  std::vector<std::vector<T>> *k_dist;
  std::vector<std::vector<T>> *k_minus1_dist;
  std::vector<std::vector<size_t>> *k_pred;
  std::vector<std::vector<size_t>> *k_minus1_pred;

  for (size_t k = 0; k < n_verts; k++) {
    // Matriz (k) inicial es siempre k1, (k-1) inicial es k2.
    if (k % 2 == 0) {
      k_minus1_dist = &distances_k1;
      k_minus1_pred = &predecessors_k1;
      k_dist = &distances_k2;
      k_pred = &predecessors_k2;
    }
    else {
      k_minus1_dist = &distances_k2;
      k_minus1_pred = &predecessors_k2;
      k_dist = &distances_k1;
      k_pred = &predecessors_k1;
    }

    for (size_t i = 0; i < n_verts; i++) {
      for (size_t j = 0; j < n_verts; j++) {
        // Primeras 2 condiciones para evitar overflow
        // Si la distancia k-1 no mejora con el detour, se conserva dist(k-1) en dist(k).
        if ((*k_minus1_dist)[i][k] == INFTY 
        || (*k_minus1_dist)[k][j] == INFTY
        || (*k_minus1_dist)[i][j] <= (*k_minus1_dist)[i][k] + (*k_minus1_dist)[k][j]) 
        {
          (*k_dist)[i][j] = (*k_minus1_dist)[i][j];
          (*k_pred)[i][j] = (*k_minus1_pred)[i][j];
        }

        else /*(distances_k[i][j] > distances_(k-1)[i][k] + distances_(k-1)[k][j])*/ {
          (*k_dist)[i][j] = (*k_minus1_dist)[i][k] + (*k_minus1_dist)[k][j];
          (*k_pred)[i][j] = (*k_minus1_pred)[k][j]; 
        }
      }
      // Detección de ciclos negativos
      if ((*k_dist)[i][i] < T())
        return FloydWarshallResult<T>{false, *k_dist, *k_pred};
    }
  }

  return FloydWarshallResult<T>{true, *k_dist, *k_pred};
}

#endif