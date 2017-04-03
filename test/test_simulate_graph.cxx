
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
#include <chrono>

using namespace verilog;
using namespace std::chrono;

int main(int nargs, char** argv){
  
  ast::Verilog v;
  parser::parse_verilog_file(v, argv[1]);


  graph::G_builder b;
  convert(v, b);


  std::map<std::string, bool> input, output1, output2;
  Generator gen;
  gen.generate_inputs(v.inputs, input, v.inputs.size());



  steady_clock::time_point t1 = steady_clock::now();
  simulate(input, output1, b);
  steady_clock::time_point t2 = steady_clock::now();
  simulate(input, output2, v);
  steady_clock::time_point t3 = steady_clock::now();


  std::cout << "nand    Simulation: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds." << std::endl;
  std::cout << "Netlist Simulation: " << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << " microseconds." << std::endl;
  
  for(auto it : v.outputs) {
    if (output1[it] != output2[it]) {
      std::cerr << "ERROR: output `" << it << "' differs\n";
      return -1;
    }
  }
  return 0;

}
