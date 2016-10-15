
#pragma once

#include <data_model.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>
#include <sstream>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/copy.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graphviz.hpp>

namespace verilog
{
  namespace bdd {

    enum class NegP {
      Positive, Negative
    };

    enum class Type {
      Zero, One, Input
    };

    struct Node {
      std::string input_name;
      Type t;
    };

    typedef boost::adjacency_list<
      boost::setS,
      boost::setS,
      boost::bidirectionalS,
      Node, NegP> GD;

    typedef unsigned int hashcode;

    struct BDD {
      GD graph;

      GD::vertex_descriptor 
        /**
         * The nodes representing the sinks of the BDD
         */
        zero,
        one,
        /**
         * The node representing the source of the bdd
         */
        source;

      std::map<hashcode, std::vector<int> > bdd_heap;

      BDD(const std::string name) {
        zero = boost::add_vertex(graph);
        one  = boost::add_vertex(graph);
        source = add_vertex(name);

        add_edge(source, one, NegP::Positive);
        add_edge(source, zero, NegP::Negative);

        graph[zero].t = Type::Zero;
        graph[one].t  = Type::One;
      }

      /**
       * Creates a copy of a,
       * merges all nodes of b into the copy.
       * calls the conjunction_internal function for the copy.
       */

      void copy_graph(const BDD & o, 
          std::map<GD::vertex_descriptor,
                   GD::vertex_descriptor> & node_map
          ) {

        GD::vertex_iterator v, vend;
        for(boost::tie(v, vend) = boost::vertices(o.graph);
            v != vend; ++v) {
          GD::vertex_descriptor n = boost::add_vertex(graph);
          graph[n].input_name = o.graph[*v].input_name;
          graph[n].t          = o.graph[*v].t         ;
          node_map[*v] = n;
        }
        
        GD::edge_iterator e, eend;
        for(boost::tie(e, eend) = boost::edges(o.graph);
            e != eend; ++e) {
          GD::vertex_descriptor s = boost::source(*e, o.graph);
          GD::vertex_descriptor d = boost::target(*e, o.graph);
          boost::add_edge(node_map[s], node_map[d], o.graph[*e], graph);
        }
      }

      BDD operator&(const BDD & b) {

        std::map<GD::vertex_descriptor,
          GD::vertex_descriptor> node_map;

        BDD c = *this;
        c.copy_graph(b, node_map);

        GD::vertex_descriptor old_zero   = node_map[b.zero];
        GD::vertex_descriptor old_one    = node_map[b.one];
        GD::vertex_descriptor old_source = node_map[b.source];

        std::cout << "BDD c => " << c << "\n";

        // copies the old one edges into the new graph
        c.merge_input_edges(c.one, old_one);
        std::cout << "merged one c => " << c << "\n";

        std::cout << "merging " << old_one << " into " << c.one << "\n";
        boost::clear_vertex(old_one, c.graph);
        boost::remove_vertex(old_one, c.graph);

        // copies the old zero edges into the new graph
        c.merge_input_edges(c.zero, old_zero);
        std::cout << "merged zero c => " << c << "\n";
        std::cout << "merging " << old_zero << " into " << c.zero << "\n";
        boost::clear_vertex(old_zero, c.graph);
        boost::remove_vertex(old_zero, c.graph);

        std::cout << "before conjunction internal " << c << "\n";

        c.source = c.conjunction_internal(c.source, old_source);
        std::cout << "new source " << c.source << "\n";
        return c;


      }

      private:
      /**
       * Internal function that checks for all the cases
       * Accepts the nodes that must be conjoined
       * and returns the new source.
       */
      GD::vertex_descriptor
        conjunction_internal(
          GD::vertex_descriptor & a, 
          GD::vertex_descriptor & b) {
        std::cout << "conjunction_internal " << a << " " << b << "\n";
        add_edge(a, b, NegP::Positive);
        return 0;
      }


      GD::vertex_descriptor add_vertex(const std::string name) {
        GD::vertex_descriptor v = boost::add_vertex(graph);
        graph[v].input_name = name;
        graph[v].t          = Type::Input;
        return v;
      }

      void add_edge(
          GD::vertex_descriptor & a, 
          GD::vertex_descriptor & b, 
          NegP p) {
        boost::add_edge(a, b, p, graph);
      }

      /**
       * USE WITH CAUTION, as it changes the current BDD
       * - changes zero and one
       */
      public:
      void reverse() {
        GD::vertex_descriptor aux = one;
        one = zero;
        zero = aux;

        graph[zero].t = Type::Zero;
        graph[one].t  = Type::One;
      }

      /**
       * USE WITH CAUTION, as it changes the current BDD
       * merges the input edges of the node b into node a.
       */
      void merge_input_edges(
          GD::vertex_descriptor a, 
          GD::vertex_descriptor b) {
        GD::in_edge_iterator e, end;
        for (boost::tie(e, end) = in_edges(b, graph); e != end; ++e) {
            GD::vertex_descriptor s = boost::source(*e, graph);
            add_edge(s, a, graph[*e]);
        }
      }

      std::map<int, std::vector<int> > find_layers() {
        // TODO
      }

      // Criar um hash o mais unico possivel para um bdd
      hashcode hash(int v) {
        // TODO
      }

      // Compares BDDs
      friend bool operator<(const BDD & bdd1, const BDD & bdd2) {
        // TODO
      }

      /**
       * USE WITH CAUTION, as it changes the current BDD
       * - minimizes the BDD
       */
      void minimize() {
        // TODO
      }

      friend bool operator==(const BDD & bdd1, const BDD & bdd2) {
        return
          (bdd1.source == bdd2.source) &&
          (bdd1.one == bdd2.one) &&
          (bdd1.zero == bdd2.zero);
        // TODO &&
        //  (bdd1.graph == bdd2.graph);
      }
      friend bool operator!=(const BDD & bdd1, const BDD & bdd2) {
        return
          (bdd1.source != bdd2.source) &&
          (bdd1.one != bdd2.one) &&
          (bdd1.zero != bdd2.zero);
        // TODO&&
        //  (bdd1.graph != bdd2.graph);
      }

      public:
      int size() {
        int i = 0;
        GD::vertex_iterator n, e;
        for(boost::tie(n, e) = boost::vertices(graph);
            n != e; ++n) ++i;
        return i;
      }

      friend std::ostream& operator<<(std::ostream &o, const BDD & b) {

        o << "digraph G {\n" ;
        GD::vertex_iterator v, vend;
        for(boost::tie(v, vend) = boost::vertices(b.graph);
            v != vend; ++v) {
          switch(b.graph[*v].t) {
            case(Type::Input) : {
                o << *v << " [label=\""
                  << b.graph[*v].input_name
                  << "\", ";
                if (*v == b.source)
                  o << "shape=doublecircle];\n";
                else
                  o << "shape=circle];\n";
              break;
            }
            case(Type::One) : {
                o << *v << " [label=\"1\", shape=box]; \n";
              break;
            }
            case(Type::Zero) : {
                o << *v << " [label=\"0\", shape=box]; \n";
              break;
            }
          }
        }

        GD::edge_iterator e, eend;
        for(boost::tie(e, eend) = boost::edges(b.graph);
            e != eend; ++e) {
          GD::vertex_descriptor s = boost::source(*e, b.graph);
          GD::vertex_descriptor d = boost::target(*e, b.graph);

          o << s << "->" << d;
          switch(b.graph[*e]) {
            case(NegP::Positive): {
              o << " [style=solid];\n";
              break;
            }
            case(NegP::Negative): {
              o << " [style=dashed];\n";
              break;
            }
          }
        }


        o << "}\n" ;
        return o;
      }
    };
  }
}
