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

void testcase() {
	int n, e, s, a, b;
	std::cin >>n; std::cin >> e; std::cin >> s; std::cin >> a; std::cin >> b;

	//Create a graph for every species with its specific weights, forest[i] is graph of species i
	std::vector<weighted_graph> forest(s);
	// for (int i = 0; i < s; i++){
	// 	forest[i].resize(e);
	// }

	//Create a graph for every species
	for (int i = 0; i < e; i++){ //Loop over all edges of the graph 
		int t1, t2; std::cin >> t1; std::cin >> t2;
		for (int j = 0; j < s; j++){ //Loop over all species for edge i
			int time; std::cin >> time; //Time it takes for species j to travel from tree t1 to tree t2
			boost::add_edge(t1, t2, time, forest[j]);
		}
	}

	//Read in locatin of hives (do need them?)
	std::vector<int> hives(s);
	for (int i = 0; i < s; i++){
		std::cin >> hives[i];
	}

	//Create mst for every species
	std::vector<std::vector<edge_desc>> forest_msts(s);
	for (int i = 0; i < s; i++){
		boost::kruskal_minimum_spanning_tree(forest[i], std::back_inserter(forest_msts[i])); 
	}

	//Create final graph, always add edge of species with shortest travel time
	weighted_graph G(n);
	for (int i = 0; i < s; i++){ //Loop mst of every species
		for (std::vector<edge_desc>::iterator it = forest_msts[i].begin(); it != forest_msts[i].end(); it++){ //Loop over every edge in mst of species i
			int weight_i = boost::get(boost::edge_weight_t(), forest[i], *it);
			std::pair<edge_desc, bool> edge = boost::edge(boost::source(*it, G), boost::target(*it, G), G); //Look if edge already inserted in G
			if (edge.second){ //Edge already inserted by another species, check if smaller weight
				int weight_g = boost::get(boost::edge_weight_t(), G, edge.first);
				if (weight_i > weight_g){
					boost::put(boost::edge_weight_t(), G, edge.first, weight_i);
				}
			} else {
				boost::add_edge(boost::source(*it, forest[i]), boost::target(*it, forest[i]), weight_i, G);
			}
		}
	}

	//Calculate shortest paths from  a to b
	std::vector<int> dist_map(n);
	boost::dijkstra_shortest_paths(G, a, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
	std::cout << dist_map[b] << std::endl;



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
