
#include <verilog_parser.hpp>
#include <verilog_graph_adapter.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int
main(int nargs, char** argv)
{
  {
    verilog::ast::Verilog v;
    verilog::parser::parse_verilog_file(v, argv[1]);

    verilog::graph::G_builder g;
    verilog::convert(v, g);
    verilog::graph::write_graph(std::cout, g);
  }

}
