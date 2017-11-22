
#pragma once

#include <data_model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
#include <string>
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
      const int  r_t;
      /**
       * The positive edge for this node.
       */
      const Node * p;
      /**
       * The negative edge for this node.
       */
      const Node * n;
      Node(const int s_, const int r_t_, const Node * p_, const Node * n_): 
        s(s_), r_t(r_t_), p(p_), n(n_)
      {}

    };

    struct BDD {
      //vector<Node> graph;
      const Node * zero;
      const Node * one;
      std::map<int, vector<const Node*>> layers;

      BDD() { 
        zero = new Node(0, 0, NULL, NULL);
        one  = new Node(1, 1, NULL, NULL);
        layers[0].push_back(zero);
        layers[0].push_back(one);
      }

      const Node * add_simple_input(int s) {
        return create_node(s, s, one, zero);
      }

      /*int clone_node(int x) {
        if(x == 0) return 0;
        if(x == 1) return 1;
      }
      */

      const Node * negate(const Node * x) {
        if(x == zero) return one;
        if(x == one) return zero;

        return create_node(x->s, -1, x->p, x->n);
      }

      const Node * conjunction(const Node * x, const Node * y) {
        if(x == one) return y;
        if(y == one) return x;
        if(x == zero) return zero;
        if(y == zero) return zero;

        if(x->s == y->s) {
          const Node * p = conjunction(x->p, y->p);
          const Node * n = conjunction(x->n, y->n);
          return create_node(x->s, -1, p, n);
        }
        else if(x->s > y->s) {
          const Node * p = conjunction(x->p, y);
          const Node * n = conjunction(x->n, y);
          return create_node(x->s, -1, p, n);
        }
        else { // x < y
          const Node * p = conjunction(y->p, x);
          const Node * n = conjunction(y->n, x);
          return create_node(y->s, -1, p, n);
        }
      }

      const Node * create_node(int s, int r, const Node * p, const Node * n) {
        const Node * x = new Node(s, r, p, n);
        layers[s].push_back(x);
        return x;
      }


      friend std::ostream& operator<<(std::ostream & out, const BDD & b) {
        out << "digraph G {\n";
        //out << "  0 [shape=box,style=filled];\n";
        //out << "  1 [shape=box,style=filled];\n";
        for(auto [s, layer] : b.layers) {
          for(const Node * x : layer) {
            const void * p = x;
            std::string fill = x->r_t >=0 ? "style=filled," : "";
            out << "  \"" << p << "\" [" << fill << "label=\""<< x->s << ","<< p<< "\"];\n";
          }
          for(const Node * x : layer) {
            if(x!=b.zero && x != b.one) {
              out << "  \"" << x << "\"->\"" << x->p << "\"\n";
              out << "  \"" << x << "\"->\"" << x->n << "\"[style=dotted];\n";
            }
          }
        }
        out << "};\n";
        return out;
      }
    };
  }
}
