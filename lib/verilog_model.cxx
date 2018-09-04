#include <verilog_model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace verilog;
using namespace verilog::ast;

std::map<Opcode, std::string> verilog::ast::opcode_string_map = {
  {Opcode::And , "and" },
  {Opcode::Nand, "nand"},
  {Opcode::Or  , "or"  },
  {Opcode::Nor , "nor" },
  {Opcode::Xor , "xor" },
  {Opcode::Xnor, "xnor"},
  {Opcode::Buf , "buf" },
  {Opcode::Not , "not" }
};

std::map<std::string, Opcode> verilog::ast::string_opcode_map = {
  {"and" , Opcode::And },
  {"nand", Opcode::Nand},
  {"or"  , Opcode::Or  },
  {"nor" , Opcode::Nor },
  {"xor" , Opcode::Xor },
  {"xnor", Opcode::Xnor},
  {"buf" , Opcode::Buf },
  {"not" , Opcode::Not }
};


std::ostream& verilog::ast::operator<< (std::ostream& stream, const Opcode& op) {
  stream << opcode_string_map[op];
  return stream;
}

Function:: Function(Opcode op_, std::vector<std::string> par_): op(op_), parameters(par_) {}

void Verilog::add_inputs(std::vector<std::string> const & vs) {
  inputs.insert(inputs.end(), vs.begin(), vs.end());
}

void Verilog::add_outputs(std::vector<std::string> const & vs) {
  outputs.insert(outputs.end(), vs.begin(), vs.end());
}

void Verilog::add_wires(std::vector<std::string> const & vs) {
  wires.insert(wires.end(), vs.begin(), vs.end());
}

void Verilog::add_ports(std::vector<std::string> const & vs) {
  ports.insert(ports.end(), vs.begin(), vs.end());
}

void Verilog::add_function(std::string const & op, std::vector<std::string> const & vs) {
  functions.push_back(Function(ast::string_opcode_map[op], vs));
}

