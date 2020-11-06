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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n) {
	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (int i = 0; i < n; i++){
		int x, y; std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
	}
	Triangulation t;
	t.insert(pts.begin(), pts.end());
	
	int m; std::cin >> m;
	for (int i = 0; i < m; i++){
		int x, y; std::cin >> x >> y;
		K::Point_2 point = K::Point_2(x, y);
		Triangulation::Vertex_handle closest = t.nearest_vertex(point);
		K::Point_2 cls = closest -> point();
		std::cout << (long) CGAL::squared_distance(point, cls) << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test.in");
	std::cin.rdbuf(in.rdbuf());

	int n;
	std::cin >> n;
	while (std::cin && n != 0){
		testcase(n);
		std::cin >> n;
	}		
	return 0;
}
