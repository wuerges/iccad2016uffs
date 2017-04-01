
#pragma once

#include <verilog_model.hpp>
#include <graph_model.hpp>
#include <sstream>
#include <algorithm>
#include <map>
#include <stdexcept>

namespace verilog {
  /// For the NOT port, the input is the back
  // The edges are Negative

  void add_function_not(ast::Function & f, graph::G_builder & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != --f.parameters.end();  ++it) {
      g.add_edge(orig, *it, NegP::Negative);
    }
  }
  /// For the BUF port, the input is the back
  // The edges are Positive
  void add_function_buf(ast::Function & f, graph::G_builder & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != --f.parameters.end();  ++it) {
      g.add_edge(orig, *it, NegP::Positive);
    }
  }

  /*
  void add_function_xnor_1(int i1, int i2, int o, graph::G_builder & b) {
    int t1 = b.g.new_vertex();
    int t2 = b.g.new_vertex();
    int t3 = b.g.new_vertex();
    int t4 = b.g.new_vertex();

    b.add_edge(i1, t1, NegP::Positive);
    b.add_edge(i2, t1, NegP::Positive);

    b.add_edge(i1, t2, NegP::Positive);
    b.add_edge(t1, t2, NegP::Negative);

    b.add_edge(i2, t3, NegP::Positive);
    b.add_edge(t1, t3, NegP::Negative);

    b.add_edge(t2, t4, NegP::Negative);
    b.add_edge(t3, t4, NegP::Negative);

    b.add_edge(t4, o, NegP::Positive);
  }
  */

  void add_function_xor_1(int i1, int i2, int o, graph::G_builder & b) {
    int t1 = b.g.new_vertex();
    int t2 = b.g.new_vertex();
    int t3 = b.g.new_vertex();
    int t4 = b.g.new_vertex();

    b.add_edge(i1, t1, NegP::Positive);
    b.add_edge(i2, t1, NegP::Positive);

    b.add_edge(i1, t2, NegP::Positive);
    b.add_edge(t1, t2, NegP::Negative);

    b.add_edge(i2, t3, NegP::Positive);
    b.add_edge(t1, t3, NegP::Negative);

    b.add_edge(t2, t4, NegP::Negative);
    b.add_edge(t3, t4, NegP::Negative);

    b.add_edge(t4, o, NegP::Negative);
  }

  void add_function_xor(int v1, std::vector<std::string> & is, 
      int vo, graph::G_builder & b) {
    if (is.empty()) {
      throw std::invalid_argument("Xor input size is smaller than 2");
    }
    else if (is.size() == 1) {
      int v2 = b.get_vertex(is.back());
      add_function_xor_1(v1, v2, vo, b);
    }
    else {
      int prev_o = b.g.new_vertex();
      int v2 = b.get_vertex(is.back());
      is.pop_back();
      add_function_xor_1(v1, v2, prev_o, b);
      add_function_xor(prev_o, is, vo, b);
    }
  }

  void add_function_xor(std::vector<std::string> & is, 
      int o, graph::G_builder & b) {
    if (is.size() < 2) {
      throw std::invalid_argument("Xor Input size is smaller than 2");
    }
    else {
      int v1 = b.get_vertex(is.back());
      is.pop_back();
      add_function_xor(v1, is, o, b);
    }
  }

  void add_function_xnor(std::vector<std::string> & is, 
      std::string o, graph::G_builder & b) {
    if (is.size() < 2) {
      throw std::invalid_argument("Xnor Input size is smaller than 2");
    }
    else {
      int extra = b.g.new_vertex();
      add_function_xor(is, extra, b);
      b.add_edge(extra, o, NegP::Negative);
    }
  }

  void add_function_xor(ast::Function & f, graph::G_builder & g) {
    std::vector<std::string> is(++f.parameters.begin(), f.parameters.end());
    
    int o = g.get_vertex(f.parameters.front());
    add_function_xor(is, o, g);
  }

  void add_function_xnor(ast::Function & f, graph::G_builder & g) {
    std::vector<std::string> is(++f.parameters.begin(), f.parameters.end());
    
    add_function_xnor(is, f.parameters.front(), g);
  }

  // 
  void add_function_NP(ast::Function & f, graph::G_builder & g, NegP np_in, NegP np_out) {
    int dest;
    if (np_out == NegP::Negative) { 
      dest = g.g.new_vertex();
      g.add_edge(dest, f.parameters.front(), NegP::Negative);
    }
    else {
      dest = g.get_vertex(f.parameters.front());
    }
    for(auto it = ++f.parameters.begin();
        it != f.parameters.end();  ++it) {
      g.add_edge(*it,dest, np_in);
    }
  }

  /// For the AND port, the output is the front
  void add_function_and(ast::Function & f, graph::G_builder & g) {
    add_function_NP(f, g, NegP::Positive, NegP::Positive);
  }
  
  // NOR is equivalent to AND with negation of all inputs.
  void add_function_nor(ast::Function & f, graph::G_builder & g) {
    add_function_NP(f, g, NegP::Negative, NegP::Positive);
  }

  /// The OR port is an NOR port with the output negated.
  // This needs an extra vertex.
  void add_function_or(ast::Function & f, graph::G_builder & g) {
    add_function_NP(f, g, NegP::Negative, NegP::Negative);
  }

  /// The NAD port is an AND port with the output negated.
  // This needs an extra vertex.
  void add_function_nand(ast::Function & f, graph::G_builder & g) {
    add_function_NP(f, g, NegP::Positive, NegP::Negative);
  }

  void convert(ast::Verilog & v, graph::G_builder &g) {

    g.inputs = v.inputs;
    //std::sort(g.inputs.begin(), g.inputs.end());
    g.outputs = v.outputs;
    //std::sort(g.outputs.begin(), g.outputs.end());

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
        case ast::Opcode::Xor: 
          add_function_xor(f, g);
          break;
        case ast::Opcode::Xnor: 
          add_function_xnor(f, g);
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
