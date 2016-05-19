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

      std::map<Opcode, std::string> opcode_string_map = {
        {Opcode::And , "and" },
        {Opcode::Nand, "nand"},
        {Opcode::Or  , "or"  },
        {Opcode::Nor , "nor" },
        {Opcode::Xor , "xor" },
        {Opcode::Xnor, "xnor"},
        {Opcode::Buf , "buf" },
        {Opcode::Not , "not" }
      };

      std::map<std::string, Opcode> string_opcode_map = {
        {"and" , Opcode::And },
        {"nand", Opcode::Nand},
        {"or"  , Opcode::Or  },
        {"nor" , Opcode::Nor },
        {"xor" , Opcode::Xor },
        {"xnor", Opcode::Xnor},
        {"buf" , Opcode::Buf },
        {"not" , Opcode::Not }
      };


     std::ostream& operator<< (std::ostream& stream, const Opcode& op) {
     stream << opcode_string_map[op];
      return stream;
    }

    struct Function {
      Opcode op;
      std::vector<std::string> parameters;
      Function(Opcode op_, auto par_): op(op_), parameters(par_) {}
    };
    struct Verilog {

      std::vector<std::string> inputs, outputs, wires, ports;
      std::vector<Function> functions;

      void add_inputs(std::vector<std::string> const & vs) {
        inputs.insert(inputs.end(), vs.begin(), vs.end());
      }

      void add_outputs(std::vector<std::string> const & vs) {
        outputs.insert(outputs.end(), vs.begin(), vs.end());
      }
      
      void add_wires(std::vector<std::string> const & vs) {
        wires.insert(wires.end(), vs.begin(), vs.end());
      }

      void add_ports(std::vector<std::string> const & vs) {
        ports.insert(ports.end(), vs.begin(), vs.end());
      }

      void add_function(std::string const & op, std::vector<std::string> const & vs) {
        functions.push_back(Function(ast::string_opcode_map[op], vs));
      }

    };
  }
}

