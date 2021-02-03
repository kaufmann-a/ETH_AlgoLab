#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/pending/disjoint_sets.hpp>

struct Edge{
	int u;
	int v; 
	int weight;

	bool operator <(const Edge &other) const{
		return weight < other.weight;
	}
};

int calc_mst(std::vector<Edge> &edges, std::vector<int> &to_ignore, size_t nr_to_ignore, bool fill_ignore_vec, size_t n){
	// setup and initialize union-find data structure
	boost::disjoint_sets_with_storage<> uf(n);
	size_t n_components = n;
	size_t cur_sum_edge_weights = 0;
	for (size_t i = 0; i < edges.size(); i++){
		if (i != nr_to_ignore){
			Edge cur_edge = edges[i];
			int c1 = uf.find_set(cur_edge.u);
			int c2 = uf.find_set(cur_edge.v);
			if (c1 != c2){
				uf.link(c1, c2);
				cur_sum_edge_weights += cur_edge.weight;
				if (fill_ignore_vec){
					to_ignore.push_back(i);
				}
				if (--n_components == 1) break;
			}
		}
	}
	return cur_sum_edge_weights;
}


void testcase() {
	size_t n, s; std::cin >> n >> s;

	std::vector<Edge> edges;
	for (size_t j = 1; j <= n-1; j++){
		for (size_t k = 1; k <= n-j; k++){
			int u = j-1; int v = j+k-1; int weight; std::cin >> weight;
			Edge e = {}; e.u = u; e.v = v; e.weight = weight;
			edges.push_back(e);
		}
	}
	std::sort(edges.begin(), edges.end());

	std::vector<int> to_ignore;
	calc_mst(edges, to_ignore, edges.size(), true, n);

	int second_best = std::numeric_limits<int>::max();
	for (size_t i = 0; i < to_ignore.size(); i++){
		int next = calc_mst(edges, to_ignore, to_ignore[i], false, n);
		if (next < second_best){
			second_best = next;
		}
	}
	std::cout << second_best << std::endl;
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
