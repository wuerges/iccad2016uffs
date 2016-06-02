#pragma once

#include <graph_model.hpp>
#include <vector>
#include <iostream>
#include <deque>
#include <iterator>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace verilog {
  namespace graph {
    void simulate(std::vector<bool> & inputs, std::vector<bool> & outputs, G & g) {

      std::deque<int> topo_order;
      boost::topological_sort(g.graph, std::front_inserter(topo_order));
      //
      boost::write_graphviz(std::cout, g.graph);

      for (auto it : topo_order) {
        std::cout << it << '\n';
      }
    }
  }
}
