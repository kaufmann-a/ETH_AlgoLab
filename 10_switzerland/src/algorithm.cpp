#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
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
	long n, m; std::cin >> n >> m;

	graph G(n+2);
	edge_adder adder(G);
	long source = n;
	long sink = n+1;

	std::vector<long> balances(n);

	long sumPosBalances = 0;
	for (int i = 0; i < n; i++){
		long curBal; std::cin >> curBal;
		if (curBal >= 0){
			adder.add_edge(source, i, curBal);
			sumPosBalances += curBal;
		} else {
			adder.add_edge(i, sink, -curBal);
		}
	}
	for (int i = 0; i < m; i++){
		long from, to, dij; std::cin >> from >> to >> dij;
		adder.add_edge(from, to, dij);
	}
	long flow = boost::push_relabel_max_flow(G, source, sink);

	if (flow >= sumPosBalances){
		std::cout << "no" << std::endl;
	} else {
		std::cout << "yes" << std::endl;
	}
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
