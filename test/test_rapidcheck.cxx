

#include <graph_model_arbitrary.hpp>

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
    [](const rc::AG & ag) {
      RC_ASSERT(in_degree(ag.g.zero, ag.g.graph) == 0);
      RC_ASSERT(in_degree(ag.g.one, ag.g.graph) == 0);
    });

  rc::check("inputs have no incomming edges and outputs have no outgoing edges",
    [](const rc::AG & ag) {
      G g = ag.g;
      for(auto it : g.inputs)
        RC_ASSERT(in_degree(g.get_vertex(it), g.graph) == 0);

      for(auto it : ag.g.outputs)
        RC_ASSERT(out_degree(g.get_vertex(it), g.graph) == 0);
    });



  return 0;
}
