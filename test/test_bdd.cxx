
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

using namespace verilog;
using namespace std::chrono;

int main(int nargs, char** argv){
  
  //ast::Verilog v;
  //parser::parse_verilog_file(v, argv[1]);


  bdd::BDD a("a");
  bdd::BDD b("b");
  bdd::BDD c("c");
  bdd::BDD d("d");
/*
  convert(v, g);


  std::vector<bool> input, output1, output2;
  Generator gen;
  gen.generate_inputs(input, v.inputs.size());



  steady_clock::time_point t1 = steady_clock::now();
  simulate(input, output1, g);
  steady_clock::time_point t2 = steady_clock::now();
  simulate(input, output2, v);
  steady_clock::time_point t3 = steady_clock::now();


  std::cout << "nand    Simulation: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds." << std::endl;
  std::cout << "Netlist Simulation: " << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << " microseconds." << std::endl;
  
  if (output1 == output2)
    return 0;
  else 
    return -1;

*/
}
