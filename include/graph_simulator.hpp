#pragma once

#include <graph_model.hpp>
#include <vector>
#include <iostream>
#include <deque>
#include <iterator>

// #include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace verilog {
  namespace graph {


    bool propagate(bool b, graph::NegP np) {
      if (np == graph::NegP::Positive) {
        return b;
      }
      else {
        return !b;
      }
    }

    void simulate(std::vector<bool> & inputs, std::vector<bool> & outputs, G & g) {

      using namespace boost;

      std::deque<int> topo_order;
      boost::topological_sort(g.graph, std::front_inserter(topo_order));
      //
      //boost::write_graphviz(std::cout, g.graph);

      // Initialize the inputs
      for (int i = 0; i < g.inputs.size(); ++ i) {
        g.graph[g.name_map[g.inputs[i]]] = inputs[i];
      }

      // Propagate the values to the outputs
      for (int node : topo_order) {
        if(out_degree(node, g.graph) > 0) {
          auto p = out_edges(node, g.graph);

          // First target
          auto e1 = *p.first;
          int t1 = target(e1, g.graph);

          g.graph[t1] = propagate(g.graph[node], g.graph[e1]);

          for (auto e = ++p.first; e != p.second; ++e) {
            int t = target(*e, g.graph);
            bool value = propagate(g.graph[node], g.graph[*e]);

            g.graph[t] &= value;
          }

        }
      }

      // output the outputs!
      for(std::string n : g.outputs) {
        outputs.push_back(g.graph[g.name_map[n]]);
      }
    }
  }
}
