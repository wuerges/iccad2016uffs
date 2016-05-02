#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <verilog_parser.hpp>

int
main(int nargs, char** argv)
{
  using boost::spirit::istream_iterator;

  std::ifstream input(argv[1]);
  istream_iterator begin(input);
  istream_iterator end;
  parser::parse_verilog(begin, end);
  return 0;
}
