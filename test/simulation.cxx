#include <verilog_parser.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/home/x3.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <map>
#include <queue>

using namespace verilog;
using namespace ast;
using namespace boost;
using namespace spirit;
using namespace parser;

struct node{
  bool value, checked;
  std::string name;
  Opcode type;
  
  std::vector<std::string> parameters;
};

node createNewNode(std::string is, int &totalNodes,std::map<std::string,int> &indexNodes, bool checked, Opcode type, bool value){
  node tmp;
  indexNodes[is] = totalNodes;
  tmp.checked = checked;
  tmp.name = is;
  tmp.type = type;
  tmp.value = value;
  totalNodes++;
  return tmp;   
}

void addNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, std::vector<std::string> &listNodes, int &totalNodes, bool checked, Opcode type, bool value){
  for(auto is : listNodes)
    nodes.push_back(createNewNode(is,totalNodes,indexNodes,checked,type,value));
}
    
void addLiteralNode(std::map<std::string,int> &indexNodes, std::vector<node> &nodes,int &totalNodes, std::string typeLiteral){
  bool value = (typeLiteral == "1'b1")? true : false;
  node tmp = createNewNode(typeLiteral,totalNodes,indexNodes,true,Opcode::Buf,value);
  nodes.push_back(tmp); 
}
    
void addFunctionsTypeNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
  for(auto it : v.functions) {
    if(it.op == Opcode::Not || it.op == Opcode::Buf){
      int index, size = it.parameters.size()-1; 
      node tmp ;
	  
      for(int i = 0; i < size; i++){
	if(!indexNodes[it.parameters[i]]){
	  tmp = createNewNode(it.parameters[i],totalNodes,indexNodes,false,it.op,false);
	  tmp.parameters.push_back(it.parameters[size]);
	  nodes.push_back(tmp);
	}
	else{
	  index = indexNodes[it.parameters[i]];
	  nodes[index].parameters.push_back(it.parameters[size]);
	  nodes[index].checked = false;
	}
      }   
    }
    else{
      node tmp ;
      bool value = (it.op == Opcode::And || it.op == Opcode::Nand);
	  
      tmp = createNewNode(it.parameters[0],totalNodes,indexNodes,false,it.op,value);
      tmp.parameters = it.parameters; 
      nodes.push_back(tmp);
    }
  } 
}

bool booleanTest(int u, std::map<std::string,int> &indexNodes, std::vector<node> &nodes){
  bool value, flag = 0;
  int index, i;
  
  nodes[u].checked = true;
  
  if(!(nodes[u].type == Opcode::Not || nodes[u].type == Opcode::Buf)){
    for(i = 1; i < nodes[u].parameters.size() && !flag; i++){
      index = indexNodes[nodes[u].parameters[i]];
      
      value = (nodes[index].checked)? nodes[index].value: booleanTest(index,indexNodes,nodes);
      
      switch(nodes[u].type){
         case Opcode::And: case Opcode::Nand:
	   nodes[u].value &= value;
	   if(!nodes[u].value) flag = true;
	   break;
         case Opcode::Or: case Opcode::Nor:
	   nodes[u].value =(nodes[u].value || value);
	   //std::cout << std::boolalpha << value<<"\\\\" << nodes[index].name << "\n";
	   if(nodes[u].value) flag = true;	    
	   break;
         case Opcode::Xor: case Opcode::Xnor:
	   nodes[u].value ^= value;
	   break;
      }
    }
    if(nodes[u].type==Opcode::Nand || nodes[u].type==Opcode::Nor || nodes[u].type==Opcode::Xnor)
	nodes[u].value = !nodes[u].value;
  }
  else{
    index = indexNodes[nodes[u].parameters[0]];
    value = (nodes[index].checked)? nodes[index].value : booleanTest(index,indexNodes,nodes);
    
    if(nodes[u].type == Opcode::Not)
      value = !value;
    
    nodes[u].value = value;
  }
  
  return nodes[u].value;
}

int main(int nargs, char** argv){
  int totalNodes = 0, i = 0;
  bool entradas[]= {0,0,0};
  Verilog v;
  
  std::map<std::string,int> indexNodes;
  std::vector<node> nodes;
  
  std::ifstream input(argv[1]);
  input.unsetf(std::ios::skipws);
  istream_iterator begin(input);
  istream_iterator end;
  parse_verilog(v, begin, end);
  
  addLiteralNode(indexNodes,nodes,totalNodes,"1'b0");
  addLiteralNode(indexNodes,nodes,totalNodes,"1'b1");
  addNodes(indexNodes,nodes,v.inputs,totalNodes,true, Opcode::Buf,false);
  addNodes(indexNodes,nodes,v.outputs,totalNodes,true, Opcode::Buf,false);
  addFunctionsTypeNodes(indexNodes,nodes,v,totalNodes); 
  
  for(auto it : v.inputs)
    nodes[indexNodes[it]].value = entradas[i++];

  for(auto it: nodes){
    std::cout << "Porta("<<it.type<<"): "+it.name+"------Valor: "<< std::boolalpha << it.value << "\n";
    for(auto is : it.parameters)
      std::cout << "      " << is <<"\n";
  }
  
  for(int i = 2; i < nodes.size(); i++){
    if(!nodes[i].checked)
      booleanTest(i,indexNodes,nodes);   
  }

  for(auto it : v.outputs)
    std::cout << std::boolalpha << nodes[indexNodes[it]].value <<"\n";
 
 
  std::cout  << "---------------------------\n";
  
  for(auto it: nodes){
    std::cout << "Porta("<<it.type<<"): "+it.name+"------Valor: "<< std::boolalpha << it.value << "\n";
    for(auto is : it.parameters)
      std::cout << "      " << is <<"\n";  
      }
  return 1;
}
