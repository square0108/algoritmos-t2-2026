#include "graph.h"
#include "bellman-ford.h"
#include "floyd-warshall.h"
#include <iostream>

void error_msg();
template <typename T>
void debug_print_results(FloydWarshallResult<T> r, bool zero_indexed);
template <typename T>
void debug_print_results(std::vector<BellmanFordResult<T>> bf_r, bool zero_indexed);
template <typename T>
void print_csv(std::vector<BellmanFordResult<T>> bf_apsp_r, bool zero_indexed);
template <typename T>
void print_csv(FloydWarshallResult<T> fw_r, bool zero_indexed);

int main(int argc, char *argv[])
{
  bool is_zero_indexed, print_results, write_to_csv;
  std::string filename, algo_option, zero_index_option, print_option;
  if (argc == 4 || argc == 5) {
    filename = argv[1];
    algo_option = argv[2];
    zero_index_option = argv[3];
    // Parsing user input
    if (argc == 5) {
      // This Shit sucks !!
      print_option = argv[4];
      if (print_option == "--print-results") {
        print_results = true;
        write_to_csv = false;
      }
      else if (print_option == "--print-csv") {
        print_results = false;
        write_to_csv = true;
      }
      else {
        print_results = false;
        write_to_csv = false;
      }
    }

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
  
  // Load .mtx graph into memory
  Graph<double> g = Graph<double>(filename, true, is_zero_indexed);
  AdjacencyList<double> a = AdjacencyList<double>(g);

  if (print_results)
  {
    //std::cout << "Adjacency matrix: " << std::endl;
    g.print();
    //std::cout << "Adjacency list: " << std::endl;
    //a.print();
  }

  // Decide which algorithm to use
  if (algo_option == "--FW") {
    FloydWarshallResult<double> fw_r = Floyd_Warshall(g);
    if (print_results) debug_print_results(fw_r, is_zero_indexed);
    else if (write_to_csv) print_csv(fw_r, is_zero_indexed);
  }
  else if (algo_option == "--BF") {
    std::vector<BellmanFordResult<double>> bf_r = Bellman_Ford_APSP(g);
    if (print_results) debug_print_results(bf_r, is_zero_indexed);
    else if (write_to_csv) print_csv(bf_r, is_zero_indexed);
  }
  else
    error_msg();

  return 0;
}

void error_msg() {
  std::cerr << "Usage: ./executable <path_to_graph_file> <--FW | --BF> <-0 | -1> [--print-results]" << std::endl << "such that -0 is for zero indexed, -1 is for one indexed" << std::endl;
  std::cerr << "<> denotes mandatory arguments, [] denotes optional arguments." << std::endl;
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
void debug_print_results(std::vector<BellmanFordResult<T>> bf_r, bool zero_indexed) {
  std::cout << "Shortest path distances" << std::endl;
  for (size_t i = 0; i < bf_r.size(); i++) {
    BellmanFordResult<T> r = bf_r[i];
    r.print_distances(zero_indexed);
  }
  std::cout << "Predecessors" << std::endl;
  for (size_t i = 0; i < bf_r.size(); i++)
  {
    BellmanFordResult<T> r = bf_r[i];
    r.print_predecessors(zero_indexed);
  }
}

// Prints in the format of a csv file where each entry is (node1),(node2),(SP Dist from node1 to node2)
template <typename T>
void print_csv(FloydWarshallResult<T> fw_r, bool zero_indexed)
{
  size_t offset = 1;
  if (zero_indexed) offset--;

  for (size_t i = 0; i < fw_r.distances.size(); i++) {
    for (size_t j = 0; j < fw_r.distances.size(); j++) {
      T dist = (fw_r.distances[i])[j];
      if (dist == std::numeric_limits<T>::max())
        std::cout << i + offset << "," << j + offset << "," << "INF" << std::endl;
      else
        std::cout << i + offset << "," << j + offset << "," << (fw_r.distances[i])[j] << std::endl;
    }
  }
}

template <typename T>
void print_csv(std::vector<BellmanFordResult<T>> bf_apsp_r, bool zero_indexed) {
  size_t offset = 1;
  if (zero_indexed) offset--;

  for (size_t i = 0; i < bf_apsp_r.size(); i++) {
    for (size_t j = 0; j < (bf_apsp_r[i]).distances.size(); j++) {
      T dist = (bf_apsp_r[i]).distances[j];
      if (dist == std::numeric_limits<T>::max())
        std::cout << i + offset << "," << j + offset << "," << "INF" << std::endl;
      else 
        std::cout << i + offset << "," << j + offset << "," << (bf_apsp_r[i]).distances[j] << std::endl;
    }
  }
}