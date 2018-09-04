
#pragma once

#include "data_model.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
#include <string>
#include <optional>

namespace verilog
{
  namespace bdd {
    using std::vector;
    vector<std::string> symbol_table;

    struct Node {
      /**
       * Index of the node's input in the symbol table
       * s == 0 -> for the special node 0.
       * s == 1 -> for the special node 1.
       * s >= 2 -> for any regular variable.
       */
      const int s;

      /**
       * If this node is a representative, r_t stores it.
       */
      const std::optional<int>  r_t;
      /**
       * The positive edge for this node.
       */
      Node * p;
      /**
       * The negative edge for this node.
       */
      Node * n;

      Node(const int s_, const int r_t_, Node * p_, Node * n_):
        s(s_), r_t(r_t_), p(p_), n(n_)
      {}

      Node(const int s_, Node * p_, Node * n_):
        s(s_), p(p_), n(n_)
      {}

      /**
       * This constructor should only be used to build zero or one
       */
      Node(const int s_): s(s_)
        {
            p = this;
            n = this;
        }

    };

    struct BDD {
      //vector<Node> graph;
      Node * zero;
      Node * one;
      vector<vector<Node*>> layers;

      BDD():zero(new Node(0)), one(new Node(1)), layers(2) {
        layers[0].push_back(zero);
        layers[1].push_back(one);
      }

      Node * add_simple_input(int s) {
        return create_node(s, s, one, zero);
      }

      Node * negate(Node * x) {
        if(x == zero) return one;
        if(x == one) return zero;

        return create_node(x->s, negate(x->p), negate(x->n));
      }

      Node * conjunction(Node * x, Node * y) {
        if(x == one) return y;
        if(y == one) return x;
        if(x == zero) return zero;
        if(y == zero) return zero;

        if(x->s == y->s) {
          Node * p = conjunction(x->p, y->p);
          Node * n = conjunction(x->n, y->n);
          return create_node(x->s, p, n);
        }
        else if(x->s > y->s) {
          Node * p = conjunction(x->p, y);
          Node * n = conjunction(x->n, y);
          return create_node(x->s, p, n);
        }
        else { // x < y
          Node * p = conjunction(y->p, x);
          Node * n = conjunction(y->n, x);
          return create_node(y->s, p, n);
        }
      }

      Node * create_node(int s, int r, Node * p, Node * n) {
        Node * x = new Node(s, r, p, n);
        if(layers.size() < s+1) layers.resize(s+1);
        layers[s].push_back(x);
        return x;
      }

      Node * create_node(int s, Node * p, Node * n) {
        Node * x = new Node(s, p, n);
        if(layers.size() < s+1) layers.resize(s+1);
        layers[s].push_back(x);
        return x;
      }

      friend std::ostream& operator<<(std::ostream & out, const BDD & b) {
        out << "digraph G {\n";
        //out << "  0 [shape=box,style=filled];\n";
        //out << "  1 [shape=box,style=filled];\n";
        for(auto layer : b.layers) {
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
