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
    
    void addInputsNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
      for(auto is : v.inputs){
	node tmp;
	indexNodes[is] = totalNodes++;
	tmp.dIn = 0;
	tmp.type = 0;
	tmp.bkpDin = 0;
	tmp.typeFunction = Opcode::Buf;
	tmp.value = false;
	nodes.push_back(tmp);
      }
    } 
   
    void addOutputsNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
      for(auto is : v.outputs){
	node tmp;
	indexNodes[is] = totalNodes++;
	tmp.dIn = 0;
	tmp.bkpDin = 0;
	tmp.typeFunction = Opcode::Buf;
	tmp.type = 2;
	tmp.value = false;
	nodes.push_back(tmp);
      }
    }

    void addWiresNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
      for(auto is : v.wires){
	node tmp;
	indexNodes[is] = totalNodes++;
	tmp.type = 0;
	nodes.push_back(tmp);
      }
    }

    void addFunctionsTypeNodes(std::map<std::string,int> &indexNodes, std::vector<node> &nodes, Verilog &v,int &totalNodes){
      for(auto it : v.functions) {
	if(it.op == Opcode::Buf){
	  node tmp;
	  int j = (int)it.parameters.size();
      
	  tmp.typeFunction = Opcode::Buf;
	  tmp.value = tmp.dIn = tmp.bkpDin = -1;
	  tmp.type = -1;
	  nodes[indexNodes[it.parameters[j-1]]].adj.push_back(totalNodes);
      
	  for(int i = 0; i < (j-1); i++)
	    tmp.adj.push_back(indexNodes[it.parameters[i]]);
      
	  nodes.push_back(tmp);
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
	int u;
	u = searchOrder.front();
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
	    case Opcode::Not:
	      nodes[v].value = !nodes[u].value;
	      break;
	    case Opcode::Buf:
	      nodes[v].value = nodes[u].value;
	      nodes[v].dIn = -1;
	      for (int i = 0; i < nodes[v].adj.size(); i++) {
		int iv = nodes[v].adj[i];
		nodes[iv].value = nodes[v].value;	  
	      }
	      break; 
	    }
	    if (!nodes[v].dIn) searchOrder.push(v);
	  }
	}
      }
    }

    void addLiteralNode(std::map<std::string,int> &indexNodes, std::vector<node> &nodes,int &totalNodes, std::string typeLiteral){
      bool value;
  
      if(typeLiteral == "1'b1") value = true;
      else value = false;

      node tmp;
      indexNodes[typeLiteral] = totalNodes++;
      tmp.dIn = 0;
      tmp.type = 0;
      tmp.bkpDin = 0;
      tmp.typeFunction = Opcode::Buf;
      tmp.value = value;
      nodes.push_back(tmp);
    }

    void simulate(std::vector<bool> &inputs, std::vector<bool> &outputs, Verilog &v) {
      int totalNodes = 0, i = 0;
         
      std::map<std::string,int> indexNodes;
      std::vector<node> nodes;
      
      addLiteralNode(indexNodes,nodes,totalNodes,"1'b0");
      addLiteralNode(indexNodes,nodes,totalNodes,"1'b1");
      addInputsNodes(indexNodes,nodes,v,totalNodes);
      addOutputsNodes(indexNodes,nodes,v,totalNodes);
      addWiresNodes(indexNodes,nodes,v,totalNodes); 
      addFunctionsTypeNodes(indexNodes,nodes,v,totalNodes);

      for(auto it : v.inputs)   
	nodes[indexNodes[it]].value = inputs[i++];
       
      booleanTest(indexNodes,nodes);

      i = 0;
      for(auto it : v.outputs)
        outputs[i++] =  nodes[indexNodes[it]].value; 
    }
    

  }
}
