#include <icarus_simulator.hpp>
#include <inputs_generator.hpp>
#include <verilog_parser.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>

using namespace verilog;

int main(int nargs, char** argv){
  
  ast::Verilog v;
  parser::parse_verilog_file(v, argv[1]);
  std::ofstream f;
  std::map<std::string, bool> input, output1;
  Generator gen;
  gen.generate_inputs(v.inputs, input);
  
  std::system(("rm s.v; cp " + std::string(argv[1]) + " s.v").c_str());
  
  f.open("s.v", std::fstream::app | std::fstream::ate);
  icarus::simulate(f, input, output1, v);
  f.close();
  
  
  
  std::system("iverilog s.v;./a.out > saidaIcarus");
  
  
  return 0;

}
