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


  BDD bdds;
  int x = bdds.add_simple_input(5);
  int y = bdds.add_simple_input(6);
  printf("//x = %d, y = %d\n", x, y);
  bdds.conjunction(x, y);

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
