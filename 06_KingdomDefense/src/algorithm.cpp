#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>


// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
	int l, p; std::cin >> l >> p;

	std::vector<int> demands(l);
	graph G(l+2);
	edge_adder adder(G);

	for (int i = 0; i < l; i++){
		int g, d; std::cin >> g >> d;
		demands[i] = d-g;
	}
	for (int i = 0; i < p; i++){
		int fj, tj, cj, Cj; std::cin >> fj >> tj >> cj >> Cj;
		adder.add_edge(fj, tj, Cj-cj);
		demands[fj] += cj;
		demands[tj] -= cj;
	}

	int source = l;
	int sink = l+1;
	int maxFlowNeeded = 0;
	for (int i = 0; i < l; i++){
		if (demands[i] < 0){
			adder.add_edge(source, i, -demands[i]);

		} else if (demands[i] > 0) {
			adder.add_edge(i, sink, demands[i]);
			maxFlowNeeded += demands[i];
		}
	}

	long flow = boost::push_relabel_max_flow(G, source, sink);
	if (flow >= maxFlowNeeded){
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
