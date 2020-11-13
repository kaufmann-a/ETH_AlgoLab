#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
int MAX_BET = 100;
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
	int N, M, S; std::cin >> N >> M >> S;

	// Create graph, edge adder class and propery maps
    graph G(N+M+S+1); //source = 0; buyers = 1-N; sites = N+1-N+M; states = N+M+1-N+M+S; sink = N+M+S+1
    int source = 0; int sink = N+M+S+1;
	
	edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

	for (int i = N+M+1; i <= N+M+S; i++){
		int nrSites; std::cin >> nrSites;
		adder.add_edge(i, sink, nrSites, 0);
	}
	for (int i = N+1; i <= N+M; i++){
		int statej; std::cin >> statej; statej += (N+M);
		adder.add_edge(i, statej, 1, 0);
	}
	for (int i = 1; i <= N; i++){
		for (int j = 1; j <= M; j++){ //bet of buyer i for site j
			int bet; std::cin >> bet; int curBuyer = i; int curSite = j+N;
			int transformed_bet = (-1 * bet) + MAX_BET; //Transformation of bet to get a graph with pos. cost only (max. bet is 100)
			adder.add_edge(curBuyer, curSite, 1, transformed_bet);
		}
	}
	for (int i = 1; i <= N; i++){
		adder.add_edge(source, i, 1, 0);
	}

	// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost = boost::find_flow_cost(G);

    // Iterate over all edges leaving the source to sum up the flow values.
    int nrSitesSold = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        nrSitesSold += c_map[*e] - rc_map[*e];     
    }
	std::cout << nrSitesSold << " " << MAX_BET*nrSitesSold - cost << std::endl;
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