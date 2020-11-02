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
	int n, m, k, l; std::cin >> n >> m >> k >> l;

	graph G(2*n+2);
	edge_adder adder(G);

	int source = 2*n;
	int sink = 2*n+1; 

	//Read in police stations
	for (int i = 0; i < k; i++){
		int curStation_graph1; std::cin >> curStation_graph1;
		int curStation_graph2 = curStation_graph1 + n;

		adder.add_edge(source, curStation_graph1, 1);
		adder.add_edge(curStation_graph2, sink, 1);
	}

	//Read in locations of photos
	for (int i = 0; i < l; i++){
		int curLoc_graph1; std::cin >> curLoc_graph1;
		int curLoc_graph2 = curLoc_graph1 + n;

		adder.add_edge(curLoc_graph1, curLoc_graph2, 1);
	}

	//Read in one-way streets
	for (int i = 0; i < m; i++){
		int x_g1, y_g1; std::cin >> x_g1 >> y_g1;
		int x_g2 = x_g1+n; int y_g2 = y_g1+n;
		adder.add_edge(x_g1, y_g1, k);
		adder.add_edge(x_g2, y_g2, 1);
	}

	long flow = boost::push_relabel_max_flow(G, source, sink);
	std::cout << flow << std::endl;

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test2.in");
	std::cin.rdbuf(in.rdbuf());
	
	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
