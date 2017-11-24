#pragma once

#include <graph_model.hpp>
#include <bdd_model.hpp>
#include <nand_bdd_adapter.hpp>
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <memory>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace verilog {

  using namespace graph;
  using namespace bdd;
  using std::shared_ptr;
  using std::vector;
  using std::map;
  using std::pair;

  void merge_nodes(Node_p & from, Node_p & to, G & g, BDD & b) {
    std::cerr << "Merging nodes\n";
    if(from->r_t >= 0) {
      if(to->r_t >= 0) {
        b.index[from->r_t] = to;
        auto p = out_edges(from->r_t, g.graph);
        for (auto e = p.first; e != p.second; ++e) {
          int t = source(*e, g.graph);
          add_edge(to->r_t, t, g.graph[*e], g.graph);
        }
        clear_out_edges(from->r_t, g.graph);
      }
      else {
        to->r_t = from->r_t;
      }
    }
  }

  void minimize1case3(BDD & b, G & g, Node_p node) {
    if(node->p && node->p->p
        && node->p->p == node->p->n) {
      auto aux = node->p;
      node->p = node->p->p;
      merge_nodes(aux, node->p, g, b);
    }
    if(node->n && node->n->n
        && node->n->p == node->n->n) {
      auto aux = node->n;
      node->n = node->n->p;
      merge_nodes(aux, node->n, g, b);
    }
  }

  void minimize(BDD & b, G & g) {
    using namespace boost;
    std::deque<int> topo_order;
    boost::topological_sort(g.graph, std::front_inserter(topo_order));

    //vector<shared_ptr<bdd::Node>> index(num_vertices(g.graph));
    layers_t layers;
    get_layers(b, layers);

    for(auto [key, layer] : layers) {

      for(auto node : layer) {
        // Fix for case 2:
        minimize1case3(b, g, node);
      }


      /*
      for(auto node : layer) {
        if(node->p == node->n) {
          // Case 1 should remove this node and merge to the parent.
          auto aux = node->p;
          node->p = aux->p;
          node->n = aux->n;
          merge_nodes(aux, node, g, b);
        }
      }
      */

      typedef pair<Node_p, Node_p> NN;
      map<NN, vector<Node_p>> merging;
      for(auto node : layer) {
        if(node->s > 1) // Do not atempt to merge 0 and 1
          merging[NN(node->p, node->n)].push_back(node);
      }

      // Case 2, where both negative and positive edges are equal
      for(auto & [k, nodes] : merging) {
        if (nodes.size() > 1) {
          std::cerr << "Case 2 reached " 
            << k.first 
            << ","<< k.second << "\n";
          for(int i = 1; i < nodes.size(); ++i) {
            nodes[i]->p = nodes[0];
            nodes[i]->n = nodes[0];
          }
        }
      }
    }
    
    for(auto & node : b.index) {
      if(node->p && node->p->p
          && node->p == node->n) {
        std::cerr << "Case X reached \n" ;
        merge_nodes(node, node->p, g, b);
        node = node->p;
      }
    }

  }

};
