
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

  void add_function_not(ast::Function & f, graph::G & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != --f.parameters.end();  ++it) {
      g.add_edge(orig, *it, graph::NegP::Negative);
    }
  }
  /// For the BUF port, the input is the back
  // The edges are Positive
  void add_function_buf(ast::Function & f, graph::G & g) {
    std::string orig = f.parameters.back();
    for(auto it = f.parameters.begin();
        it != --f.parameters.end();  ++it) {
      g.add_edge(orig, *it, graph::NegP::Positive);
    }
  }

  void add_function_xnor(std::string i1, std::string i2, std::string o, graph::G & g) {
    std::string t1 = g.new_named_vertex();
    std::string t2 = g.new_named_vertex();
    std::string t3 = g.new_named_vertex();
    std::string t4 = g.new_named_vertex();

    g.add_edge(i1, t1, graph::NegP::Positive);
    g.add_edge(i2, t1, graph::NegP::Positive);

    g.add_edge(i1, t2, graph::NegP::Positive);
    g.add_edge(t1, t2, graph::NegP::Negative);

    g.add_edge(i2, t3, graph::NegP::Positive);
    g.add_edge(t1, t3, graph::NegP::Negative);

    g.add_edge(t2, t4, graph::NegP::Negative);
    g.add_edge(t3, t4, graph::NegP::Negative);

    g.add_edge(t4, o, graph::NegP::Positive);
  }

  void add_function_xor(std::string i1, std::string i2, std::string o, graph::G & g) {
    std::string t1 = g.new_named_vertex();
    std::string t2 = g.new_named_vertex();
    std::string t3 = g.new_named_vertex();
    std::string t4 = g.new_named_vertex();

    g.add_edge(i1, t1, graph::NegP::Positive);
    g.add_edge(i2, t1, graph::NegP::Positive);

    g.add_edge(i1, t2, graph::NegP::Positive);
    g.add_edge(t1, t2, graph::NegP::Negative);

    g.add_edge(i2, t3, graph::NegP::Positive);
    g.add_edge(t1, t3, graph::NegP::Negative);

    g.add_edge(t2, t4, graph::NegP::Negative);
    g.add_edge(t3, t4, graph::NegP::Negative);

    g.add_edge(t4, o, graph::NegP::Negative);
  }

  void add_function_xnor(std::vector<std::string> & is, 
      std::string o, graph::G & g) {
    if (is.size() < 2) {
      throw std::invalid_argument("Input size is smaller than 2");
    }
    else if (is.size() == 2) {
      add_function_xnor(is.front(), is.back(), o, g);
    }
    else {
      std::string i1 = is.back();
      is.pop_back();
      std::string i2 = is.back();
      is.pop_back();

      std::string extra = g.new_named_vertex();
      add_function_xnor(i1, i2, extra, g);

      is.push_back(extra);
      add_function_xnor(is, o, g);
    }
  }

  void add_function_xor(std::vector<std::string> & is, 
      std::string o, graph::G & g) {
    if (is.size() < 2) {
      throw std::invalid_argument("Input size is smaller than 2");
    }
    else if (is.size() == 2) {
      add_function_xor(is.front(), is.back(), o, g);
    }
    else {
      std::string i1 = is.back();
      is.pop_back();
      std::string i2 = is.back();
      is.pop_back();

      std::string extra = g.new_named_vertex();
      add_function_xor(i1, i2, extra, g);

      is.push_back(extra);
      add_function_xor(is, o, g);
    }
  }

  void add_function_xor(ast::Function & f, graph::G & g) {
    std::vector<std::string> is(++f.parameters.begin(), f.parameters.end());
    
    add_function_xor(is, f.parameters.front(), g);
  }

  void add_function_xnor(ast::Function & f, graph::G & g) {
    std::vector<std::string> is(++f.parameters.begin(), f.parameters.end());
    
    add_function_xnor(is, f.parameters.front(), g);
  }

  // 
  void add_function_NP(ast::Function & f, graph::G & g, graph::NegP np_in, graph::NegP np_out) {
    std::string dest;
    if (np_out == graph::NegP::Negative) { 
      dest = g.new_named_vertex();
      g.add_edge(dest, f.parameters.front(), graph::NegP::Negative);
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

    g.inputs = v.inputs;
    std::sort(g.inputs.begin(), g.inputs.end());
    g.outputs = v.outputs;
    std::sort(g.outputs.begin(), g.outputs.end());

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
