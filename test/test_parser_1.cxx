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

struct verilogProperties{
  std::string name;
  int inputs, outputs, ports, wires, functions;
  std::map<Opcode,int> qtd;
};


int main(int nargs, char** argv){
  
  std::vector<verilogProperties >verilogFinal;
  
  for(int i = 1 , j = 0; i < nargs; i++){
    Verilog v;
    verilogProperties tmp;
    std::ifstream input(argv[i]);
    input.unsetf(std::ios::skipws);
    istream_iterator begin(input);
    istream_iterator end;
    parse_verilog(v, begin, end);
   
    std::vector<std::string>::iterator is;
   
    std::cout << v.ports.size() << "\n"; 
    tmp.inputs = v.inputs.size();
    tmp.outputs = v.outputs.size();
    tmp.ports = v.ports.size();
    tmp.wires = v.wires.size();
    tmp.functions = v.functions.size();
    
    for(auto ir : v.functions) tmp.qtd[ir.op]++;
  
    for(j = strlen(argv[i]); argv[i][j] != '/'; j--);
    tmp.name = argv[i];
    tmp.name = tmp.name.substr(j+1);
    verilogFinal.push_back(tmp);
  }
  
  FILE * pFile;
  pFile = fopen("benchmarksTable.markdown","w");
  if(pFile != NULL){
    fputs("##BENCHMARK'S TABLE\n",pFile);
    fputs("|TEST|GATES|INPUTS|OUTPUTS|FUNCTION|AND|NAND|OR|NOR|XOR|XNOR|NOT|BUF|\n",pFile);
    fputs("|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|\n",pFile);
    
    for(auto iv : verilogFinal){
      fprintf(pFile,"|%s|",iv.name.c_str());
      fprintf(pFile,"%d|",iv.ports);
      fprintf(pFile,"%d|",iv.inputs);
      fprintf(pFile,"%d|",iv.outputs);
      fprintf(pFile,"%d|",iv.functions);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::And],(iv.qtd[Opcode::And]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::Nand],(iv.qtd[Opcode::Nand]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::Or],(iv.qtd[Opcode::Or]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::Nor],(iv.qtd[Opcode::Nor]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::Xor],(iv.qtd[Opcode::Xor]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::Xnor],(iv.qtd[Opcode::Xnor]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|",iv.qtd[Opcode::Not],(iv.qtd[Opcode::Not]/(double)iv.functions)*100.0);
      fprintf(pFile,"%d(%.1lf%%)|\n",iv.qtd[Opcode::Buf],(iv.qtd[Opcode::Buf]/(double)iv.functions)*100.0);
    }
  
    fclose(pFile);
  }

  return 0;

}
