#pragma once

#include "graph_model.hpp"

//#include <iostream>
//#include <string>
//#include <vector>
//#include <stdexcept>
//#include <map>
//#include <sstream>
//#include <boost/graph/graphviz.hpp>
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
//#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <boost/graph/copy.hpp>

namespace verilog
{
namespace graph {

std::deque<int> topological_sort(G & g) {
  std::deque<int> topo_order;
  boost::topological_sort(g.graph, std::front_inserter(topo_order));
  return topo_order;
}

}
}
