#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "graph.h"

template <typename T>
struct BellmanFordResult
{
  bool no_negative_cycles;
  std::vector<T> distances;
  std::vector<size_t> predecessors;
  size_t root;

  BellmanFordResult() {}

  BellmanFordResult(bool nc, std::vector<T> &d, std::vector<size_t> &p, size_t r)
  {
    no_negative_cycles = nc;
    distances = d;
    predecessors = p;
    root = r;
  }

  void print_distances(bool zero_indexed = true)
  {
    const T INFTY = std::numeric_limits<T>::max();
    auto print_idx = zero_indexed ? 0 : 1;

    for (size_t i = 0; i < distances.size(); i++)
    {
      if (distances[i] == INFTY)
        std::cout << "INF";
      else
        std::cout << distances[i];
      std::cout << "\t";
    }
    std::cout << std::endl;
  }

  void print_predecessors(bool zero_indexed = true)
  {
    const T INFTY = std::numeric_limits<T>::max();
    auto print_idx = zero_indexed ? 0 : 1;

    for (size_t i = 0; i < predecessors.size(); i++)
    {
      if (predecessors[i] == NO_PREDECESSOR)
        std::cout << "NIL";
      else
        std::cout << predecessors[i];
      std::cout << "\t";
    }
    std::cout << std::endl;
  }
};

/*
Calcula la distancia mínima desde una raíz `root` a todos los vértices la lista de adyacencia `g_adj_list`.
Requiere que `g_adj_list` esté inicializado con un grafo preexistente.
Retorna una 3-tupla con tres objetos:
- `bool`: Si el grafo contiene un ciclo negativo alcanzable desde `root`, retorna `false`. Si no, retorna `true`.
- `std::vector<T>`: `i`-ésima distancia de un camino más corto desde `root` hasta `i`.
- `std::vector<size_t>`: Lista donde el elemento `i`-ésimo es el predecesor del vértice `i`-ésimo en un camino más corto desde `root` a `i`.
*/
template <typename T>
BellmanFordResult<T> Bellman_Ford(AdjacencyList<T> &g_adj_list, size_t root)
{
  std::vector<T> dist_to_root;
  std::vector<size_t> predecessor;
  const T INFTY = std::numeric_limits<T>::max();

  // Initialize-Single-Source
  size_t n_verts = g_adj_list.size();
  dist_to_root.resize(n_verts, INFTY);
  predecessor.resize(n_verts, NO_PREDECESSOR);
  dist_to_root[root] = T();

  // Loop principal
  for (size_t i = 0; i < n_verts - 1; i++)
  {

    // relajar todas las aristas que salen desde el vértice u
    for (size_t u = 0; u < n_verts; u++)
    {
      // para evitar overflow, y además no se actualizará nada si dist(u) es infinita
      if (dist_to_root[u] == INFTY)
        continue;

      for (typename AdjacencyList<T>::EdgeHead edge : g_adj_list[u])
      {
        if (dist_to_root[edge.head_vertex] > dist_to_root[u] + edge.weight)
        {
          dist_to_root[edge.head_vertex] = dist_to_root[u] + edge.weight;
          predecessor[edge.head_vertex] = u;
        }
        // else, v no cambia
      }
    }
  }

  // Check de ciclos negativos con un último barrido de las aristas
  for (size_t u = 0; u < n_verts; u++)
  {
    for (typename AdjacencyList<T>::EdgeHead edge : g_adj_list[u])
    {
      if (dist_to_root[u] == INFTY) // u es inalcanzable desde root, no es posible detectar ciclo
        continue;

      if (dist_to_root[edge.head_vertex] > dist_to_root[u] + edge.weight)
      {
        BellmanFordResult<T> result{false, dist_to_root, predecessor, root};
        return result;
      }
    }
  }

  BellmanFordResult<T> result{true, dist_to_root, predecessor, root};
  return result;
}

// ¿Medir tiempo de creación de lista de adyacencia en experimentos? (pasar grafo "raw" a Bellman-Ford APSP)
// ¿o pasarle la lista de adyacencia ya hecha?
template <typename T>
std::vector<BellmanFordResult<T>> Bellman_Ford_APSP(AdjacencyList<T> &adjlist)
{
  size_t n_verts = adjlist.size();
  std::vector<BellmanFordResult<T>> apsp_result(n_verts);
  for (size_t v = 0; v < n_verts; v++)
  {
    apsp_result[v] = Bellman_Ford(adjlist, v);
  }
  return apsp_result;
}

/* Overload que acepta matriz de adyacencia y construye lista de adyacencia antes de llamar BF. */
template <typename T>
std::vector<BellmanFordResult<T>> Bellman_Ford_APSP(Graph<T> &adjmat)
{
  AdjacencyList<T> adjlist = AdjacencyList<T>(adjmat);
  return Bellman_Ford_APSP(adjlist);
}

#endif