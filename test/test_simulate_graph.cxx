
#include <inputs_generator.hpp>
#include <verilog_parser.hpp>
#include <graph_simulator.hpp>
#include <verilog_simulator.hpp>
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


  std::vector<bool> input, output1, output2;
  Generator gen;
  gen.generate_inputs(input, v.inputs.size());


  simulate(input, output1, g);
  simulate(input, output2, v);
  
  if (output1 == output2)
    return 0;
  else 
    return -1;

}
