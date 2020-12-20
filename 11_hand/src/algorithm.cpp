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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<Index,Index,K::FT> 						   Edge;
typedef std::vector<Edge> 									   EdgeV;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>		graph;
typedef boost::graph_traits<graph>::vertex_descriptor		   vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator			   edge_it;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
	return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int nrFamilies(Delaunay &del, K::FT &givenSquaredDist, int n, int k){
	graph G(n);
	//First compute a graph containing all edges of delaunay with squared length >= s
	for (auto e = del.finite_edges_begin(); e != del.finite_edges_end(); ++e) {
    	Index i1 = e->first->vertex((e->second+1)%3)->info();
    	Index i2 = e->first->vertex((e->second+2)%3)->info();
		if (del.segment(e).squared_length() <= givenSquaredDist){
			boost::add_edge(i1, i2, G);
		}
  	}
	
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
	
	std::vector<std::vector<vertex_desc>> component_vertices(ncc);
	// Iterate over all vertices
	for (int i = 0; i < n; ++i)
		component_vertices[component_map[i]].push_back(i);

	// Iterate over all components
	int countNrFamilies = 0; //This is the answer of the second question
	for (int c_i = 0; c_i < ncc; ++c_i){
		if (component_vertices[c_i].size() >= k){
			countNrFamilies++;
		}
	}
	return countNrFamilies;
}

void testcase() {
	long n, k, f, s; std::cin >> n >> k >> f >> s;

	typedef std::pair<K::Point_2,Index> IPoint;
  	std::vector<IPoint> points;
  	points.reserve(n);
  	for (Index i = 0; i < n; ++i) {
		long x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}
	Delaunay t;
	t.insert(points.begin(), points.end());


	EdgeV edges;
  	edges.reserve(3*n); // there can be no more in a planar graph
  	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		// ensure smaller index comes first
    	if (i1 > i2) std::swap(i1, i2);
    	edges.emplace_back(i1, i2, t.segment(e).squared_length());
  	}
  	std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
		  return std::get<2>(e1) <std::get<2>(e2);
	  });

	int l = 0; int r = edges.size()-1;
	while (l < r){
		int mid = l + (r- l) / 2;
		K::FT curSquaredDist = std::get<2>(edges[mid]);
		int curNrFamilies = nrFamilies(t, curSquaredDist, n, k);
		if (curNrFamilies < f){
			r = mid-1;
		} else if (curNrFamilies >= f){
			l = mid;
		}
	}
	long q1 = std::get<2>(edges[l]);

	K::FT squaredDist = s;
	int q2 = nrFamilies(t, squaredDist, n, k);
	
	std::cout << q1 << " " << q2 << std::endl;
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
