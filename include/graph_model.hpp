
#pragma once

#include <data_model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace verilog 
{
  namespace graph {


    struct Node {
      LogicValue value = LogicValue::Undefined;
      std::string identifier;
      Node(const std::string & i): identifier(i) {}
      Node(): identifier("<extra>") {}
    };

    typedef boost::adjacency_list<
      boost::setS, 
      boost::vecS, 
      boost::bidirectionalS, 
      Node, NegP> GD;

    struct G {
      GD::vertex_descriptor zero, one;
      GD graph;

      G() {
        zero = add_vertex("1'b0");
        one  = add_vertex("1'b1");
      }

      int add_vertex(const std::string & name) {
        return boost::add_vertex(Node(name), graph);
      }

      int new_vertex() {
        return boost::add_vertex(graph);
      }
  
      void add_edge(int a, int b, NegP p) {
        boost::add_edge(a, b, p, graph);
      }

    };
    
    struct G_builder  {
      G g;
      std::map<const std::string, int> name_map;
      std::vector<std::string> inputs, outputs;

      int get_vertex(std::string name) {
        auto it = name_map.find(name);
        if (it != name_map.end()) {
          return it->second;
        }
        else {
          int x = g.add_vertex(name);
          name_map[name] = x;
          return x;
        }
      }

      /*
      std::string new_named_vertex() {
        int v = g.new_vertex();
        std::stringstream ss;
        ss << "extra_vertex_" << v;
        name_map[ss.str()] = v;
        return ss.str();
      }
      */

      void add_edge(int v1, std::string name2, NegP p) {
        int v2 = get_vertex(name2);
        g.add_edge(v1, v2, p);
      }
      
      void add_edge(std::string name1, std::string name2, NegP p) {
        int v1 = get_vertex(name1);
        int v2 = get_vertex(name2);
        g.add_edge(v1, v2, p);
      }
      
      void add_edge(std::string name1, int v2, NegP p) {
        int v1 = get_vertex(name1);
        g.add_edge(v1, v2, p);
      }

      void add_edge(int v1, int v2, NegP p) {
        g.add_edge(v1, v2, p);
      }

      void add_vertex(std::string name) {
        get_vertex(name);
      }

    };

    void write_graph(std::ostream & o, G_builder & b) {
      boost::write_graphviz(std::cout, b.g.graph
          , [&](std::ostream& o, const GD::vertex_descriptor& v) 
          { o << "[label=" 
              << b.g.graph[v].identifier 
              << ": " 
              << b.g.graph[v].value<< "]";}
          , [&](std::ostream& o, const GD::edge_descriptor& e) 
          { if (b.g.graph[e] == NegP::Negative)
              o << "[style=dotted]"; }
          );
    }
  }
}

