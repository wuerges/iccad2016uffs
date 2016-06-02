#pragma once

#include <boost/random.hpp>

#include <vector>

namespace verilog {

  struct Generator {
    Generator(): bd(0.5) {}
    boost::random::mt19937 rng;
    boost::random::bernoulli_distribution<> bd;
    //  Generates a random input of a given size
    void generate_inputs(std::vector<bool> & inputs, int size) {
      for (int i = 0; i < size; ++i) 
        inputs.push_back(bd(rng));
    }
  };

}
