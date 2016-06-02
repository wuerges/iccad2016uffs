
#include <inputs_generator.hpp>
#include <verilog_parser.hpp>
#include <graph_simulator.hpp>
#include <verilog_graph_adapter.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace verilog;

int main(int nargs, char** argv){
  
  ast::Verilog v;
  parser::parse_verilog_file(v, argv[1]);


  graph::G g;
  convert(v, g);


  std::vector<bool> input, output;
  Generator gen;
  gen.generate_inputs(input, v.inputs.size());


  simulate(input, output, g);

  
  return 0;

}
