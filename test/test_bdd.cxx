#include "bdd_model.hpp"
#include "bdd_graph_builder.hpp"
#include "verilog_parser.hpp"
#include "verilog_graph_adapter.hpp"

#include <iostream>
// #include <fstream>
// #include <string>
// #include <vector>
// #include <map>
// #include <chrono>

// #include <boost/graph/graph_traits.hpp>
// #include <boost/graph/adjacency_list.hpp>
//
using namespace verilog;
using bdd::BDD;
using bdd::Node;
// using namespace std::chrono;
// using namespace std;

int main(int nargs, char** argv){
  BDD bdds;
  // Node * x = bdds.add_simple_input("x");
  // Node * y = bdds.add_simple_input("y");
  // printf("//x = %p, y = %p\n", x, y);
  // Node * z = bdds.land(x, y);
  // Node * t = bdds.lor(x, z);

  ast::Verilog v;
  parser::parse_verilog_file(v, argv[1]);


  graph::G_builder b;
  convert(v, b);
  build(b.g, bdds);

  std::cout << bdds << '\n';

  /*
  BDD a("a");
  BDD b("b");

  BDD c = a & b;

  std::cout << a;
  std::cout << b;
  std::cout << c;
  */

  return 0;
}
