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
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <set>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef std::map<Triangulation::Point_2, double> Dist_map;

double periode (double size){
	double dist = sqrt(size);
	if (dist <= 0.5){
		return 0;
	} else {
		return (int) std::ceil(std::sqrt(dist-0.5));
	}
}

void testcase(int n) {
	int l, b, r, t; std::cin >> l >> b >> r >> t;

	//read all points
	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (int i = 0; i < n; i++){
		int x, y; std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));

	}
	Triangulation triang;
	triang.insert(pts.begin(), pts.end());


	Dist_map distances;

	//Insert verteces to distmap with closest dish edge
	for (Vertex_iterator vert_it = triang.finite_vertices_begin(); vert_it != triang.finite_vertices_end();  vert_it++){
		int vert_x = vert_it->point().x();
		int vert_y = vert_it->point().y();
		double min_dist = (double) std::min(std::min(std::pow((vert_x-l), 2), std::pow((vert_x-r),2)), std::min(std::pow((vert_y-t),2), std::pow((vert_y-b),2)));
		distances.insert(std::make_pair(vert_it->point(), min_dist));
	}

	//Iterate over all edges and assign distance to each vertex
	for (Edge_iterator edge_it = triang.finite_edges_begin(); edge_it != triang.finite_edges_end(); edge_it++){
		Triangulation::Vertex_handle vert1 = edge_it->first->vertex((edge_it->second + 1) % 3);
		Triangulation::Vertex_handle vert2 = edge_it->first->vertex((edge_it->second + 2) % 3);
		Triangulation::Point_2 p1 = vert1->point();
		Triangulation::Point_2 p2 = vert2->point();

		double dist = CGAL::to_double(CGAL::squared_distance(p1, p2)) / 4;

		if (distances[p1] > dist){
			distances[p1] = dist;
		}
		if (distances[p2] > dist){
			distances[p2] = dist;
		}
	}

	std::vector<double> only_dist;
	only_dist.reserve(n);
	for (Dist_map::iterator dist_it = distances.begin(); dist_it != distances.end(); dist_it++){
		only_dist.push_back(dist_it->second);
	}
	std::sort(only_dist.begin(), only_dist.end(), std::less<double>());

	std::cout << periode(only_dist[0]) << " " << periode(only_dist[n/2]) << " " << periode(only_dist[n-1]) << std::endl;

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int n; std::cin >> n;
	while (n != 0){
		testcase(n);
		std::cin >> n;
	}
	return 0;
}
