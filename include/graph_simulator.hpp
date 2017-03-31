#pragma once

#include <data_model.hpp>
#include <graph_model.hpp>

#include <vector>
#include <iostream>
#include <deque>
#include <sstream>
#include <stdexcept>
#include <iterator>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace verilog {
  namespace graph {

    LogicValue propagate(LogicValue v, NegP np) {
      checkValid(v);

      return np == NegP::Positive ? v : !v;
    }


    void simulate(std::vector<bool> & inputs, std::vector<bool> & outputs, G & g) {
		
      using namespace boost;
	  
      std::deque<int> topo_order;
      boost::topological_sort(g.graph, std::front_inserter(topo_order));
      //
      //boost::write_graphviz(std::cout, g.graph);

      // Initialize the inputs

      for (int i = 0; i < g.inputs.size(); ++ i) {
        g.graph[g.name_map[g.inputs[i]]] = fromBool(inputs[i]);
      }

      // Initialize the constants 
      g.graph[g.name_map["1'b0"]] = LogicValue::Zero;
      g.graph[g.name_map["1'b1"]] = LogicValue::One;

      // Propagate the values to the outputs
      for (int node : topo_order) {
        if(in_degree(node, g.graph) > 0) {
          auto p = in_edges(node, g.graph);

          LogicValue sum = LogicValue::One;

          for (auto e = p.first; e != p.second; ++e) {

            int s = source(*e, g.graph);
            LogicValue value = propagate(g.graph[s], g.graph[*e]);

            sum = sum && value;
          }
          g.graph[node] = sum;

        }
      }

      // output the outputs!
      for(std::string n : g.outputs) {
        outputs.push_back(toBool(g.graph[g.name_map[n]]));
      }
    }
  }
}
