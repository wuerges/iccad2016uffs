#pragma once

#include <boost/random.hpp>

#include <vector>
#include <map>

namespace verilog {

  struct Generator {
    Generator(): bd(0.5) {}
    boost::random::mt19937 rng;
    boost::random::bernoulli_distribution<> bd;
    //  Generates a random input of a given size
    void generate_inputs(
        const std::vector<std::string> & names, 
        std::map<std::string, bool> & inputs) {
      for(auto n : names) {
        inputs[n] = bd(rng);
      }
    }
  };

}
