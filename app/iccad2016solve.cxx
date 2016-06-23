#include <graph_model.hpp>
#include <graph_simulator.hpp>
#include <inputs_generator.hpp>
#include <verilog_graph_adapter.hpp>
#include <simulation_model.hpp>

#include <verilog_parser.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


using namespace verilog;
using namespace solution;
using namespace std;




int main(int nargs, char** argv){



  if (nargs < 2) {
    std::cout << "usage: iccad2016solve <verilog1.v> <verilog2.v>" << endl;
    return 0;
  }

  ast::Verilog v1, v2;
  parser::parse_verilog_file(v1, argv[1]);
  parser::parse_verilog_file(v2, argv[2]);

  graph::G g1, g2;
  convert(v1, g1);
  convert(v2, g2);


  std::vector<bool>
    inputs_1,
    inputs_2,
    output_1, 
    output_2;

  Generator g;
  SimulationResult r1, r2;

  // TODO make this run for a long time
  for (int i = 0; i< 100; ++i) {
    inputs_1.clear();
    inputs_2.clear();
    output_1.clear();
    output_2.clear();

    g.generate_inputs(inputs_1, v1.inputs.size());
    g.generate_inputs(inputs_2, v2.inputs.size());

    simulate(inputs_1, output_1, g1);
    simulate(inputs_2, output_2, g2);

    r1.account_for(output_1, v1.outputs);
    r2.account_for(output_2, v2.outputs);
  }

  vector<Count> sorted_r1, sorted_r2;

  r1.get_counts(sorted_r1);
  r2.get_counts(sorted_r2);


  Match m;
  build_match(sorted_r1, sorted_r2, m);

  return 0;
}
