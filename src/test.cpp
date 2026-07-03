#include "graph.h"
#include "bellman-ford.h"
#include <iostream>

void error_msg();

int main(int argc, char* argv[]) {
  std::string filename = argv[1];
  bool is_zero_indexed;
  if (argc == 3) {
    std::string zero_index_option = argv[2];
    if (zero_index_option == "-1") is_zero_indexed = false;
    else if (zero_index_option == "-0") is_zero_indexed = true;
    else error_msg();
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
  auto t = Bellman_Ford(a, root);
  std::cout << "Root vertex: " << root << std::endl;
  std::cout << "No negative cycle?: " << t.no_negative_cycles << std::endl;
  std::cout << "Shortest path distances: " << std::endl;
  t.print_distances(is_zero_indexed);
  std::cout << "Shortest path predecessors: " << std::endl;
  t.print_predecessors(is_zero_indexed);

  return 0;
}

void error_msg() {
  std::cerr << "Usage: ./executable <path_to_graph_file> <> [-0 | -1]" << std::endl << "such that -0 is for zero indexed, -1 is for one indexed" << std::endl;
  exit(-1);
}