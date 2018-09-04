#pragma once

#include "bdd_model.hpp"
#include "graph_algorithm.hpp"
#include "graph_model.hpp"
#include <vector>

namespace verilog
{

//using namespace bdd;
//using namespace graph;

void build(bdd::BDD & bdds, graph::G & g) {
  auto topo_order = topological_sort(g);

  std::vector<bdd::Node*> bdd_nodes(num_vertices(g.graph));

  for (int node : topo_order) {
    if(in_degree(node, g.graph) == 0) {
      bdd_nodes[node] = bdds.add_simple_input(g.graph[node].identifier);
    }
    else if(node == g.zero) {
      bdd_nodes[node] = bdds.zero;
    }
    else if(node == g.one) {
      bdd_nodes[node] = bdds.one;
    }
    else {
      auto p = in_edges(node, g.graph);
      bdd::Node * sum = bdds.one;

      for (auto e = p.first; e != p.second; ++e) {
        bdd::Node * s = bdd_nodes[source(*e, g.graph)];

        bdd::Node * s2 = g.graph[*e] == NegP::Positive ? s : bdds.negate(s);

        sum = bdds.land(sum, s);

        // return np == NegP::Positive ? v : !v;
        // LogicValue value = propagate(g.graph[s].value, g.graph[*e]);

      }

      bdd_nodes[node] = sum;
    }
  }
}

}
