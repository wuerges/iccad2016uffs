#include <bdd_model.hpp>
#include <nand_bdd_adapter.hpp>
#include <verilog_parser.hpp>
#include <verilog_graph_adapter.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <memory>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace verilog;
using namespace verilog::bdd;
using namespace std::chrono;
using namespace std;

int main(int nargs, char** argv){

  ast::Verilog v;
  parser::parse_verilog_file(v, argv[1]);

  graph::G_builder b;
  convert(v, b);

  BDD bdds;
  convert(b.g, bdds);
  /*
  shared_ptr<Node> x, y, z;
  x = bdds.add_simple_input(5);
  y = bdds.add_simple_input(6);
  printf("//x = %p, y = %p\n", x, y);
  z = bdds.conjunction(x, y);
  */

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
