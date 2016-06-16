#pragma once

#include <map>
#include <vector>
#include <pair>


namespace verilog {
namespace solution {
  using namespace std;

  typedef map<string, int > entropy_map;
  typedef map<string, bool > result_map;


  struct Match {
      map<string, vector<string> > inputs; 
      map<string, vector<string> > outputs; 
      vector<string> zeroes;
  };

  struct SimulationResult {
    entropy_map inputs, outputs;
    int number_of_runs;

    void account_for(const & input_result_map, const & output_result_map) {

      for(auto kv : input_result_map)
        if (kv.second)
          inputs[kv.first]++;

      for(auto kv : output_result_map)
        if (kv.second)
          outputs[kv.first]++;

      number_of_runs++;
    }

  };

  void matching(const & SimulationResult, Match &) {
  }

}
}
