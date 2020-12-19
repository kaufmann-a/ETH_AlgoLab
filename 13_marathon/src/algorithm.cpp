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
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>>      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc_flow;
typedef traits::edge_descriptor edge_desc_flow;

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
	int n, m, s, f; std::cin >> n >> m >> s >> f;

	weighted_graph G(n);
  	weight_map weights = boost::get(boost::edge_weight, G);

	std::vector<std::tuple<int, int, int, int>> streets(m);
	for (int i = 0; i < m; i++){
		int a, b, c, d; std::cin >> a >> b >> c >> d;
		streets[i] = std::make_tuple(a, b, c, d);
	}

	//Build graph to run dijkstras
	std::vector<std::vector<std::pair<int, int>>> adj_matrix(n, std::vector<std::pair<int, int>>(n));
	for (int i = 0; i < m; i++){
		int u = std::get<0>(streets[i]);
		int v = std::get<1>(streets[i]);
		int length = std::get<3>(streets[i]);
		int nrPeople = std::get<2>(streets[i]);
		edge_desc e = boost::add_edge(u, v, G).first; weights[e]=std::get<3>(streets[i]);
		if (adj_matrix[u][v].first != 0){
			if (adj_matrix[u][v].first > length){
				adj_matrix[u][v] = std::make_pair(length, nrPeople);
				adj_matrix[v][u] = std::make_pair(length, nrPeople);
			} else if (adj_matrix[u][v].first == length){
				adj_matrix[u][v].second += nrPeople;
				adj_matrix[v][u].second += nrPeople;
			}
		} else {
			adj_matrix[u][v] = std::make_pair(length, nrPeople);
			adj_matrix[v][u] = std::make_pair(length, nrPeople);
		}
	}

	std::vector<vertex_desc> path;
	std::vector<int>         dist_map(n);
  	std::vector<vertex_desc> pred_map(n);

	boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(
		dist_map.begin(), boost::get(boost::vertex_index, G))).predecessor_map(boost::make_iterator_property_map(
			pred_map.begin(), boost::get(boost::vertex_index, G))));

	std::vector<std::vector<bool>> visited(n, std::vector<bool>(n));

	graph G_flow(n);
	edge_adder adder(G_flow);
	for (int i = 0; i < n; i++){
		int u = pred_map[i];
		int v = i;
		int capacity = adj_matrix[u][v].second;
		if (capacity != 0){
			adder.add_edge(u, v, capacity);
			adder.add_edge(v, u, capacity);
		}
		visited[u][v] = true;
		visited[v][u] = true;
	}

	//check which add. streets to add to flowgraph
	for (int i = 0; i < m; i++){
		int u = std::get<0>(streets[i]);
		int v = std::get<1>(streets[i]);
		int lentgth = adj_matrix[u][v].first;
		int nrPeople = adj_matrix[u][v].second;
		if (visited[u][v] == false){
			if (dist_map[u] + lentgth == dist_map[v]){
				adder.add_edge(u, v, nrPeople);
			} 
			visited[u][v] = true;
		}
		if (visited[v][u] == false){
			if (dist_map[v] + lentgth == dist_map[u]){
				adder.add_edge(v, u, nrPeople);
			}
		}
	}

	long flow = boost::push_relabel_max_flow(G_flow, s, f);
  	std::cout << flow << std::endl;

	
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
