#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


void testcase() {
	int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;

	// Create graph, edge adder class and propery maps
    graph G(c+1);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

	int start_node = c;

	//read in edges
	for (int i = 0; i < g; i++){
		int x, y, d, e; std::cin >> x >> y >> d >> e;
		adder.add_edge(x, y, e, d);
	}

	//Add edge start node
	adder.add_edge(start_node, k, std::numeric_limits<int>::max(), 0);

	boost::successive_shortest_path_nonnegative_weights(G, start_node, a);
    int cost2 = boost::find_flow_cost(G);

	int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(start_node,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }

	//Maybe already in budget?
	if (cost2 <= b){
		std::cout << s_flow << std::endl;
		return;
	}

	//Do binsearch 
	int l = 0; int r = s_flow; 
	int cur_best = 0;
	while (l < r){
		int mid = l + (r-l)/2;

		out_edge_it ebg, eend;
		for (boost::tie(ebg, eend) = boost::out_edges(c, G); ebg != eend; ++ebg){
			int cur_cap = boost::get(boost::edge_capacity_t(), G, *ebg);
			boost::put(boost::edge_capacity_t(), G, *ebg, mid);
		}
		boost::successive_shortest_path_nonnegative_weights(G, start_node, a);
    	int next_cost = boost::find_flow_cost(G);
		
		if (next_cost <= b){
			cur_best = mid;
			l = mid+1;
		} else {
			r = mid;
		}
	}
	std::cout << cur_best << std::endl;
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
