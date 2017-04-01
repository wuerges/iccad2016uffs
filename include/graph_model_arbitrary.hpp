#pragma once

#include <graph_model.hpp>
#include <rapidcheck.h>

#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace verilog;
using namespace graph;

namespace rc
{

  // represents an arbitrary edge to be used with AG
  struct AEdge {
    std::string from, to;
    NegP polarity;
    AEdge(const std::string & f, const std::string & t, const NegP & p): 
      from(f), to(t), polarity(p) {}
  };

  // represents an arbitrary graph, generated automatically
  struct AG {
    G_builder b;
    AG(const std::vector<AEdge> & edges) {
      for(auto it : edges) {
        // Only add edges in one direction, so
        // the resulting graph is directed.
        if (it.from > it.to)
          b.add_edge(it.from, it.to, it.polarity);
      }
      remove_in_edge_if(b.g.one, [](auto e){ return true; } , b.g.graph);
      remove_in_edge_if(b.g.zero, [](auto e){ return true; } , b.g.graph);

      // a reverse name map;
      std::map<int, std::string> rev_map;
      for(auto it : b.name_map) 
        rev_map[it.second] = it.first;

      GD::vertex_iterator v, vend;
      for(boost::tie(v, vend) = boost::vertices(b.g.graph); v != vend; ++v) {
        if(in_degree(*v, b.g.graph) == 0)
          b.inputs.push_back(rev_map[*v]);
        if(out_degree(*v, b.g.graph) == 0)
          b.outputs.push_back(rev_map[*v]);
      }
    }
  };


template<>
struct Arbitrary<NegP> {
  static Gen<NegP> arbitrary() {
    return gen::element(NegP::Positive, NegP::Negative);
  }
};


template<>
struct Arbitrary<AEdge> {
  static Gen<AEdge> arbitrary() {
    return gen::construct<AEdge>(
        gen::arbitrary<std::string>(),
        gen::arbitrary<std::string>(),
        gen::arbitrary<NegP>());
  }
};

template<>
struct Arbitrary<AG> {
  static Gen<AG> arbitrary() {

    auto nodes = *gen::nonEmpty<std::vector<std::string>>();

    // Generator for edges, with edges from `nodes'
    auto gen_edge = gen::construct<AEdge>(
        gen::elementOf(nodes),
        gen::elementOf(nodes),
        gen::arbitrary<NegP>());

    // Generator for edges, adding only edges contained in `nodes'
    const auto gen_edges = gen::container<std::vector<AEdge>>(gen_edge);
    
    // Generator for the AG, which contains an g of type G, our netlist;
    return gen::construct<AG>(gen_edges);
  }
};

template<>
struct Arbitrary<G> {
  static Gen<G> arbitrary() {

    return gen::apply([](const AG & ag) {
          return ag.b.g;
        }, gen::arbitrary<AG>());
  }
};



}
