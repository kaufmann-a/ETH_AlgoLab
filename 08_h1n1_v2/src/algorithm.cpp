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
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel 	K;
typedef std::size_t                                          	Index;
typedef CGAL::Triangulation_vertex_base_2<K>   					Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double, K>	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>          	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                	Delaunay;

void testcase(size_t n) {

	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (std::size_t i = 0; i < n; i++){
		int x, y; std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
	}
	Delaunay t;
	t.insert(pts.begin(), pts.end());

	std::priority_queue<std::pair<double, Delaunay::Face_handle>> pq;
	for (auto it = t.all_faces_begin(); it != t.all_faces_end(); it++){
		if (t.is_infinite(it)){
			it->info() = std::numeric_limits<double>::max();
			pq.push(std::make_pair(it->info(), it));
		} else {
			it->info() = 0;
		}
	}

	while (!pq.empty()){
		double cur_face_max_escape = pq.top().first;
		Delaunay::Face_handle cur_face = pq.top().second; 
		pq.pop();
		if (cur_face_max_escape < cur_face->info()){
			continue;
		} else {
			for (int i = 0; i < 3; i++){
				Delaunay::Face_handle cur_neighbour = cur_face->neighbor(i);
				if (!t.is_infinite(cur_neighbour)){
					auto min = std::min(cur_face->info(), t.segment(cur_face, i).squared_length());
					if (min > cur_neighbour->info()){
						cur_neighbour->info() = min;
						pq.push(std::make_pair(cur_neighbour->info(), cur_neighbour));
					}
				}
			}
		}
	}

	size_t m; std::cin >> m;
	for (size_t i = 0; i < m; i++){
		int x, y; long d; std::cin >> x >> y >> d;
		K::Point_2 cur_escapee_point = K::Point_2(x, y);
		Delaunay::Face_handle cur_escapee_face = t.locate(cur_escapee_point);
		K::Point_2 cur_escapee_closest_vertex = t.nearest_vertex(cur_escapee_point, cur_escapee_face)->point();
		if (CGAL::squared_distance(cur_escapee_point, cur_escapee_closest_vertex) >= d){
			if (cur_escapee_face->info() >= d*4){
				std::cout << "y";
			} else {
				std::cout << "n";
			}
		} else {
			std::cout << "n";
		}
	}
	std::cout << std::endl;


	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	size_t n; std::cin >> n;
	while (n != 0){
		testcase(n);
		std::cin >> n;
	}
	return 0;
}
