
#pragma once

#include <data_model.hpp>
#include <graph_model.hpp>

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

  bool equivalence_check(const graph::G & g1, const graph::G & g2) {
    return true;
  }
}

