#pragma once

#include <vector>

namespace verilog {

  struct Generator {
    //  Generates a random input of a given size
    void generate_inputs(vector<bool> & inputs, int size);
  };

}
