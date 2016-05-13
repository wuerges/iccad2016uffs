
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace verilog 
{
  namespace graph {
    struct NegP { 
      bool neg;
    };

    typedef boost::adjacency_list<
      boost::setS, 
      boost::vecS, 
      boost::bidirectionalS, 
      boost::no_property, NegP> GD;

    struct G {
      std::map<const std::string, int> name_map;

      int get_name(const std::string & s) {
        auto it = name_map.find(s);
        if (it != name_map.end())
          return it->second;
        else {
          int ret = ++name_seed;
          name_map[s] = ret;
          return ret;
        }
      }
        private:
        int name_seed;
    };
  }
}

