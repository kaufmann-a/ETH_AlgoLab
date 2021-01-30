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

bool componentSort(const int &comp1, const int &comp2){
	return comp1 > comp2;
}

int q2(const EdgeV &edges, int k, long s, int n){

	std::vector<int> components(n, 1); //Initialize all components with size 1

	boost::disjoint_sets_with_storage<> uf(n);
  	Index n_components = n;
  	// ... and process edges in order of increasing length
  	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		
		//First check if cur edge-size to large already
		if (std::get<2>(*e) >= s){
			break;
		}
		// determine components of endpoints
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);

			//Find name of new component
			int parent = uf.find_set(c1);
			if(parent == c1){
				components[parent] += components[c2];
				components[c2] = 0;
			} else if (parent == c2){
				components[parent] += components[c1];
				components[c1] = 0;
			}
			if (--n_components == 1) break;
		}
  	}

	//We know nr of components is n_components, we sort components with largest components on top
	std::sort(components.begin(), components.end(), componentSort);

	//get nr. families
	int counter = 0;
	int l = 0; int r = n_components-1;
	while (l <= r){
		//First process all components larger k
		while (components[l] >= k && l <= r){
			counter++;
			l++;
		}

		if (l == r){
			break;
		}

		while(components[l] < k && r > l){
			components[l] += components[r];
			r--;
		}
	}
	return counter;
}

void testcase() {
	int n, k, f; long s; std::cin >> n >> k >> f >> s;

	// read points: first, we read all points and store them into a vector,
	// together with their indices
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

	EdgeV edges;
  	edges.reserve(3*n); // there can be no more in a planar graph
  	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		// ensure smaller index comes first
		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, t.segment(e).squared_length());	
  	}
  	std::sort(edges.begin(), edges.end(),
	    [](const Edge& e1, const Edge& e2) -> bool {
	    	return std::get<2>(e1) < std::get<2>(e2);
        		});

	int nrFamilies_q2 = q2(edges, k, s, n);

	//For question 1 do binsearch on all possible s, we have to assign f families

	int l = 0; int r = edges.size()-1;
	long curBest = std::get<2>(edges[0]);
	while (l < r){
		int mid = l + (r-l)/2;
		long cur_s = std::get<2>(edges[mid]);

		int nrFamilies_with_cur_s = q2(edges, k, cur_s, n);

		if (nrFamilies_with_cur_s >= f){
			curBest = cur_s;
			l = mid +1;
		} else {
			r = mid;
		}
	}
	long size_s_q1 = curBest;

	std::cout << size_s_q1 << " " << nrFamilies_q2 << std::endl;
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
