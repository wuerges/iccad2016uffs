#pragma once

#include <verilog_model.hpp>
#include <vector>
#include <map>
#include <queue>
#include <string>
#include <string.h>
#include <verilog_parser.hpp>

namespace verilog {
  namespace ast {

    struct node{
      int type, dIn, bkpDin;
      bool value;
      Opcode typeFunction;
      std::vector<int> adj;
    };
    
    node createNewNode(std::string is,int type, int &totalNodes,std::map<std::string,int> &indexNodes){
      node tmp;
      indexNodes[is] = totalNodes++;
      tmp.dIn  = tmp.bkpDin = 0;
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
	  tmp.value = tmp.dIn = tmp.bkpDin = tmp.type = 1;
	  nodes[indexNodes[it.parameters[j-1]]].adj.push_back(totalNodes);
      
	  for(int i = 0; i < (j-1); i++){
	    tmp.adj.push_back(indexNodes[it.parameters[i]]);
	    nodes[indexNodes[it.parameters[i]]].dIn++;
	  }
	  nodes.push_back(tmp);
	  totalNodes++;
	}
	else{
	  int index = indexNodes[it.parameters[0]];
	  nodes[index].type = 1;
	  nodes[index].typeFunction = it.op;
	  nodes[index].value = (it.op == Opcode::And || it.op == Opcode::Nand);
      
	  for(auto iv: it.parameters){
	    if(it.parameters[0] != iv){	  
	      nodes[indexNodes[iv]].adj.push_back(index);
	      nodes[index].dIn = ++nodes[index].bkpDin;   
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
    
    void simulate(std::vector<bool> &inputs, std::vector<bool> &outputs, Verilog &v) {
      int totalNodes = 0, i = 0;
         
      std::map<std::string,int> indexNodes;
      std::vector<node> nodes;
      
      addLiteralNode(indexNodes,nodes,totalNodes,"1'b0");
      addLiteralNode(indexNodes,nodes,totalNodes,"1'b1");
      addNodes(indexNodes,nodes,v.inputs,totalNodes,0);
      addNodes(indexNodes,nodes,v.outputs,totalNodes,1);
      addNodes(indexNodes,nodes,v.wires,totalNodes,3); 
      addFunctionsTypeNodes(indexNodes,nodes,v,totalNodes); 
      
      for(auto it : v.inputs)   
	nodes[indexNodes[it]].value = inputs[i++];
       
      booleanTest(indexNodes,nodes);
      
      i = 0;
      
      for(auto it : v.outputs)
        outputs.push_back(nodes[indexNodes[it]].value);
    }
  }
}
