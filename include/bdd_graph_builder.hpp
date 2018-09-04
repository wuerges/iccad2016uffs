#pragma once

#include "bdd_model.hpp"
#include "graph_algorithm.hpp"
#include "graph_model.hpp"
#include <vector>

namespace verilog
{

//using namespace bdd;
//using namespace graph;

struct BDD_Builder {
  graph::G & g;
  bdd::BDD & bdds;
  std::vector<bdd::Node*> bdd_nodes;


  BDD_Builder(graph::G & g, bdd::BDD & bdds): g(g), bdds(bdds), bdd_nodes(num_vertices(g.graph)) {

  }

  bdd::Node * sum_nodes_group_ands_and_ors(int node) {
    auto p = in_edges(node, g.graph);

    bdd::Node * sum_or = bdds.zero;
    bdd::Node * sum_and = bdds.one;

    for (auto e = p.first; e != p.second; ++e) {
      bdd::Node * s = bdd_nodes[source(*e, g.graph)];

      if(g.graph[*e] == NegP::Positive) {
        sum_and = bdds.land(sum_and, s);
      }
      else {
        sum_or = bdds.lor(sum_or, s);
      }
      //bdd::Node * s2 = g.graph[*e] == NegP::Positive ? s : bdds.negate(s);
    }
    return bdds.land(sum_and, bdds.negate(sum_or));
  }

  bdd::Node * sum_nodes(int node) {
    auto p = in_edges(node, g.graph);
    bdd::Node * sum = bdds.one;

    for (auto e = p.first; e != p.second; ++e) {
      bdd::Node * s = bdd_nodes[source(*e, g.graph)];

      bdd::Node * s2 = g.graph[*e] == NegP::Positive ? s : bdds.negate(s);

      sum = bdds.land(sum, s);
    }
    return sum;
  }

  bdd::Node * sum_nodes_lor(int node) {
    auto p = in_edges(node, g.graph);
    bdd::Node * sum = bdds.zero;

    for (auto e = p.first; e != p.second; ++e) {
      bdd::Node * s = bdd_nodes[source(*e, g.graph)];

      bdd::Node * s2 = g.graph[*e] == NegP::Positive ? bdds.negate(s) : s;

      sum = bdds.lor(sum, s);
    }
    return bdds.negate(sum);
  }

  void build() {
    auto topo_order = topological_sort(g);


    for (int node : topo_order) {
      if(node == g.zero) {
        bdd_nodes[node] = bdds.zero;
      }
      else if(node == g.one) {
        bdd_nodes[node] = bdds.one;
      }
      else if(in_degree(node, g.graph) == 0) {
        printf("// adding input node %d\n", node);
        bdd_nodes[node] = bdds.add_simple_input(g.graph[node].identifier);
      }
      else {

        // TODO check which of these 3 is best.
        //bdd_nodes[node] = sum_nodes_lor(node);
        bdd_nodes[node] = sum_nodes(node);
        //bdd_nodes[node] = sum_nodes_group_ands_and_ors(node);

        if(bdd_nodes[node]->r_t) {
          printf("// replacing representative of node %p: %d -> %d\n", bdd_nodes[node], *bdd_nodes[node]->r_t, node);
        }
        else {
          printf("// adding representative to node %p: %d\n", bdd_nodes[node], node);
        }
        bdd_nodes[node]->r_t = node;
      }
    }
  }

};

}
