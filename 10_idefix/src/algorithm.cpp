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
#include <CGAL/boost/graph/graph_traits_Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Finite_vertex_handles::iterator Vertex_handles_iterator;
typedef std::map<Triangulation::Point_2, double> Dist_map;

long dfs (std::map<Triangulation::Point_2, bool> visited_map, const Triangulation &triang, const Triangulation::Vertex_handle &cur_v_handle, std::map<Triangulation::Vertex_handle, long> &nr_bones_at_tree, long s){
	if (visited_map[cur_v_handle->point()]){
		return 0;
	}

	visited_map[cur_v_handle->point()] = true;
	long nrBonesCurTree = nr_bones_at_tree[cur_v_handle];
	
	Triangulation::Edge_circulator c = triang.incident_edges(cur_v_handle);
	do {
		std::cout << "hallo" << std::endl;
		if (!triang.is_infinite(c)){
			auto p1 = c->first;
			auto p2 = c->second;
			// if (CGAL::squared_distance(, p2) <= s){
			// 	nrBonesCurTree += dfs(visited_map, triang, p2, nr_bones_at_tree, s);
			// }
		}
	} while (++c != triang.incident_edges(cur_v_handle));
	return nrBonesCurTree;
}




void testcase() {
	long n, m, s, k; std::cin >> n >> m >> s >> k;

	//read all trees
	std::vector<K::Point_2> trees;
	trees.reserve(n);
	for (int i = 0; i < n; i++){
		long x, y; std::cin >> x >> y;
		trees.push_back(K::Point_2(x, y));
	}
	Triangulation triang;
	triang.insert(trees.begin(), trees.end());

	//read all bones
	std::vector<K::Point_2> bones;
	bones.reserve(m);
	for (int i = 0; i < m; i++){
		long x, y; std::cin >> x >> y;
		bones.push_back(K::Point_2(x, y));
	}
	
	std::map<Triangulation::Vertex_handle, long> nr_bones_at_tree;
	double radius = s/4;
	for (int i = 0; i < m; i++){
		Triangulation::Vertex_handle nearestVertex = triang.nearest_vertex(bones[i]);
		double squaredDist = CGAL::squared_distance(nearestVertex->point(), bones[i]);
		if (squaredDist <= radius){
			std::map<Triangulation::Vertex_handle, long>::iterator it = nr_bones_at_tree.find(nearestVertex);
			if (it != nr_bones_at_tree.end()) {
				nr_bones_at_tree[nearestVertex] += 1;
			} else {
				nr_bones_at_tree.insert(std::make_pair(nearestVertex, 1));
			}
		}
	}

	//Do dfs over all verteces
	std::map<Triangulation::Point_2, bool> visited_map;
	for (Vertex_iterator v_it = triang.finite_vertices_begin(); v_it != triang.finite_vertices_end(); v_it++){
		visited_map.insert(std::make_pair(v_it->point(), false));
	}
	int nr_Bones = 0;
	for (Vertex_iterator v_it = triang.finite_vertices_begin(); v_it != triang.finite_vertices_end(); v_it++){
		long curNrBones = dfs(visited_map, triang, v_it, nr_bones_at_tree, s);
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
