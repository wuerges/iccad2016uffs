//A funcao "addFunctionsTypeNodes" eh a funcao que esta tomando mais tempo de execucao, isto esta ocorrendo pelo fato de que a funcao, fica alocando dinamicamente os parametros do ADJ. Porem dah forma como a simulacao funciona atualmente, nao tem muito como fugir deste problema. Uma solucao viavel seria a implementacao de uma nova funcao de simulacao, totalmente repensada, nao sendo mais necessario fazer uma toposort+bfs, como estava sendo feito. A ideia desta simulacao seria montar o grafo ao contrario, e chamar, e fazer a simulacao tbm ao contrario, porem da forma bottom-up, sendo assim, a simulacao nao seria mais chamada a partir das entradas, e sim da saida.
//A funcionamento da nova simulacao seria, dependo de cada tipo de porta, chamar uma recursao que faria a mesma coisa para todos os seus parametros, e assim que a recursao voltar, teria o valor da porta calculado.
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
  int type, dIn;
  bool value;
  Opcode typeFunction;
  std::vector<int> adj;
};

node createNewNode(std::string is, int type, int &totalNodes,std::map<std::string,int> &indexNodes){
  node tmp;
  indexNodes[is] = totalNodes++;
  tmp.dIn = 0;
  tmp.type = type;
  tmp.typeFunction = Opcode::Buf;
  tmp.value = false;

  return tmp;
}

void addNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, std::vector<std::string> listNodes, int &totalNodes, int type){
  for(auto is : listNodes)
    nodes.push_back(createNewNode(is,type,totalNodes,indexNodes));
}

void addLiteralNode(std::map<std::string,int> &indexNodes, std::vector<node> &nodes,int &totalNodes, std::string typeLiteral){
  bool value;
  
  if(typeLiteral == "1'b1") value = true;
  else value = false;

  node tmp = createNewNode(typeLiteral,0,totalNodes,indexNodes); 
  tmp.value = value;
  nodes.push_back(tmp); 
}

void addFunctionsTypeNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
  for(auto it : v.functions) {
    if(it.op == Opcode::Not || it.op == Opcode::Buf){
      node tmp;
      int j = (int)it.parameters.size();
      
      tmp.typeFunction = it.op;
      tmp.value = tmp.dIn = tmp.type = 1;
      nodes[indexNodes[it.parameters[j-1]]].adj.push_back(totalNodes);
      
      for(int i = 0; i < (j-1); i++){
	tmp.adj.push_back(indexNodes[it.parameters[i]]);
	nodes[indexNodes[it.parameters[i]]].dIn++;
      }
      nodes.push_back(tmp);
      totalNodes++;
    }
    else{
      int index = indexNodes[it.parameters[0]], i = 0;
      nodes[index].type = 1;
      nodes[index].typeFunction = it.op;
      nodes[index].value = (it.op == Opcode::And || it.op == Opcode::Nand);
        
      for(auto iv: it.parameters){
	if(it.parameters[0] != iv){	  
	  nodes[indexNodes[iv]].adj.push_back(index);
	  nodes[index].dIn++;   
	}
      }
    }
  } 
}  

void booleanTest(std::map<std::string,int> &indexNodes, std::vector<node> &nodes){
  std::queue<int> searchOrder;

  for(int it = 0, i = 0; it < nodes.size(); it++, i++)
    if(!nodes[it].type) searchOrder.push(it);

  while(!searchOrder.empty()){
    int u = searchOrder.front();
    searchOrder.pop();
    
    for(auto v : nodes[u].adj){
      if(nodes[v].dIn){
	nodes[v].dIn--;
	switch(nodes[v].typeFunction){
	   case Opcode::And:
	     nodes[v].value &= nodes[u].value;
	     if(!nodes[v].value) nodes[v].dIn = 0;
	     break;
	   case Opcode::Nand:
	     nodes[v].value &= nodes[u].value;
	     if(!nodes[v].dIn) nodes[v].value = !nodes[v].value;;
	     break;
	   case Opcode::Or:
	     nodes[v].value |= nodes[u].value;
	     if(nodes[v].value) nodes[v].dIn = 0;
	     break;
	   case Opcode::Nor:
	     nodes[v].value |= nodes[u].value;
	     if(!nodes[v].dIn) nodes[v].value = !nodes[v].value;
	     break;
	   case Opcode::Xor:
	     nodes[v].value ^= nodes[u].value;
	     break;
	   case Opcode::Xnor:
	     nodes[v].value ^= nodes[u].value;
	     if (!nodes[v].dIn) nodes[v].value = !nodes[v].value;
	     break;
	   case Opcode::Buf:
	     nodes[v].value = nodes[u].value;
	     break;
	   case Opcode::Not:
	     nodes[v].value = !nodes[u].value;
	     break;
	}
	if (!nodes[v].dIn)  searchOrder.push(v);
      }
    }
  }
}

int main(int nargs, char** argv){
  int totalNodes = 0, i = 0;
  bool entradas[]= {0,1,1,1};
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
  addNodes(indexNodes,nodes,v.inputs,totalNodes,0);
  addNodes(indexNodes,nodes,v.outputs,totalNodes,1);
  addNodes(indexNodes,nodes,v.wires,totalNodes,3); 
  addFunctionsTypeNodes(indexNodes,nodes,v,totalNodes); 

  for(auto it : v.inputs)
    nodes[indexNodes[it]].value = entradas[i++];
  
  booleanTest(indexNodes,nodes);
  
  for(auto it : v.outputs)
    std::cout << std::boolalpha << nodes[indexNodes[it]].value <<"\n";

  return 1;
}
