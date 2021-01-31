#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <set>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

int nrBones(EdgeV &edges, std::vector<std::multiset<long>> bones_to_tree, long s){
	//First build component array
	int nrTrees = bones_to_tree.size();
	int curMaxCompSize = 0;
	std::vector<int> components(nrTrees);
	for (int i = 0; i < nrTrees; i++){
		for (auto it = bones_to_tree[i].begin(); it != bones_to_tree[i].end(); it++){
			long curDist = *it;
			if (curDist <= s){
				components[i]++;
				if (components[i] > curMaxCompSize){
					curMaxCompSize = components[i];
				}
			} else {
				break;
			}
		}
	}
	// setup and initialize union-find data structure
	boost::disjoint_sets_with_storage<> uf(nrTrees);
	Index n_components = nrTrees;
	// ... and process edges in order of increasing length
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		if (std::get<2>(*e) > s){ //Stop building emst
			break;
		}
		
		// determine components of endpoints
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);
			int bonesInComp = components[c1] + components[c2];
			Index parent = uf.find_set(c1);
			if (parent == c1){
				components[c1] = bonesInComp; components[c2] = 0;
			} else {
				components[c2] = bonesInComp; components[c1] = 0;
			}
			if (bonesInComp > curMaxCompSize){
				curMaxCompSize = bonesInComp;
			}
			if (--n_components == 1) break;
		}
	}
	return curMaxCompSize;
}


void testcase() {
	int n, m; long s; int k; std::cin >> n >> m >> s >> k;

	typedef std::pair<K::Point_2,Index> IPoint;
	std::vector<IPoint> points;
	points.reserve(n);
	for (uint i = 0; i < n; ++i) {
		long x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}
	Delaunay t;
	t.insert(points.begin(), points.end());

	//Read in bones
	std::vector<std::multiset<long>> bones_to_tree(n);
	std::vector<long> distances_bin_search;
	for (int i = 0; i < m; i++){
		long x, y; std::cin >> x >> y;
		K::Point_2 bone = K::Point_2(x, y);
		auto closest_tree_vertex = t.nearest_vertex(bone);
		Index closest_tree_index = closest_tree_vertex->info();
		K::Point_2 closest_tree_point = closest_tree_vertex->point();
		long squared_dist = CGAL::squared_distance(bone, closest_tree_point)*4;
		bones_to_tree[closest_tree_index].insert(squared_dist);
		distances_bin_search.push_back(squared_dist);
	}


	// extract edges and sort by (squared) length
	// This step takes O(n log n) time (for the sorting).
	EdgeV edges;
	edges.reserve(3*n); // there can be no more in a planar graph
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		// ensure smaller index comes first
		if (i1 > i2) std::swap(i1, i2);
		auto sq_length = t.segment(e).squared_length();
		edges.emplace_back(i1, i2, sq_length);
		distances_bin_search.push_back(sq_length);
	}
	std::sort(edges.begin(), edges.end(),
	[](const Edge& e1, const Edge& e2) -> bool {
		return std::get<2>(e1) < std::get<2>(e2);
		});

	std::sort(distances_bin_search.begin(), distances_bin_search.end());
	
	int answer_q1 = nrBones(edges, bones_to_tree, s);

	//Do binsearch on distances array
	int l = 0; int r = distances_bin_search.size()-1;
	long smallest_r = std::numeric_limits<long>::max();
	while (l < r){
		int mid = l + (r-l)/2;
		long cur_dist = distances_bin_search[mid];
		int curNrBones = nrBones(edges, bones_to_tree, cur_dist);
		if (curNrBones < k){
			l = mid+1;
		} else {
			r = mid;
			smallest_r = cur_dist;
		}
	}
	long answer_q2 = smallest_r;

	std::cout << answer_q1 << " " << answer_q2 << std::endl;

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
