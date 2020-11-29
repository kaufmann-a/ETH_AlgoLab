#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

// Includes
#include <iostream>
#include <vector>
// BGL includes 
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>

// Graph Type, OutEdgeList Type, VertexList Type, (un)directedS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges
typedef boost::graph_traits<graph>::vertex_iterator vertex_it;

void testcase() {
	long n, m; std::cin >> n >> m;

	std::vector<long> balances(n);

	for (int i = 0; i < n; i++){
		std::cin >> balances[i];
	}
	graph G(n);
	std::vector<bool> hasOutGoingEdge(n, false);
	for (int i = 0; i < m; i++){
		long from, to, dij; std::cin >> from >> to >> dij;
		hasOutGoingEdge[from] = true;
		boost::add_edge(from, to, G);
	}
	// Connected components
	// ====================
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	//int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
	int ncc = boost::strong_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G)));

	std::vector<long> balancePerComp(ncc);

	for (int i = 0; i < n; i++){
		balancePerComp[component_map[i]] += balances[i];
	}

	bool foundFreeState = false;
	for (int i = 0; i < ncc; i++){
		if (balancePerComp[i] > 0){
			foundFreeState = true;
			break;
		}
	}

	// for (int i = 0; i < n; i++){
	// 	if (!hasOutGoingEdge[i] && balances[i] > 0){
	// 		foundFreeState = true;
	// 	}
	// }

	if (foundFreeState){
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}
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
