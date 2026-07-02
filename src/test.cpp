#include "graph.h"
#include <iostream>

void error_msg();

int main(int argc, char* argv[]) {
  if (argc != 2) error_msg();
  std::string filename = argv[1];
  bool zero_indexed = true;
  bool is_directed = true;
  Graph<double> g = Graph<double>(filename, is_directed, zero_indexed);
  g.print();

  AdjacencyList<double> a = AdjacencyList<double>(g);
  a.print();

  return 0;
}

void error_msg() {
  std::cerr << "Usage: ./executable <path_to_graph_file>";
}