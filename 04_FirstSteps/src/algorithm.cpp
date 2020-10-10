#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;


int dijkstra_dist(const weighted_graph &G, int s){
	int n = boost::num_vertices(G);
	std::vector<int> dist_map(n);

	boost::dijkstra_shortest_paths(G, s, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));

	int longest = 0;
	for (int i = 0; i < n; i++){
		if (dist_map[i] > longest){
			longest = dist_map[i];
		}
	}
	return longest;
}


int kruskal(const weighted_graph &G){
	std::vector<edge_desc> mst; //Prepare vector to store mst
	
	//Calculate min spanning tree
	boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst)); //Call boost function to actually calculate MinimumSpanningTree and store it in mst object

	int sum = 0;
	for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); it++){
		sum += boost::get(boost::edge_weight_t(), G, *it);
	}

	return sum;
}

void testcase() {
	int n; std::cin >> n;
	int m; std::cin >> m;

	//std::vector<std::vector<int>> graph(n);
	weighted_graph G(n);
	
	for (int i = 0; i < m; i++){
		int startp; std::cin >> startp;
		int endp; std::cin >> endp;
		int weight; std::cin >> weight;
		boost::add_edge(startp, endp, weight, G);
	}
	int sum = kruskal(G);
	int longest = dijkstra_dist(G, 0);

	std::cout << sum << " " << longest << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/graphs_easy.in");
	std::cin.rdbuf(in.rdbuf());
	
	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
