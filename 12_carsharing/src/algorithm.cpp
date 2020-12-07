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

int MAX_TIME = 100001;
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
	std::map<int, int> node_index;
	
	std::vector<int> l(S);
	int sum_cars = 0;
	for (int i = 0; i < S; i++){
		std::cin >> l[i];
		sum_cars += l[i];

		int index_start = MAX_TIME*i;
		int index_end = MAX_TIME*(i+1)-1;
		node_index.insert(std::make_pair(index_start, node_index.size()));
		node_index.insert(std::make_pair(index_end, node_index.size()));
	}
	
	std::vector<std::tuple<int, int, int, int, int>> requests(N);
	for (int i = 0; i < N; i++){
		int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
		requests[i] = std::make_tuple(s, t, d, a, p);
		
		int index_s = MAX_TIME*(s-1)+d;
		if (node_index.find(index_s) == node_index.end()){
			node_index.insert(std::make_pair(index_s, node_index.size()));
		}
		int index_t = MAX_TIME*(t-1)+a;
		if (node_index.find(index_t) == node_index.end()){
			node_index.insert(std::make_pair(index_t, node_index.size()));
		}
	}

	int nrNodes = node_index.size()+2;
	int source = node_index.size();
	int sink = node_index.size()+1;

	// Create graph, edge adder class and propery maps
    graph G(N);
    edge_adder adder(G);  

	//Edges to rental stations and inside stations
	for (int i = 0; i < S; i++){
		
		int time1 = MAX_TIME*i;
		int index1 = node_index[MAX_TIME*i];
		adder.add_edge(source, index1, l[i], 0); //Edge to rental station
	}

	//Edges in case car not used in time segment
	std::map<int, int>::iterator it_beg = node_index.begin();
	int time1 = it_beg->first;
	int index1 = it_beg->second;
	it_beg++;
	for (std::map<int, int>::iterator it = it_beg; it != node_index.end(); it++){
		int time2 = it->first;
		int index2 = it->second;
		if (time1/MAX_TIME == time2/MAX_TIME){
			int nrTimeSegments = time2-time1;
			adder.add_edge(index1, index2, sum_cars, nrTimeSegments*100);
		}
		time1 = time2;
		index1= index2;
	}

	//Edges for requests ident for node: 100'000*s_i+time (s_i = 0 ... S-1)
	for (int i = 0; i < requests.size(); i++){
		int s = std::get<0>(requests[i]);
		int t = std::get<1>(requests[i]);
		int d = std::get<2>(requests[i]);
		int a = std::get<3>(requests[i]);
		int p = std::get<4>(requests[i]);

		int from_time = (s-1)*MAX_TIME+d;
		int to_time = (t-1)*MAX_TIME+a;
		int nrTimeSegments = a-d;
		int cost = -p + (nrTimeSegments)*100;
		adder.add_edge(node_index[from_time], node_index[to_time], 1, cost);
	}

	//Edges to sink
	for (int i = 0; i < S; i++){
		int from = MAX_TIME*(i+1)-1;
		adder.add_edge(node_index[from], sink, sum_cars, 0);
	}

	// Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
    boost::successive_shortest_path_nonnegative_weights(G, source, sink);
    int cost2 = boost::find_flow_cost(G);
	int profit = (-cost2) + ((MAX_TIME-1)*sum_cars*100);
	std::cout << profit << std::endl;
	int s_flow = 0;
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
