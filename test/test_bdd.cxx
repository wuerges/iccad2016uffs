
//#include <inputs_generator.hpp>
//#include <verilog_parser.hpp>
//#include <graph_simulator.hpp>
//#include <verilog_simulator.hpp>
//#include <verilog_graph_adapter.hpp>

#include <bdd_model.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace verilog::bdd;
using namespace std::chrono;

int main(int nargs, char** argv){
  
  BDD a("a");
  BDD b("b");

  /**
   * tests if number of nodes of a, d, c and d are equal
   */

  if (a.size() != 3) {
    std::cerr 
      << "BDD a should have size 3 but had size: " 
      << a.size() << "\n";
    return -1;
  }
  if (b.size() != 3) {
    std::cerr 
      << "BDD b should have size 3 but had size: " 
      << b.size() << "\n";
    return -2;
  }

  BDD c = a & b;

  c.source = a.source;
  if (c.size() != 4) {
    std::cerr 
      << "BDD c should have size 4 but had size: " 
      << c.size() << "\n";
    return -3;
  }
  
  BDD d = c;
  d.reverse();

  if (c != d) {
    std::cerr 
      << "c == d\n";
    return -4;
  }

  BDD e = d;
  e.reverse();

  if (e != d) {
    std::cerr 
      << "e != d\n";
    return -5;
  }


  std::cout << c;

  return 0;
}
