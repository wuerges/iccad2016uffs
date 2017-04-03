

#include <graph_model_arbitrary.hpp>
#include <inputs_generator.hpp>
#include <graph_simulator.hpp>

#include <rapidcheck.h>


#include <boost/tuple/tuple.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <algorithm>

int main() {
  rc::check("double reversal yields the original value",
            [](const std::vector<int> &l0) {
              auto l1 = l0;
              std::reverse(begin(l1), end(l1));
              std::reverse(begin(l1), end(l1));
              RC_ASSERT(l0 == l1);
            });

  rc::check("nodes zero and one have no inputs",
    [](const G & g) {
      RC_ASSERT(in_degree(g.zero, g.graph) == 0);
      RC_ASSERT(in_degree(g.one, g.graph) == 0);
    });

  rc::check("inputs have no incomming edges and outputs have no outgoing edges",
    [](const rc::AG & ag) {
      G_builder b = ag.b;
      for(auto it : b.inputs)
        RC_ASSERT(in_degree(b.get_vertex(it), b.g.graph) == 0);

      for(auto it : ag.b.outputs)
        RC_ASSERT(out_degree(b.get_vertex(it), b.g.graph) == 0);
    });

  rc::check("check if a graph is simulated 2 times with same input, it outputs the same results",
    [](const rc::AG & ag) {
      
      Generator gen;
      std::map<std::string, bool> input, output1, output2;
      gen.generate_inputs(ag.b.inputs, input);

      G_builder b = ag.b;

      simulate(input, output1, b);
      simulate(input, output2, b);

      RC_ASSERT(output1 == output2);

    });




  return 0;
}
