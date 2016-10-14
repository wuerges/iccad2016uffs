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

  BDD c = a & b;

  std::cout << c;

  return 0;
}
