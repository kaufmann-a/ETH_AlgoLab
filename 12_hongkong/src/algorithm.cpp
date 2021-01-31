#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel 		K;
typedef std::size_t                                           	Index;
typedef CGAL::Triangulation_vertex_base_2<K>   		   		  	Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K>   	Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>           	Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                 	Delaunay;

void testcase() {
	size_t n, m; long rad; std::cin >> n >> m >> rad;
	K::FT r = rad;

	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (std::size_t i = 0; i < n; ++i) {
		long x, y;
		std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
	}
	// construct triangulation
	Delaunay t;
	t.insert(pts.begin(), pts.end());

	std::priority_queue<std::pair<K::FT, Delaunay::Face_handle>> prio_queue;
	for (auto it = t.all_faces_begin(); it != t.all_faces_end(); it++){
		//First find clearance
		if (t.is_infinite(it)){
			it->info() = std::numeric_limits<double>::max();
			prio_queue.push(std::make_pair(std::numeric_limits<double>::max(), it));
		} else {
			K::Point_2 voronoy = t.dual(it);
			K::Point_2 anyVertex = it->vertex(0)->point();

			K::FT clearance = CGAL::squared_distance(voronoy, anyVertex);
			it->info() = clearance;
			prio_queue.push(std::make_pair(clearance, it));
		}
	}

	//Greedy optimization of takeoff clearances
	while(!prio_queue.empty()){
		auto cur_face = prio_queue.top(); prio_queue.pop();
		K::FT clearance_cur_face = cur_face.first;
		Delaunay::Face_handle cur_face_handle = cur_face.second;
		
		if(clearance_cur_face < cur_face_handle->info()){ //This is the case in which face was already assigned a max clearance
			continue;
		} else {
			for(int i = 0; i < 3; i++){
				auto cur_neighbour = cur_face_handle->neighbor(i);
				if (!t.is_infinite(cur_neighbour)){
					K::FT edge_length_cur_neighbour = t.segment(cur_face_handle, i).squared_length();
					K::FT min = std::min(edge_length_cur_neighbour, cur_face_handle->info());

					if (min > cur_neighbour->info()){
						cur_neighbour->info() = min;
						prio_queue.push(std::make_pair(min, cur_neighbour));
					}
				}
			}
		}
	}

	//Read in baloons
	for (size_t i = 0; i < m; i++){
		long x, y, ss; std::cin >> x >> y >> ss;
		K::FT s = ss;
		K::Point_2 cur_bal = K::Point_2(x, y);
		
		//First check if balon can be inflated
		auto face_of_cur_bal = t.locate(cur_bal);
		K::Point_2 nearest_tree = t.nearest_vertex(cur_bal, face_of_cur_bal)->point();
		if (CGAL::squared_distance(cur_bal, nearest_tree) >= CGAL::square(r+s)){
			
			K::FT clearance = face_of_cur_bal->info();
			K::FT needed_clearance = CGAL::square(2*r+2*s);
			if (clearance >= needed_clearance){
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
	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
