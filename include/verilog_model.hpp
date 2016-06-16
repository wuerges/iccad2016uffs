#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace verilog 
{
  namespace ast {

    enum class Opcode {
      Nand, And, Or, Nor, Xor, Xnor, Buf, Not
    };

    extern std::map<Opcode, std::string> opcode_string_map;
    extern std::map<std::string, Opcode> string_opcode_map;
    extern std::ostream& operator<< (std::ostream& stream, const Opcode& op);

    struct Function {
      Opcode op;
      std::vector<std::string> parameters;
      Function(Opcode op_, auto par_);
    };
    struct Verilog {

      std::vector<std::string> inputs, outputs, wires, ports;
      std::vector<Function> functions;

      void add_inputs(std::vector<std::string> const & vs);
      void add_outputs(std::vector<std::string> const & vs);
      void add_wires(std::vector<std::string> const & vs);
      void add_ports(std::vector<std::string> const & vs);
      void add_function(std::string const & op, std::vector<std::string> const & vs);

    };
  }
}

