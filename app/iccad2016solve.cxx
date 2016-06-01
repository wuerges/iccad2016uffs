#include <verilog_parser.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <stdio.h>
#include <map>


using namespace verilog;
using namespace ast;
using namespace boost;
using namespace spirit;
using namespace parser;

int main(int nargs, char** argv){

  Verilog v1, v2;

  if (nargs < 2) {
    std::cout << "usage: iccad2016solve <verilog1.v> <verilog2.v>";
    return 0;
  }

  parse_verilog_file(v1, argv[1]);
  parse_verilog_file(v2, argv[2]);

  return 0;
}
