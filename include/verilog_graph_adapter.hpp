
#pragma once

#include <verilog_model.hpp>
#include <graph_model.hpp>
#include <stdexcept>

namespace verilog {

  void add_function_and(ast::Function & f, graph::G & g) {

  }

  void convert(ast::Verilog & v, graph::G &g) {

    for(auto w : v.wires)
      g.add_vertex(w);

    for(auto f : v.functions) {
      switch (f.op) {
        case ast::Opcode::And: 
          add_function_and(f, g);
          break;
        default: 
          throw std::invalid_argument("Invalid Function name");
      }
    }
  }

}
