#pragma once

#include <graph_model.hpp>
#include <vector>
#include <iostream>
#include <deque>
#include <sstream>
#include <stdexcept>
#include <iterator>

// #include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace verilog {
  namespace graph {


    LogicValue negate(LogicValue v) {
      return (v == LogicValue::True) ? LogicValue::False : LogicValue::True;
    }

    void checkValid(LogicValue v) {
      switch(v) {
        case LogicValue::True:
        case LogicValue::False:
          return;
        default:
          std::stringstream ss;
          ss << "Error: Logic value <>";
          throw std::invalid_argument(ss.str());
      }
    }

    LogicValue propagate(LogicValue v, graph::NegP np) {
      checkValid(v);

      return np == NegP::Positive ? v : negate(v);
    }

    LogicValue lv_and(LogicValue v1, LogicValue v2) {
      checkValid(v1);
      checkValid(v2);
      if (v1 == LogicValue::False)
        return v1;
      return v2;
    }

    LogicValue initValue(bool b) {
      return b ? LogicValue::True : LogicValue::False;
    }

    bool toBool(LogicValue v) {
      checkValid(v);
      return v == LogicValue::True;
    }

    void simulate(std::vector<bool> & inputs, std::vector<bool> & outputs, G & g) {

      using namespace boost;

      std::deque<int> topo_order;
      boost::topological_sort(g.graph, std::front_inserter(topo_order));
      //
      //boost::write_graphviz(std::cout, g.graph);

      // Initialize the inputs
      for (int i = 0; i < g.inputs.size(); ++ i) {
        g.graph[g.name_map[g.inputs[i]]] = initValue(inputs[i]);
      }
      // Initialize the constants 
      g.graph[g.name_map["1'b0"]] = LogicValue::False;
      g.graph[g.name_map["1'b1"]] = LogicValue::True;

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
            LogicValue value = propagate(g.graph[node], g.graph[*e]);

            g.graph[t] = lv_and(g.graph[t], value);
          }

        }
      }

      // output the outputs!
      for(std::string n : g.outputs) {
        outputs.push_back(toBool(g.graph[g.name_map[n]]));
      }
    }
  }
}
