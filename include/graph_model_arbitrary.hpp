#pragma once

#include <graph_model.hpp>
#include <rapidcheck.h>

using namespace verilog;
using namespace graph;

namespace rc
{

  struct AG {
    G g;
    AG(int a) {}
  };


template<>
struct Arbitrary<G> {
  static Gen<AG> arbitrary() {
    //G * g = new G;
    auto vs = *gen::arbitrary<std::vector<std::string>>();
    auto ag = gen::construct<AG>(gen::arbitrary<int>());
    //return g;
    return ag;
  }
};


}
