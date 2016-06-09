
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace verilog 
{
  namespace graph {
    enum class LogicValue { 
      Zero, True, False, Invalid
    };

    enum class NegP { 
      Positive, Negative   
    };

    typedef boost::adjacency_list<
      boost::setS, 
      boost::vecS, 
      boost::bidirectionalS, 
      LogicValue, NegP> GD;

    struct G {
      std::map<const std::string, int> name_map;
      std::vector<std::string> inputs, outputs;

      GD graph;

      int add_vertex(std::string name) {
        int v = new_vertex();
        name_map[name] = v;
        return v;
      }


      std::string new_named_vertex() {
        int v = new_vertex();
        std::stringstream ss;
        ss << "extra_vertex_" << v;
        name_map[ss.str()] = v;
        return ss.str();
      }

      int new_vertex() {
        return boost::add_vertex(graph);
      }
  
      int get_vertex(std::string name) {
        auto it = name_map.find(name);
        if (it != name_map.end()) {
          return it->second;
        }
        else {
          return add_vertex(name);
        }
      }

      void add_edge(std::string name1, std::string name2, NegP p) {
        int v1 = get_vertex(name1);
        int v2 = get_vertex(name2);
        add_edge(v1, v2, p);
      }

      void add_edge(int a, int b, NegP p) {
        boost::add_edge(a, b, p, graph);
      }

    };
  }
}

