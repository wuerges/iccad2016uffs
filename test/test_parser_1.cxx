#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <verilog_parser.hpp>

int
main(int nargs, char** argv)
{
  using boost::spirit::istream_iterator;

  std::ifstream input(argv[1]);
  input.unsetf(std::ios::skipws);

  istream_iterator begin(input);
  istream_iterator end;
  return parser::parse_verilog(begin, end);
}
