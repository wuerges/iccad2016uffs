
#pragma once

#include <data_model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
/*#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graphviz.hpp>
*/

namespace verilog
{
  namespace bdd {
    using std::vector;
    vector<std::string> symbol_table;

    struct Node {
      /**
       * Index of the node's input in the symbol table
       */
      const int s;
      /**
       * marks if this is a representative node.
       */
      const bool r;
      /**
       * The positive edge for this node.
       */
      const int p;
      /**
       * The negative edge for this node.
       */
      const int n;
      Node(const int s_, const int r_, const int p_, const int n_): 
        s(s), r(r), p(p_), n(n_)
      {}
      //Node(const int s_): s(s), r(true), p(1), n(0) {}

    };

    struct BDD {
      vector<Node> graph;
      BDD() { 
        graph.emplace_back(0, true, 0, 1);
        graph.emplace_back(1, true, 1, 0);
      }

      int add_simple_input(int s) {
        graph.emplace_back(s, true, 1, 0);
        return s;
      }

      /*int clone_node(int x) {
        if(x == 0) return 0;
        if(x == 1) return 1;
      }
      */

      int negate(int x) {
        if(x == 0) return 1;
        if(x == 1) return 0;
        
        int p = negate(graph[x].p);
        int n = negate(graph[x].n);
        graph.emplace_back(graph[x].s, false, p, n);
        return graph.size() - 1;
      }

      int conjunction(int x, int y) {
        if(x == 1) return y;
        if(y == 1) return x;
        if(x == 0) return 0;
        if(y == 0) return 0;


      }

    };
  }
}
