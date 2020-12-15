#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc_flow;
typedef boost::graph_traits<graph>::edge_iterator           edge_it_flow; // Iterator

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

int dijkstra_path(const weighted_graph &G, int s, int t, std::vector<vertex_desc> &path) {
  int n = boost::num_vertices(G);
  std::vector<int>         dist_map(n);
  std::vector<vertex_desc> pred_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G)))
    .predecessor_map(boost::make_iterator_property_map(
      pred_map.begin(), boost::get(boost::vertex_index, G))));

  int cur = t;
  path.clear(); path.push_back(cur);
  while (s != cur) {
    cur = pred_map[cur];
    path.push_back(cur);
  }
  std::reverse(path.begin(), path.end());
  return dist_map[t];
}

void testcase() {
	int n, m, a, s, c, d; std::cin >> n >> m >> a >> s >> c >> d;

	weighted_graph G(n);
  	weight_map weights = boost::get(boost::edge_weight, G);

	for (int i = 0; i < m; i++){
		char w; int x, y, z; std::cin >> w >> x >> y >> z;
		edge_desc e1;
		e1 = boost::add_edge(x, y, G).first; weights[e1]=z;
		if (w == 'L'){
			edge_desc e2;
			e2 = boost::add_edge(y, x, G).first; weights[e2] = z;
		}
	}

	std::vector<int> agents(a, 0);
	for (int i = 0; i < a; i++){
		std::cin >> agents[i];
	}
	std::vector<int> shelters(s, 0);
	for (int i = 0; i < s; i++){
		std::cin >> shelters[i];
	}

	// Create graph, edge adder class and propery maps
    graph G_flow(2*n+2);
    edge_adder adder(G_flow);  
    auto c_map = boost::get(boost::edge_capacity, G_flow);
    auto r_map = boost::get(boost::edge_reverse, G_flow);
    auto rc_map = boost::get(boost::edge_residual_capacity, G_flow);
	int source = 2*n;
	int sink = 2*n+1;
	for (int i = 0; i < a; i++){
		//Run dijkstra for every agent
		int startNode = agents[i];
  		std::vector<int> dist_map(n, -1);
  		std::vector<vertex_desc> pred_map(n);
  		boost::dijkstra_shortest_paths(G, startNode, boost::distance_map(
			  boost::make_iterator_property_map(dist_map.begin(), 
			  boost::get(boost::vertex_index, G))).predecessor_map(
				  boost::make_iterator_property_map(pred_map.begin(), 
				  boost::get(boost::vertex_index, G))));
		
		adder.add_edge(source, startNode, 1, 0);

		//Iterate over all rachable targets and add shortest distance
		for (int j = 0; j < s; j++){
			int curShelter = shelters[j];
			if (dist_map[curShelter] != std::numeric_limits<int>::max()){
				adder.add_edge(startNode, n+curShelter, 1, dist_map[curShelter]);
			}
		}
	}

	for (int i = 0; i < s; i++){
		int curShelter = shelters[i];
		adder.add_edge(n+curShelter, sink, c, 0);
	}

	boost::successive_shortest_path_nonnegative_weights(G_flow, source, sink);

	edge_it_flow e_beg, e_end;
	int longest_edge = 0;
	for (boost::tie(e_beg, e_end) = boost::edges(G_flow); e_beg != e_end; ++e_beg){
		int s_flow = c_map[*e_beg] - rc_map[*e_beg];
		if (s_flow > 0){
			auto w_map = boost::get(boost::edge_weight, G_flow);
			int cost = w_map[*e_beg];
			if (cost > longest_edge){
				longest_edge = cost;
			}
		}

	}

	std::cout << longest_edge + d << std::endl;

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
