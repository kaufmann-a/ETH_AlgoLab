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

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void testcase() {
	int n, m, k, x, y; std::cin >> n >> m >> k >> x >> y;


	//We draw k graphs, if the edge is a river, it connects graph i with graph i+1
	weighted_graph G((k+1)*n);
	for (int i = 0; i < m; i++){
		int a, b, c, d; std::cin >> a >> b >> c >> d;
		
		//First check if road is a river
		if (d == 1){
			for (int j = 0; j < k; j++){
				int startV_dir1 = (j*n)+a;
				int startV_dir2 = (j*n)+b;
				int targetV_dir1 = ((j+1)*n)+b;
				int targetV_dir2 = ((j+1)*n)+a;
				boost::add_edge(startV_dir1, targetV_dir1, c, G);
				boost::add_edge(startV_dir2, targetV_dir2, c, G);
			}
			for (int j = 0; j <= k; j++){ //Add edge in every subgraph
				int startV = (j*n)+a;
				int targetV = (j*n)+b;
				boost::add_edge(startV, targetV, c, G);
			}	
		} else {
			for (int j = 0; j <= k; j++){ //Add edge in every subgraph
				int startV = (j*n)+a;
				int targetV = (j*n)+b;
				boost::add_edge(startV, targetV, c, G);
			}		
		}
	}

	//Doing dijkstra on the graph
	std::vector<int> dist_map((k+1)*n);
	boost::dijkstra_shortest_paths(G, x, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));

	std::cout << dist_map[k*n+y] << std::endl;

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
