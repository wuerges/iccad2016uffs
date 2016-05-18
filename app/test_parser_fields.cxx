#include <verilog_parser.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/spirit/include/support_istream_iterator.hpp>

using namespace verilog;
using namespace ast;
using namespace boost;
using namespace spirit;
using namespace parser;

int main(int nargs, char** argv){
  std::vector<Verilog>tests;
  Verilog v;
  
  for(int i = 1; i < nargs; i++){
    std::ifstream input(argv[i]);
    input.unsetf(std::ios::skipws);
    istream_iterator begin(input);
    istream_iterator end;
    parse_verilog(v, begin, end);
    tests.push_back(v);
  }
  
  std::vector<Verilog>::iterator it;
  for(it = tests.begin(); it != tests.end(); it++){
    std::vector<std::string>::iterator is;
    std::vector<Function>::iterator ir; 

    std::cout <<"Iputs\n";  
    for(is = it->inputs.begin(); is != it->inputs.end(); is++)
      std::cout << *is <<"\n";
    
    std::cout <<"Outputs\n";
    for(is = it->outputs.begin(); is != it->outputs.end(); is++)
      std::cout << *is <<"\n";

    std::cout <<"Wires\n";
    for(is = it->wires.begin(); is != it->wires.end(); is++)
      std::cout << *is <<"\n";

    std::cout <<"Functions\n"; 
    for(ir = it->functions.begin(); ir != it->functions.end(); ir++){
      std::cout << "Name: "<< ir->op <<"\n";
      for(is = ir->parameters.begin(); is != ir->parameters.end();is++){
	std::cout << *is <<"\n";
      }
    }
  }

  std::ifstream input(argv[1]);
  input.unsetf(std::ios::skipws);
  istream_iterator begin(input);
  istream_iterator end;

 
  return parse_verilog(v, begin, end);

  
}
