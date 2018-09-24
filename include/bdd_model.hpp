
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
    using std::map;
    using std::pair;
    using std::optional;
    // using std::set;

    struct Node {
      //

      // key get_key() {
      //   return key(s, {p, n});
      // }

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
      optional<int>  r_t;
      /**
       * The positive edge for this node.
       */
      Node * p;
      /**
       * The negative edge for this node.
       */
      Node * n;

      Node(const int s_, Node * p_, Node * n_, optional<int> r_t_ = optional<int>()):
        s(s_), r_t(r_t_), p(p_), n(n_)
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

    typedef pair<Node*, Node*> sib;

    struct BDD {
      //vector<Node> graph;
      Node * zero;
      Node * one;
      vector<map<sib, Node*>> layers;
      int index = 2;
      map<std::string, int> symbol_table;
      map<int, std::string> symbol_table_rev;

      int number_of_nodes = 0;

      BDD():zero(new Node(0)), one(new Node(1)), layers(2) {
        layers[0][{zero,zero}] = zero;
        layers[1][{one,one}] = one;
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
        return create_node(s, one, zero, s);
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

      int height(Node*x) {
        return layers.size();
      }

//       def get_items(self):
// if self.x == 0 or self.x == 1:
//   return [self.get_item()]
// return [self.get_item()] + self.neg.get_items() + self.pos.get_items()


      /**
       *
       */
      Node * check_equivalent(Node * x) {
        if (x == one || x == zero) return x;
        if (x->p == x->n) return x->p;
        return x;
      }

      void check_sons(Node * x) {
        if(x == one || x == zero) return;
        Node *xp = check_equivalent(x->p);
        Node *xn = check_equivalent(x->n);
        if(x->p != xp || x->n != xn){
          x->p = xp;
          x->n = xp;
          layers[x->s].erase(sib{x->p, x->n});
          layers[x->s][sib{xp, xn}] = x; // TODO this may overwrite a node
        }
      }

      /**
       * WARNING: This is really slow O(2^n)
       */

      void simplify_slow(Node * z) {
      // vector<vector<Node*>> layers;
        for(int l = 1; l < layers.size(); ++l) {
          for(int i = 0; i < layers[l].size(); ++i) {
          }
        }

    // def simplify(z):
    //   for h in range(z.height()):
    //     #phase 1: remove redundant
    //     z = z.remove_redundant()
    //
    //     #phase 2: remove equals
    //     bdds = z.get_items()
    //     x = {}
    //     for k, v in bdds:
    //       if not k in bdds:
    //         x[k] = v
    //     for k, v in x.items():
    //       v.pos = x[v.pos.get_key()]
    //       v.neg = x[v.neg.get_key()]
    //
    //     return z
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

      /**
      * Won't actually create a node if not needed.
      */

      Node * create_node(int s, Node * p, Node * n, optional<int> r = optional<int>()) {
        if(layers.size() < s+1) layers.resize(s+1);
        auto it = layers[s].find(sib{p,n});
        if(it == layers[s].end()) {
          Node * x = new Node(s, p, n, r);
          number_of_nodes++;
          layers[s][sib{p,n}] = x;
          return x;
        }
        else {
          if(r) {
            if(it->second->r_t) {
              // TODO Both have representatives!
            }
            else {
              it->second->r_t = r;
            }
          }
          return it->second;
        }
      }

      friend std::ostream& operator<<(std::ostream & out, const BDD & b) {
        out << "digraph G {\n";
        for(auto layer : b.layers) {
          for(auto [k, x] : layer) {
            const void * p = x;
            std::string fill = x->r_t.has_value() ? "style=filled," : "";
            std::string shape = x ==b.zero || x == b.one ? "shape=rectangle," : "";
            std::string variable = b.symbol_table_rev.find(x->s)->second;
            out << "  \"" << p << "\" [" << fill << shape << "label=\"" << variable << /*","<< p<< */"\"];\n";
          }
          for(auto [k,x] : layer) {
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
