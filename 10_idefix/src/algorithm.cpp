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
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>					graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges


int nrBones (Delaunay t, long n, long s, std::vector<std::pair<int, long>> &bones_locations){
	// For every edge whose distance is smaller s, we add an edge to a new bgl graph
	graph G(n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		K::Point_2 p1 = e->first->vertex((e->second+1)%3)->point();
		K::Point_2 p2 = e->first->vertex((e->second+2)%3)->point();
		auto dist = CGAL::to_double(CGAL::squared_distance(p1, p2));
		if (CGAL::squared_distance(p1, p2) <= s){
			boost::add_edge(i1, i2, G);
		}
	}

	// We run connected components on the graph, in the components map we will have the nr. of the component, node i belongs to
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G)));

	std::vector<int> trees_with_bones(n, 0); //Tree i has j bones with smaller r: trees_with_bones[i] = j
	for (int i = 0; i < bones_locations.size(); i++){
		Index node_nr = bones_locations[i].first;
		auto dist = bones_locations[i].second;
		long radius = s/4;
		if (dist <= radius){
			trees_with_bones[node_nr] += 1;
		}
	}

	std::vector<int> component_nr_bones(ncc, 0);
	for(int i = 0; i < n; i++){
		int curComponent = component_map[i];
		int nrBonesAt_tree_i = trees_with_bones[i];
		component_nr_bones[curComponent] += nrBonesAt_tree_i;
	}

	int curBestComponent = 0;
	for (int i = 0; i < ncc; i++){
		if (component_nr_bones[i] > curBestComponent){
			curBestComponent = component_nr_bones[i];
		}
	}
	return curBestComponent;
}

void testcase() {
	long n, m, s, k; std::cin >> n >> m >> s >> k;

	//Read in all oak trees and build delaunay triangulation
	typedef std::pair<K::Point_2,Index> IPoint;
	std::vector<IPoint> points;
	points.reserve(n);
	for (Index i = 0; i < n; ++i) {
		int x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}
	Delaunay t;
	t.insert(points.begin(), points.end());

	// For every edge whose distance is smaller s, we add an edge to a new bgl graph
	graph G(n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		K::Point_2 p1 = e->first->vertex((e->second+1)%3)->point();
		K::Point_2 p2 = e->first->vertex((e->second+2)%3)->point();
		auto dist = CGAL::to_double(CGAL::squared_distance(p1, p2));
		if (CGAL::squared_distance(p1, p2) <= s){
			boost::add_edge(i1, i2, G);
		}
	}

	// We run connected components on the graph, in the components map we will have the nr. of the component, node i belongs to
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G)));

	std::vector<std::pair<int, long>> bones_locations; //Bone i is closest to tree j with dist k: bones_locations[i]=(j,k)
	std::vector<int> trees_with_bones(n, 0); //Tree i has j bones with smaller r: trees_with_bones[i] = j
	for (int i = 0; i < m; i++){
		int x, y; std::cin >> x >> y;
		K::Point_2 curBone = K::Point_2(x, y);

		auto nearestVertex = t.nearest_vertex(curBone);
		Index node_nr = nearestVertex->info();
		K::Point_2 pt = nearestVertex->point();
		auto dista = CGAL::to_double(CGAL::squared_distance(curBone, pt));
		bones_locations.push_back(std::make_pair(node_nr, CGAL::squared_distance(curBone, pt)));
		long radius = s/4;
		if (CGAL::squared_distance(curBone, pt) <= radius){
			trees_with_bones[node_nr] += 1;
		}
	}

	std::vector<int> component_nr_bones(ncc, 0);
	for(int i = 0; i < n; i++){
		int curComponent = component_map[i];
		int nrBonesAt_tree_i = trees_with_bones[i];
		component_nr_bones[curComponent] += nrBonesAt_tree_i;
	}

	int curBestComponent = 0;
	for (int i = 0; i < ncc; i++){
		if (component_nr_bones[i] > curBestComponent){
			curBestComponent = component_nr_bones[i];
		}
	}


	//Do binsearch on the radius
	long left = 0; long right = std::pow(2, 51);
	while (left < right){
		long middle = left + (right-left)/2;

		int best = nrBones(t, n, middle, bones_locations);
		if (best >= k){
			right = middle;
		} else {
			left = middle+1;
		}

	}

	std::cout << curBestComponent << " " << right << std::endl;
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
