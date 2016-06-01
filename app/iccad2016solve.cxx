#include <graph_model.hpp>
#include <graph_simulator.hpp>
#include <inputs_generator.hpp>
#include <verilog_graph_adapter.hpp>

#include <verilog_parser.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>


using namespace verilog;

int main(int nargs, char** argv){



  if (nargs < 2) {
    std::cout << "usage: iccad2016solve <verilog1.v> <verilog2.v>";
    return 0;
  }

  ast::Verilog v1, v2;
  parser::parse_verilog_file(v1, argv[1]);
  parser::parse_verilog_file(v2, argv[2]);

  graph::G g1, g2;
  convert(v1, g1);
  convert(v2, g2);


  std::vector<std::vector<bool> > 
    inputs_1,
    inputs_2,
    outputs_1, 
    outputs_2;

  Generator g;
  for (int i = 0; i< 100000; ++i) {
    inputs_1.push_back({});
    inputs_2.push_back({});

    g.generate_inputs(inputs_1.back(), v1.inputs.size());
    g.generate_inputs(inputs_2.back(), v2.inputs.size());

    outputs_1.push_back({});
    outputs_2.push_back({});

    simulate(inputs_1.back(), outputs_1.back(), g1);
    simulate(inputs_2.back(), outputs_2.back(), g2);

  }



  return 0;
}
