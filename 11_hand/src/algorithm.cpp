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
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

bool mySort(std::pair<K::Point_2, long> &p1, std::pair<K::Point_2, long> &p2){
	return p1.second < p2.second;
}

void testcase() {
	int n, k, f; double s; std::cin >> n >> k >> f >> s;

	typedef std::pair<K::Point_2,Index> IPoint;
	std::vector<IPoint> tents;
	tents.reserve(n);

	for (int i = 0; i < n; i++){
		int x, y; std::cin >> x >> y;
		tents.push_back(std::make_pair(K::Point_2(x, y), i));
	}
	Delaunay t;
	t.insert(tents.begin(), tents.end());

	double smallestDist = std::numeric_limits<double>::max(); 
	graph G(n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    	Index i1 = e->first->vertex((e->second+1)%3)->info();
   		Index i2 = e->first->vertex((e->second+2)%3)->info();
		double dist = CGAL::to_double(t.segment(e).squared_length()); 
		if (dist < smallestDist) {
			smallestDist = dist;
		}
		if (dist < s){
			boost::add_edge(i1, i2, G);
		}
  	}
	
	std::vector<vertex_desc> mate_map(n);  // exterior property map
  	const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
  	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  	int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

	std::cout << smallestDist << " " << n-matching_size << std::endl;
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
