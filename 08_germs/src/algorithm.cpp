#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel 	K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EKS;
typedef std::size_t                                            	Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   	Vb;
typedef CGAL::Triangulation_face_base_2<K>                     	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  	Delaunay;
typedef std::tuple<Index,Index,K::FT> 							Edge;
typedef std::vector<Edge> 										EdgeV;

double ceil_to_double(const EKS::FT &x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

long answer(K::FT curSquaredDist){
	EKS::FT exact_squared_dist = curSquaredDist;
	exact_squared_dist = exact_squared_dist/4;
	exact_squared_dist = CGAL::sqrt(exact_squared_dist);
	exact_squared_dist -= 0.5;
	exact_squared_dist = CGAL::sqrt(exact_squared_dist);
	double dist = ceil_to_double(exact_squared_dist);
	long long_dist = dist;
	return long_dist;
}

void testcase(int n) {
	int l, b, r, t; std::cin >> l >> b >> r >> t;

	std::vector<K::Point_2> points;
	for (Index i = 0; i < n; ++i) {
		int x, y;
		std::cin >> x >> y;
		points.push_back(K::Point_2(x, y));
	}
	Delaunay tr;
	tr.insert(points.begin(), points.end());

	//First assign every vertex squared dist to closest dish edge times 4
	for (auto it = tr.finite_vertices_begin(); it != tr.finite_vertices_end(); it++){
		K::FT x = it->point().x(); K::FT y = it->point().y();
		it->info() = std::min(std::min(std::min(CGAL::square(x-l)*4, CGAL::square(x-r)*4), CGAL::square(y-t)*4), CGAL::square(y-b)*4);
	}

	//Iterate over edges
	for (auto it = tr.finite_edges_begin(); it != tr.finite_edges_end(); it++){
		Delaunay::Vertex_handle v1 = it->first->vertex((it->second+1)%3);
		Delaunay::Vertex_handle v2 = it->first->vertex((it->second+2)%3);
		if (tr.segment(it).squared_length() < v1->info()) v1->info() = tr.segment(it).squared_length();
		if (tr.segment(it).squared_length() < v2->info()) v2->info() = tr.segment(it).squared_length();
	}

	std::vector<K::FT> lengths;
	for (auto it = tr.finite_vertices_begin(); it != tr.finite_vertices_end(); it++){
		lengths.push_back(it->info());
	}
	std::sort(lengths.begin(), lengths.end());

	std::cout << answer(lengths[0]) << " " << answer(lengths[n/2]) << " " << answer(lengths[n-1]) << std::endl;;

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int n;
	std::cin >> n;
	while (n != 0){
		testcase(n);
		std::cin >> n;
	}
	return 0;
}
