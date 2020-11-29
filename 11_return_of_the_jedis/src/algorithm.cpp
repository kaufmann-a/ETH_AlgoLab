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
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

bool sortEdge(std::tuple<int, int, int> &edge1, std::tuple<int, int, int> &edge2){
	return std::get<2>(edge1) < std::get<2>(edge2);
}

int calculate_mst(std::vector<std::tuple<int, int, int>> &edges, std::vector<int> &mst_edge_indexes, int n, int nrEdgeToIgnore){
	int cost = 0;
	//int index_mst_edges = 0;
	boost::disjoint_sets_with_storage<> union_find(n);
	for (int i = 0; i < edges.size(); i++){
		if (nrEdgeToIgnore == i){
			continue;
		}
		int u = union_find.find_set(std::get<0>(edges[i]));
		int v = union_find.find_set(std::get<1>(edges[i]));
		if (u != v){
			union_find.link(u, v);
			cost += std::get<2>(edges[i]);
			mst_edge_indexes.push_back(i);
			if(--n == 1) break;
			//index_mst_edges++;
		}
	}
	return cost;
}


void testcase() {
	int n, index; std::cin >> n >> index;

	std::vector<std::tuple<int, int, int>> edges;
	for(int i = 0; i < n-1; i++) {
		for(int j = i+1; j < n; j++) {
			int cost;
			std::cin >> cost;
			edges.push_back(std::make_tuple(i, j, cost));
		}
	}
	
	std::sort(edges.begin(), edges.end(), sortEdge);
	std::vector<int> mst;
	calculate_mst(edges, mst, n, edges.size()+1);

	int cost = std::numeric_limits<int>::max();
	for (int i = 0; i < mst.size(); i++){
		int curEdgeToIgnore = mst[i];
		std::vector<int> mst_cur;
		int curCost = calculate_mst(edges, mst_cur, n, curEdgeToIgnore);
		cost = std::min(curCost, cost);
	}
	std::cout << cost << std::endl;

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