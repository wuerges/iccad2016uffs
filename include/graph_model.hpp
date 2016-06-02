
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
    enum class NegP { 
      Positive, Negative   
    };

    typedef boost::adjacency_list<
      boost::setS, 
      boost::vecS, 
      boost::bidirectionalS, 
      boost::no_property, NegP> GD;

    struct G {
      std::map<const std::string, int> name_map;

      GD graph;

      int add_vertex(std::string name) {
        int v = new_vertex();
        name_map[name] = v;
        return v;
      };

      int new_vertex() {
        return boost::add_vertex(graph);
      };

      int get_vertex(std::string name) {
        auto it = name_map.find(name);
        if (it != name_map.end())
          return it->second;

        std::stringstream ss;
        ss << "Name not found: " << name;
        throw std::invalid_argument(ss.str());
      };

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

