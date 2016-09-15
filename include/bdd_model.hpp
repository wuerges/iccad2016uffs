
#pragma once

#include <data_model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace verilog 
{
  namespace bdd {

    enum class NegP { 
      Positive, Negative   
    };

    enum class Type {
      Zero, One, Input
    };

    struct Node {
      std::string input_name;
      Type t;
    };

    typedef boost::adjacency_list<
      boost::setS, 
      boost::vecS, 
      boost::bidirectionalS, 
      Node, NegP> GD;

    typedef unsigned int hashcode;

    struct G {
      GD graph;
      int zero, one;
      
      std::map<hashcode, std::vector<int> > bdd_heap;

      G() { 
        zero = boost::add_vertex(graph);
        one  = boost::add_vertex(graph);
        graph[zero].t = Type::Zero;
        graph[one].t  = Type::One;
      }

      int add_vertex(const std::string name) {
        int v = boost::add_vertex(graph);
        graph[v].input_name = name;
        graph[v].t          = Type::Input;
        return v;
      }

      void add_edge(int a, int b, NegP p) {
        boost::add_edge(a, b, p, graph);
      }

      int add_elementar_node(const std::string name) {
          int v = add_vertex(name);
          add_edge(v, zero, NegP::Negative);
          add_edge(v, one,  NegP::Positive);
      }

      // Todo task implement revert nodes
      // Precisa uma copia de todos os nodos alcancaveis a partir de n
      // E apenas inverter o Zero e o One para as folhas.
      int reverse_node(int n) {
          
      }

      // Analise de como implementar
      int conjuntion_node(int a, int b) {
        
      }
     
      // Criar um hash o mais unico possivel para um bdd
      hashcode hash(int v) {
      }

      // search BDD
      // TODO
      int search(int v) {
      }

      // minimizes the BDD from the vertex v
      // TODO
      void minimize(int v) {
      }
    };
  }
}

