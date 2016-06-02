
#pragma once

#include <verilog_model.hpp>
#include <graph_model.hpp>
#include <sstream>
#include <map>
#include <stdexcept>

namespace verilog {
  /// For the NOT port, the input is the back
  // The edges are Negative
  void add_function_not(ast::Function & f, graph::G & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != f.parameters.end();  ++it) {
      g.add_edge(orig, *it, graph::NegP::Negative);
    }
  }
  /// For the BUF port, the input is the back
  // The edges are Positive
  void add_function_buf(ast::Function & f, graph::G & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != f.parameters.end();  ++it) {
      g.add_edge(orig, *it, graph::NegP::Positive);
    }
  }

  // 
  void add_function_NP(ast::Function & f, graph::G & g, graph::NegP np_in, graph::NegP np_out) {
    std::string dest;
    if (np_out == graph::NegP::Negative) { 
      dest = g.new_named_vertex();
      g.add_edge(f.parameters.front(), dest, graph::NegP::Negative);
    }
    else {
      dest = f.parameters.front();
    }
    for(auto it = ++f.parameters.begin();
        it != f.parameters.end();  ++it) {
      g.add_edge(*it,dest, np_in);
    }
  }

  /// For the AND port, the output is the front
  void add_function_and(ast::Function & f, graph::G & g) {
    add_function_NP(f, g, graph::NegP::Positive, graph::NegP::Positive);
  }
  
  // NOR is equivalent to AND with negation of all inputs.
  void add_function_nor(ast::Function & f, graph::G & g) {
    add_function_NP(f, g, graph::NegP::Negative, graph::NegP::Positive);
  }

  /// The OR port is an NOR port with the output negated.
  // This needs an extra vertex.
  void add_function_or(ast::Function & f, graph::G & g) {
    add_function_NP(f, g, graph::NegP::Negative, graph::NegP::Negative);
  }

  /// The NAD port is an AND port with the output negated.
  // This needs an extra vertex.
  void add_function_nand(ast::Function & f, graph::G & g) {
    add_function_NP(f, g, graph::NegP::Positive, graph::NegP::Negative);
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
        case ast::Opcode::Buf: 
          add_function_buf(f, g);
          break;
        case ast::Opcode::Not: 
          add_function_not(f, g);
          break;
        case ast::Opcode::Or: 
          add_function_or(f, g);
          break;
        case ast::Opcode::Nor: 
          add_function_nor(f, g);
          break;
        case ast::Opcode::And: 
          add_function_and(f, g);
          break;
        case ast::Opcode::Nand: 
          add_function_nand(f, g);
          break;
        default: 
          std::stringstream ss;
          ss << "Invalid function name: " << f.op;
          throw std::invalid_argument(ss.str());
      }
    }
  }

}
