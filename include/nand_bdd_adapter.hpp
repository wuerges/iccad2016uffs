#pragma once

#include <graph_model.hpp>
#include <bdd_model.hpp>
#include <algorithm>
#include <map>
#include <queue>
#include <unordered_map>
#include <stdexcept>
#include <memory>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace verilog {

  using namespace graph;
  using namespace bdd;
  using std::shared_ptr;
  using std::vector;
  using std::unordered_map;
  using std::queue;

  typedef map<int, vector<shared_ptr<bdd::Node>>> layers_t;

  void get_layers(const BDD & b, layers_t & layers) {
    unordered_map<Node_p, bool> visited;
    queue<Node_p> todo;
    
    for(Node_p node : b.index) {
      todo.push(node);
    }

    while(!todo.empty()) {
      Node_p aux = todo.front();
      todo.pop();
      if(visited[aux]) continue;
      visited[aux] = true;
      if(aux == b.zero || aux == b.one) {
        layers[0].push_back(aux);
      }
      else {
        if(!visited[aux->p]) todo.push(aux->p);
        if(!visited[aux->n]) todo.push(aux->n);
        layers[aux->s].push_back(aux);
      }
    }
  }

  std::ostream& write_bdd(std::ostream & out
      , const BDD & b
      , const G & g
      ) {
    out << "digraph G {\n";
    layers_t layers;
    get_layers(b, layers);
    for(auto [key, layer] : layers) {
      out << "// layer " << key << "\n";
      for(auto x : layer) {
        std::string fill = x->r_t >=0 ? "style=filled," : "";
        std::string box = (x->s == 0 || x ->s == 1) ? "shape=box," : "";
        std::string name = g.graph[x->s].identifier;
        out << "  \"" << x << "\" [" << fill << box << "label=\""<< name;
        if(x->r_t>=0) {
          out << ":" << x->r_t;
        }
        if(false) { // set this to true to view pointer addresses in the graph.
          out << ","<< x;
        }
        out << "\"];\n";
      }
      for(auto x : layer) {
        if(x!=b.zero && x != b.one) {
          out << "  \"" << x << "\"->\"" << x->p << "\"\n";
          out << "  \"" << x << "\"->\"" << x->n << "\"[style=dotted];\n";
        }
      }
      out << " { rank = same; ";
      for(auto x : layer) {
        out << "\"" << x << "\";";
      }
      out << "}\n";
    }
    out << "}\n";
    return out;
  }


  void convert(graph::G &g, bdd::BDD & bdds) {
    using namespace boost;
    std::deque<int> topo_order;
    boost::topological_sort(g.graph, std::front_inserter(topo_order));

    bdds.index.clear();
    bdds.index.resize(num_vertices(g.graph));

    int node_count = 0;
    for (int node : topo_order) {
      if(in_degree(node, g.graph) == 0) {
        if(node == g.one) {
          bdds.index[node] = bdds.one;
        }
        else if(node == g.zero) {
          bdds.index[node] = bdds.zero;
        }
        else {
          bdds.index[node] = bdds.add_simple_input(node);
        }
      }
      else {
        auto p = in_edges(node, g.graph);

        vector<shared_ptr<bdd::Node>> inputs;
        for (auto e = p.first; e != p.second; ++e) {
          int s = source(*e, g.graph);
          if(NegP::Positive == g.graph[*e]) {
            inputs.push_back(bdds.index[s]);
          }
          else {
            inputs.push_back(bdds.negate(bdds.index[s]));
          }
        }

        shared_ptr<bdd::Node> aux = inputs[0];
        for(int i = 1; i < inputs.size(); ++i) {
          aux = bdds.conjunction(aux, inputs[i]);
        }
        shared_ptr<bdd::Node> result(
            bdds.create_node(aux->s, node, aux->p, aux->n));
        bdds.index[node] = result;
      }
      node_count++;
      if(node_count % 1000 == 0) std::cerr << "built bdds for " << node_count << "/" << topo_order.size() <<" nodes.\n";
    }
  }

}
