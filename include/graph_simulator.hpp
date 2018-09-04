#pragma once

#include "data_model.hpp"
#include "graph_model.hpp"
#include "graph_algorithm.hpp"

#include <vector>
#include <map>
#include <iostream>
#include <deque>
#include <sstream>
#include <stdexcept>
#include <iterator>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>


namespace verilog {
  namespace graph {

    LogicValue propagate(LogicValue v, NegP np) {
      checkValid(v);

      return np == NegP::Positive ? v : !v;
    }


    void simulate(
        std::map<std::string, bool> & inputs,
        std::map<std::string, bool> & outputs, G_builder & b) {

      G & g = b.g;

      // using namespace boost;
      //
      // std::deque<int> topo_order;
      // boost::topological_sort(g.graph, std::front_inserter(topo_order));
      //
      std::deque<int> topo_order = topological_sort(g);

      // Initialize the constants
      g.graph[g.zero].value = LogicValue::Zero;
      g.graph[g.one].value = LogicValue::One;

      //std::cout << "// initialized constants\n";
      //write_graph(std::cout, b);

      // Initialize the inputs
      for (auto it : inputs) {
        g.graph[b.get_vertex(it.first)].value = fromBool(it.second);
      }


      //std::cout << "// initialized inputs\n";
      //write_graph(std::cout, b);

      // Propagate the values to the outputs
      for (int node : topo_order) {
        if(in_degree(node, g.graph) > 0) {
          auto p = in_edges(node, g.graph);

          LogicValue sum = LogicValue::One;

          for (auto e = p.first; e != p.second; ++e) {

            int s = source(*e, g.graph);
            LogicValue value = propagate(g.graph[s].value, g.graph[*e]);

            sum = sum && value;
          }
          g.graph[node].value = sum;

        }

        //std::cout << "// executed " << node << "\n";
        //write_graph(std::cout, b);
      }

      // output the outputs!
      for(std::string n : b.outputs) {
        outputs[n] = toBool(g.graph[b.get_vertex(n)].value);
      }
    }
  }
}
