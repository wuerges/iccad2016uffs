
#pragma once

#include <verilog_model.hpp>
#include <graph_model.hpp>
#include <sstream>
#include <map>
#include <stdexcept>

namespace verilog {

  /// For the NOT port, the input is the back
  void add_function_not(ast::Function & f, graph::G & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != f.parameters.end();  ++it) {
      g.add_edge(orig, *it, graph::NegP::Negative);
    }
  }

  /// For the AND port, the output is the front
  void add_function_and(ast::Function & f, graph::G & g) {
    std::string dest = f.parameters.front();
    for(auto it = ++f.parameters.begin();
        it != f.parameters.end();  ++it) {
      g.add_edge(*it,dest, graph::NegP::Positive);
    }
  }

  void convert(ast::Verilog & v, graph::G &g) {

    for(auto w : v.outputs)
      g.add_vertex(w);

    for(auto w : v.inputs)
      g.add_vertex(w);

    for(auto w : v.wires)
      g.add_vertex(w);

    for(auto f : v.functions) {
      switch (f.op) {
        case ast::Opcode::Not: 
          add_function_not(f, g);
          break;
        case ast::Opcode::And: 
          add_function_and(f, g);
          break;
        default: 
          std::stringstream ss;
          ss << "Invalid function name: " << f.op;
          throw std::invalid_argument(ss.str());
      }
    }
  }

}
