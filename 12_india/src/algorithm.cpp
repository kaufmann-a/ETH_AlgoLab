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

std::pair<int, int> calculateCost(int nrElephants, int c, std::vector<std::tuple<int, int, int, int>> gra, int k, int a){

	// Create graph, edge adder class and propery maps
    graph G(c+1);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

	int source = c; 
	adder.add_edge(source, k, nrElephants, 0);

	for (int i = 0; i < gra.size(); i++){
		int from = std::get<0>(gra[i]);
		int to = std::get<1>(gra[i]);
		int cost = std::get<2>(gra[i]);
		int cap = std::get<3>(gra[i]);
		adder.add_edge(from, to, cap, cost);
	}

	// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, source, a);
    int cost2 = boost::find_flow_cost(G);
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }
	return std::make_pair(cost2, s_flow);
}

void testcase() {
	int c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;

	std::vector<std::tuple<int, int, int, int>> G(c);
	for (int i = 0; i < g; i++){
		int x, y, d, e; std::cin >> x >> y >> d >> e;
		G[i] = std::make_tuple(x, y, d, e);
	}

	//Do binary search on the number of elephants
	//Max nr. elephants = max_e*nrGuides* = 10³*5*10³
	int l = 0; 
	int r = 5000000;
	int curBest = 0;
	while (l < r){
		int curNrElephants = 0;
		if ((r-l) % 2 == 1) { 
			curNrElephants = l+(r-l)/2+1; 
		} else { 
			curNrElephants = l+(r-l)/2; 
		}
		std::pair<int, int> result = calculateCost(curNrElephants, c, G, k, a);
		
		//First check if enough elephands available
		if (result.second < curNrElephants){
			//If not enough elephants, check if in budged with used elephants
			if (result.first < b){
				curBest = result.second;
				break;
			} else {
				r = result.second;
			}
		} else {
			if (result.first < b) {
				l = curNrElephants;
				curBest = curNrElephants;
			} else if (result.first > b) {
				if (r == curNrElephants){
					r -= 1;
				} else {
					r = curNrElephants;
				}
			} else if (result.first == b){
				curBest = curNrElephants;
				break;
			}
		}
	}
	std::cout << curBest << std::endl;
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
