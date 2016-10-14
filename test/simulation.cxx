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

node createNewNode(std::string is,int &totalNodes,std::map<std::string,int> &indexNodes){
  node tmp;
  indexNodes[is] = totalNodes;
  tmp.checked = true;
  tmp.type = Opcode::Buf;
  tmp.name = is;
  tmp.value = false;
  totalNodes++;
  return tmp;
}

void addNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, std::vector<std::string> &listNodes, int &totalNodes){
  for(auto is : listNodes)
    nodes.push_back(createNewNode(is,totalNodes,indexNodes));
}

void addLiteralNode(std::map<std::string,int> &indexNodes, std::vector<node> &nodes,int &totalNodes, std::string typeLiteral){
  node tmp = createNewNode(typeLiteral,totalNodes,indexNodes);
  tmp.value = (typeLiteral == "1'b1")? true : false;
  nodes.push_back(tmp); 
}

void addFunctionsTypeNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
  for(auto it : v.functions) {
    if(it.op == Opcode::Not || it.op == Opcode::Buf){
      node tmp;
      
      tmp.type = it.op;
      tmp.checked = false;
      tmp.parameters = it.parameters;
      tmp.checked = false;
      nodes.push_back(tmp);
      totalNodes++;
    }
    else{
      int index = indexNodes[it.parameters[0]];
      nodes[index].type = it.op;
      nodes[index].checked = false;
      nodes[index].value = (it.op == Opcode::And || it.op == Opcode::Nand);
      nodes[index].parameters = it.parameters;
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
	   if(!nodes[u].value)
	     flag = true;
	   break;
         case Opcode::Or: case Opcode::Nor:
	   nodes[u].value |= value;
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
    index = indexNodes[nodes[u].parameters[nodes[u].parameters.size()-1]];
    value = (nodes[index].checked)? nodes[index].value : booleanTest(index,indexNodes,nodes);

    
    if(nodes[u].type == Opcode::Not)
      value = !value;
    
    for(i = 0; i < (nodes[u].parameters.size()-1); i++){
      nodes[indexNodes[nodes[u].parameters[i]]].value = value;
      nodes[indexNodes[nodes[u].parameters[i]]].checked = true;
    }
  }

  return nodes[u].value;
}

int main(int nargs, char** argv){
  int totalNodes = 0, i = 0;
  bool entradas[]= {0};
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
  addNodes(indexNodes,nodes,v.inputs,totalNodes);
  addNodes(indexNodes,nodes,v.outputs,totalNodes);
  addNodes(indexNodes,nodes,v.wires,totalNodes); 
  addFunctionsTypeNodes(indexNodes,nodes,v,totalNodes); 
  
  for(auto it : v.inputs)
    nodes[indexNodes[it]].value = entradas[i++];
    
  for(int i = 2; i < nodes.size(); i++){
    if(!nodes[i].checked)
       booleanTest(i,indexNodes,nodes);   
  }

  for(auto it : v.outputs)
    std::cout << std::boolalpha << nodes[indexNodes[it]].value <<"\n";

  return 1;
}
