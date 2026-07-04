#include "graph.h"
#include "bellman-ford.h"
#include "floyd-warshall.h"
#include <iostream>

void error_msg();
template <typename T>
void debug_print_results(FloydWarshallResult<T> r, bool zero_indexed);
template <typename T>
void debug_print_results(BellmanFordResult<T> r, bool zero_indexed);

int main(int argc, char* argv[]) {
  bool is_zero_indexed;
  std::string filename;
  std::string algo_option;
  std::string zero_index_option;
  if (argc == 4) {
    filename = argv[1];
    algo_option = argv[2];
    zero_index_option = argv[3];

    if (zero_index_option == "-1")
      is_zero_indexed = false;
    else if (zero_index_option == "-0")
      is_zero_indexed = true;
    else
      error_msg();
  }
  else {
    error_msg();
  }
  
  Graph<double> g = Graph<double>(filename, true, is_zero_indexed);

  std::cout << "Adjacency matrix: " << std::endl;
  g.print();
  AdjacencyList<double> a = AdjacencyList<double>(g);
  std::cout << "Adjacency list: " << std::endl;
  a.print();

  size_t root = 0;
  
  if (algo_option == "--FW") {
    FloydWarshallResult<double> fw_r = Floyd_Warshall(g);
    debug_print_results<double>(fw_r, is_zero_indexed);
  }
  else if (algo_option == "--BF") {
    BellmanFordResult<double> bf_r = Bellman_Ford(a, root);
    debug_print_results<double>(bf_r, is_zero_indexed);
  }
  else
    error_msg();

  return 0;
}

void error_msg() {
  std::cerr << "Usage: ./executable <path_to_graph_file> <--FW | --BF> <-0 | -1>" << std::endl << "such that -0 is for zero indexed, -1 is for one indexed" << std::endl;
  exit(-1);
}

template <typename T>
void debug_print_results(FloydWarshallResult<T> r, bool zero_indexed) {
  std::cout << "No negative cycle?: " << r.no_negative_cycles << std::endl;
  std::cout << "Shortest path distances: " << std::endl;
  r.print_distances(zero_indexed);
  std::cout << "Shortest path predecessors: " << std::endl;
  r.print_predecessors(zero_indexed);
}


template <typename T>
void debug_print_results(BellmanFordResult<T> r, bool zero_indexed) {
  std::cout << "No negative cycle?: " << r.no_negative_cycles << std::endl;
  std::cout << "Shortest path distances: " << std::endl;
  r.print_distances(zero_indexed);
  std::cout << "Shortest path predecessors: " << std::endl;
  r.print_predecessors(zero_indexed);
}