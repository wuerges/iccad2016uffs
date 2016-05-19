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

using namespace verilog;
using namespace ast;
using namespace boost;
using namespace spirit;
using namespace parser;

struct verilogProperties{
  char name[200];
  int inputs, outputs, ports, wires, functions;
  int qtdAnd, qtdNand, qtdOr, qtdNor, qtdXor, qtdXnor, qtdNot, qtdBuf;

  verilogProperties(): inputs(0), outputs(0), ports(0), wires(0), functions(0),
    qtdAnd(0), qtdNand(0), qtdOr(0), qtdNor(0), qtdXor(0), qtdXnor(0), qtdNot(0),
    qtdBuf(0){}
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
    std::vector<Function>::iterator ir;
    std::cout << v.ports.size() << "\n"; 
    tmp.inputs = v.inputs.size();
    tmp.outputs = v.outputs.size();
    tmp.ports = v.ports.size();
    tmp.wires = v.wires.size();
    tmp.functions = v.functions.size();
    
    for(ir = v.functions.begin(); ir != v.functions.end(); ir++){
      if(ir->op == "and") tmp.qtdAnd++;
      else if(ir->op == "nand") tmp.qtdNand++;
      else if(ir->op == "or") tmp.qtdOr++;
      else if(ir->op == "nor") tmp.qtdNor++;
      else if(ir->op == "xor") tmp.qtdXor++;
      else if(ir->op == "xnor") tmp.qtdXnor++;
      else if(ir->op == "not") tmp.qtdNot++;
      else if(ir->op == "buf") tmp.qtdBuf++;
    }
    
    for(j = strlen(argv[i]); argv[i][j] != '/'; j--);
    strcpy(tmp.name,&argv[i][j+1]);
    
    verilogFinal.push_back(tmp);
  }
  
  FILE * pFile;
  pFile = fopen("benchmarksTable.markdown","w");
  if(pFile != NULL){
    fputs("##BENCHMARK'S TABLE\n",pFile);
    fputs("|TEST|GATES|INPUTS|OUTPUTS|FUNCTION|AND|NAND|OR|NOR|XOR|XNOR|NOT|BUF|\n",pFile);
    fputs("|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|\n",pFile);
    std::vector<verilogProperties>::iterator iv;
    
    for(iv = verilogFinal.begin(); iv != verilogFinal.end(); iv++){
      fprintf(pFile,"|%s|",iv->name);
      fprintf(pFile,"%d|",iv->ports);
      fprintf(pFile,"%d|",iv->inputs);
      fprintf(pFile,"%d|",iv->outputs);
      fprintf(pFile,"%d|",iv->functions);
      fprintf(pFile,"%d(%.1lf%%)|",iv->qtdAnd,((100.0/iv->functions)*iv->qtdAnd));
      fprintf(pFile,"%d(%.1lf%%)|",iv->qtdNand,((iv->functions/100.0)*iv->qtdNand));
      fprintf(pFile,"%d(%.1lf%%)|",iv->qtdOr,((iv->functions/100.0)*iv->qtdOr));
      fprintf(pFile,"%d(%.1lf%%)|",iv->qtdNor,((iv->functions/100.0)*iv->qtdNor));
      fprintf(pFile,"%d(%.1lf%%)|",iv->qtdXor,((iv->functions/100.0)*iv->qtdXor));
      fprintf(pFile,"%d(%.1lf%%)|",iv->qtdNot,((iv->functions/100.0)*iv->qtdNot));
      fprintf(pFile,"%d(%.1lf%%)|\n",iv->qtdBuf,((iv->functions/100.0)*iv->qtdBuf));
    }
  
    fclose(pFile);
  }

  return 0;

}
