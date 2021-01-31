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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;


long find_biggest_component(const Delaunay &t, const long &r, long diff){
	// extract edges and sort by (squared) length
	// This step takes O(n log n) time (for the sorting).
	EdgeV edges;
	edges.reserve(3*t.number_of_vertices()); // there can be no more in a planar graph
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info()-diff;
		Index i2 = e->first->vertex((e->second+2)%3)->info()-diff;
		// ensure smaller index comes first
		if (t.segment(e).squared_length() <= r*r){
			if (i1 > i2) std::swap(i1, i2);
			edges.emplace_back(i1, i2, t.segment(e).squared_length());
		}
	}
	std::sort(edges.begin(), edges.end(),
			[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
				});
	
	// setup and initialize union-find data structure
	boost::disjoint_sets_with_storage<> uf(t.number_of_vertices());
	std::vector<long> components(t.number_of_vertices(), 1);
	long curMaxComp = 1;
	Index n_components = t.number_of_vertices();
	// ... and process edges in order of increasing length
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		// determine components of endpoints
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);
			long newCompSize = components[c1] + components[c2];
			if (uf.find_set(c1) == c1){
				components[c1] = newCompSize; components[c2] = 0;
			} else {
				components[c2] = newCompSize; components[c1] = 0;
			}
			if (newCompSize > curMaxComp){
				curMaxComp = newCompSize;
			}
			if (--n_components == 1) break;
		}
	}
	return curMaxComp;
}

void testcase() {
	long n, r; std::cin >> n >> r;

	typedef std::pair<K::Point_2,Index> IPoint;
	std::vector<IPoint> points;
	points.reserve(n);
	for (long i = 0; i < n; ++i) {
		long x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}

	//Do binsearch on k, k can range from 1 to n/2
	long left = 0; long right = n/2+1;
	long curBest = 0;
	while (left < right){
		long mid = left + (right-left)/2;
		Delaunay t;
  		t.insert(points.begin()+mid, points.end());
		long maxComp = std::min(find_biggest_component(t, r, mid), mid);
		if (maxComp >= mid){
			if (mid > curBest){
				curBest = mid;
			}
			left = mid+1;
		} else {
			right = mid;
		}
	}
	std::cout << curBest << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test4.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
