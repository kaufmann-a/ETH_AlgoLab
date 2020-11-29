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
  boost::no_property, boost::property<boost::edge_weight_t, long> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

bool sortEdge(std::tuple<long, long, long> &edge1, std::tuple<long, long, long> &edge2){
	return std::get<2>(edge1) < std::get<2>(edge2);
}

long calculate_mst(std::vector<std::tuple<long, long, long>> &edges, std::vector<long> &mst_edge_indexes, long n, long nrEdgeToIgnore){
	long cost = 0;
	//int index_mst_edges = 0;
	std::vector<long> trial;
	boost::disjoint_sets_with_storage<> union_find(n);
	for (long i = 0; i < edges.size(); i++){
		if (nrEdgeToIgnore == i){
			continue;
		}
		long u = union_find.find_set(std::get<0>(edges[i]));
		long v = union_find.find_set(std::get<1>(edges[i]));
		if (u != v){
			union_find.link(u, v);
			cost += std::get<2>(edges[i]);
			trial.push_back((long) 12);
			//index_mst_edges++;
		}
	}
	return cost;
}


void testcase() {
	long n, index; std::cin >> n >> index;

	std::vector<std::tuple<long, long, long>> edges;
	for (long j = 1; j < n; j++){
		for (long k = 1; k <= n-j; k++){
			long weight; std::cin >> weight;
			edges.push_back(std::make_tuple(j, j+k, weight));
		}
	}
	
	std::sort(edges.begin(), edges.end(), sortEdge);
	std::vector<long> mst;
	long cost = calculate_mst(edges, mst, n, edges.size()+1);

	for (long i = 0; i < mst.size(); i++){
		long curEdgeToIgnore = mst[i];
		std::vector<long> mst_cur;
		long newCost = calculate_mst(edges, mst_cur, n, curEdgeToIgnore);
		cost = std::min( cost, newCost);
	}
	std::cout << cost << std::endl;

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
