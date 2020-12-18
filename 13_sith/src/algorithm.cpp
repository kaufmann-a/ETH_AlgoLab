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
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            	Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   	Vb;
typedef CGAL::Triangulation_face_base_2<K>                     	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  	Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef Delaunay::Edge_iterator								   	Edge_iterator;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>	graph;
typedef boost::graph_traits<graph>::vertex_descriptor			vertex_desc;			
typedef boost::graph_traits<graph>::edge_iterator				edge_it;		

void testcase() {
	int n; long r; std::cin >> n >> r;

	// read points
	typedef std::pair<K::Point_2,Index> IPoint;
  	std::vector<IPoint> points;
  	points.reserve(n);
  	for (Index i = 0; i < n; ++i) {
		int x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
  	}
	int counter = 1;
	std::vector<IPoint>::iterator it = points.begin();
	it++;
	int k = 0;
	while (it != points.end()){
		// construct triangulation
		Delaunay t;
		t.insert(it, points.end());
		
		graph G(n-counter);
		//construct boostgraph
		for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
			if (t.segment(e).squared_length() <= r*r) {
				Index i1 = e->first->vertex((e->second+1)%3)->info()-counter;
    			Index i2 = e->first->vertex((e->second+2)%3)->info()-counter;
				boost::add_edge(i1, i2, G);
			}
		}
		std::vector<int> component_map(n-counter);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
		int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
	
		std::vector<std::vector<vertex_desc>> component_vertices(ncc);
		// Iterate over all vertices
		for (int i = 0; i < n-counter; ++i){
			component_vertices[component_map[i]].push_back(i);
		}
		int maxSize = 0;
		for (int c_i = 0; c_i < ncc; ++c_i){
			if (component_vertices[c_i].size() > maxSize){
				maxSize = component_vertices[c_i].size();
			}
		}
		if (maxSize >= counter){
			k = counter;
		} else {
			break;
		}
		counter++;
		it++;
	}
	std::cout << k << std::endl;
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
