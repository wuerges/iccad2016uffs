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
#include <chrono>


using namespace verilog;
using namespace solution;
using namespace std;
using namespace std::chrono;



void generate_match(Generator & g, 
    graph::G_builder & g1, 
    graph::G_builder & g2, 
    const ast::Verilog & v1,
    const ast::Verilog & v2,
    const int max_duration,
    Match & m) {


  system_clock::time_point t1 = system_clock::now();
  SimulationResult r1, r2;

  // TODO make this run for a long time
  for (;;) {

    std::vector<bool>
      inputs_1,
      inputs_2,
      output_1, 
      output_2;
    g.generate_inputs(inputs_1, v1.inputs.size());
    g.generate_inputs(inputs_2, v2.inputs.size());

    simulate(inputs_1, output_1, g1);
    simulate(inputs_2, output_2, g2);


    r1.account_for(output_1, v1.outputs);
    r2.account_for(output_2, v2.outputs);

    system_clock::time_point t2 = system_clock::now();
    int duration = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
    if (duration > max_duration)
      break;
  }

  vector<Count> sorted_r1, sorted_r2;

  r1.get_counts(sorted_r1);
  r2.get_counts(sorted_r2);


  build_match(sorted_r1, sorted_r2, m);
}



struct name_map {
  int name_seed = 0;
  map<string, int> name_map;

  int get_name(string s) {
    auto it = name_map.find(s);

    if (it != name_map.end()) {
      return it->second;
    }
    else {
      name_map[s] = ++name_seed;
      return name_seed;
    }
  }
};


int main(int nargs, char** argv){

  if (nargs < 3) {
    std::cout << "usage: iccad2016solve <verilog1.v> <verilog2.v> <number of experiments>" << endl;
    return 0;
  }

  ast::Verilog v1, v2;
  parser::parse_verilog_file(v1, argv[1]);
  parser::parse_verilog_file(v2, argv[2]);

  stringstream ss;
  ss << argv[3];
  int number;
  ss >> number;

  graph::G_builder g1, g2;
  convert(v1, g1);
  convert(v2, g2);

  Generator g;
  int max_duration = 1;

  name_map names;


  for (int i = 0; i < number; ++i) {
    Match m;
    generate_match(g, g1, g2, v1, v2, max_duration, m);
    max_duration *= 2;

    int b = false;
    for (auto os : m.outputs) {
      if (b)
        cout << ", ";

      stringstream name;
      for (auto ps : os.second) {
        name << "(" << ps.first << ", " << ps.second << ")";
      }
      cout << names.get_name(name.str());


      b = true;
    }
    cout << endl;
  }





  //cout << m;

  return 0;
}
