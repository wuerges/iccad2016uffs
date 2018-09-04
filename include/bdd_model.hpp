
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
      std::optional<int>  r_t;
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
      int index = 2;
      std::map<std::string, int> symbol_table;
      std::map<int, std::string> symbol_table_rev;

      int number_of_nodes = 0;

      BDD():zero(new Node(0)), one(new Node(1)), layers(2) {
        layers[0].push_back(zero);
        layers[1].push_back(one);
        symbol_table_rev[0] = "Zero";
        symbol_table_rev[1] = "One";
      }

      Node * add_simple_input(const std::string & variable) {
        auto x = symbol_table.find(variable);
        if(x == symbol_table.end()) {
          symbol_table[variable] = index++;
        }
        int s = symbol_table[variable];
        symbol_table_rev[s] = variable;
        return create_node(s, s, one, zero);
      }

      Node * negate(Node * x) {
        if(x == zero) return one;
        if(x == one) return zero;

        return create_node(x->s, negate(x->p), negate(x->n));
      }

      Node * land(Node * x, Node * y) {
        if(x == one) return copy_of(y);
        if(y == one) return copy_of(x);
        if(x == zero) return zero;
        if(y == zero) return zero;

        if(x->s == y->s) {
          Node * p = land(x->p, y->p);
          Node * n = land(x->n, y->n);
          return create_node(x->s, p, n);
        }
        else if(x->s > y->s) {
          Node * p = land(x->p, y);
          Node * n = land(x->n, y);
          return create_node(x->s, p, n);
        }
        else { // x < y
          Node * p = land(y->p, x);
          Node * n = land(y->n, x);
          return create_node(y->s, p, n);
        }
      }

      Node * copy_of(Node * x) {
        return x;
        // TODO check if this is good or bad.
        //return create_node(x->s, x->p, x->n);
      }

      Node * lor(Node * x, Node * y) {
        if(x == one) return one;
        if(y == one) return one;
        if(x == zero) return copy_of(y);
        if(y == zero) return copy_of(x);

        if(x->s == y->s) {
          Node * p = lor(x->p, y->p);
          Node * n = lor(x->n, y->n);
          return create_node(x->s, p, n);
        }
        else if(x->s > y->s) {
          Node * p = lor(x->p, y);
          Node * n = lor(x->n, y);
          return create_node(x->s, p, n);
        }
        else { // x < y
          Node * p = lor(y->p, x);
          Node * n = lor(y->n, x);
          return create_node(y->s, p, n);
        }
      }

      Node * create_node(int s, int r, Node * p, Node * n) {
        number_of_nodes++;
        Node * x = new Node(s, r, p, n);
        if(layers.size() < s+1) layers.resize(s+1);
        layers[s].push_back(x);
        return x;
      }

      Node * create_node(int s, Node * p, Node * n) {
        number_of_nodes++;
        Node * x = new Node(s, p, n);
        if(layers.size() < s+1) layers.resize(s+1);
        layers[s].push_back(x);
        return x;
      }

      friend std::ostream& operator<<(std::ostream & out, const BDD & b) {
        out << "digraph G {\n";
        for(auto layer : b.layers) {
          for(const Node * x : layer) {
            const void * p = x;
            std::string fill = x->r_t.has_value() ? "style=filled," : "";
            std::string shape = x ==b.zero || x == b.one ? "shape=rectangle," : "";
            std::string variable = b.symbol_table_rev.find(x->s)->second;
            out << "  \"" << p << "\" [" << fill << shape << "label=\"" << variable << /*","<< p<< */"\"];\n";
          }
          for(const Node * x : layer) {
            if(x!=b.zero && x != b.one) {
              out << "  \"" << x << "\"->\"" << x->p << "\"\n";
              out << "  \"" << x << "\"->\"" << x->n << "\"[style=dotted];\n";
            }
          }
        }
        out << "}\n";
        return out;
      }
    };
  }
}
