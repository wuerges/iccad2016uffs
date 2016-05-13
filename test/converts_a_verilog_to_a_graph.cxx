
#include <verilog_parser.hpp>
#include <verilog_graph_adapter.hpp>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int
main(int nargs, char** argv)
{
  using boost::spirit::istream_iterator;

  std::ifstream input(argv[1]);
  input.unsetf(std::ios::skipws);

  istream_iterator begin(input);
  istream_iterator end;

  {
    verilog::ast::Verilog v;
    verilog::parser::parse_verilog(v, begin, end);

    verilog::graph::G g;
    verilog::convert(v, g);
  }


}
