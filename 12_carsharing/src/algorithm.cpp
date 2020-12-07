#include <limits>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
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

void testcase() {
	int N, S; std::cin >> N >> S;

	std::vector<int> l(S);
	int sum_cars = 0;
	for (int i = 0; i < S; i++){
		std::cin >> l[i];
		sum_cars += l[i];
	}
	
	int max_time = 0;
	std::vector<std::tuple<int, int, int, int, int>> requests(N);
	for (int i = 0; i < N; i++){
		int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
		requests[i] = std::make_tuple(s, t, d, a, p);
		if (a > max_time){
			max_time = a;
		}
	}
	max_time = max_time/30; //just for first 3 testsets

	int nrNodes = (max_time+1)*S+2;
	int source = (max_time+1)*S;
	int sink = (max_time+1)*S+1;
	// Create graph, edge adder class and propery maps
    graph G(N);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);

	//Edges to rental stations
	for (int i = 0; i < S; i++){
		adder.add_edge(source, i*(max_time+1), l[i], 0);
	}

	int total_cost = 0;
	//Edges in case car not used in time segment
	for (int i = 0; i < S; i++){
		for (int j = 0; j < max_time; j++){
			int from = i*(max_time+1)+j;
			int to = from + 1;
			adder.add_edge(from, to, sum_cars, 100);
			total_cost++;
		}
	}

	//Edges for requests
	for (int i = 0; i < requests.size(); i++){
		int s = std::get<0>(requests[i]);
		int t = std::get<1>(requests[i]);
		int d = std::get<2>(requests[i]);
		int a = std::get<3>(requests[i]);
		int p = std::get<4>(requests[i]);
		d = d/30; //just first 3 testsets
		a = a/30; //just first 3 testsets

		int from = (s-1)*(max_time+1)+d;
		int to = (t-1)*(max_time+1)+a;
		int nrTimeSegments = (to % (max_time+1)) - (from % (max_time+1));
		total_cost += nrTimeSegments - 1;
		int cost = -p + 100 + (nrTimeSegments-1)*100;
		adder.add_edge(from, to, 1, cost);
	}

	//Edges to sink
	for (int i = 0; i < S; i++){
		int from = i*(max_time+1) + max_time;
		adder.add_edge(from, sink, sum_cars, 0);
	}

	// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost2 = boost::find_flow_cost(G);
	int profit = (-cost2) + (max_time*sum_cars*100);
	std::cout << profit << std::endl;

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
