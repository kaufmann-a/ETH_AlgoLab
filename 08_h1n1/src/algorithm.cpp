#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <map>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;
typedef Triangulation::Finite_edges_iterator  Edge_iterator;
typedef Triangulation::Finite_vertices_iterator Vertex_iterator;

struct sortingSet {
	bool operator() (const std::pair<Triangulation::Face_handle, K::FT> face1, const std::pair<Triangulation::Face_handle, K::FT> face2) const {
		return face1.second > face2.second;
	}
};

void testcase(int n) {
	
	std::vector<K::Point_2> pts;
	pts.reserve(n);
	for (int i = 0; i < n; i++){
		int x, y; std::cin >> x >> y;
		pts.push_back(K::Point_2(x, y));
	}
	Triangulation tr;
	tr.insert(pts.begin(), pts.end());

	std::multiset<std::pair<Triangulation::Face_handle, K::FT>, sortingSet> priority_stack;
	//Add all which are at border to the stack, with the segment-length connected to the outside space
	for (Triangulation::All_faces_iterator fc_it = tr.all_faces_begin(); fc_it != tr.all_faces_end(); fc_it++){
		if (tr.is_infinite(fc_it)){
			for (int i = 0; i < 3; i++){
				if (!tr.is_infinite(fc_it->neighbor(i))){
					priority_stack.insert(std::make_pair(fc_it->neighbor(i), tr.segment(fc_it, i).squared_length()));
				}
			}
		}
		fc_it->info() = -1; //Set info to -1 for every triangle, to indicate that max not yet found
	}

	//Following for each triangle the max size is calculated, we use the priority stack to find the max distance
	while (!priority_stack.empty()){
		std::pair<Triangulation::Face_handle, K::FT> topFace = *priority_stack.begin(); //Pop first element from stack
		priority_stack.erase(priority_stack.begin()); //Delete top element from stack
		if (topFace.first->info() != -1){ //If info already contains value, max value for this triangle already found, skip it
			continue;
		}
		topFace.first->info() = topFace.second; //The second element of the pair is the largest distance we can do for this triangle

		for (int i = 0; i < 3; i++){ //add neighbours to stack
			if (topFace.first->neighbor(i)->info() == -1 && !tr.is_infinite(topFace.first->neighbor(i))){
				priority_stack.insert(std::make_pair(topFace.first->neighbor(i), std::min(tr.segment(topFace.first, i).squared_length(), topFace.second)));
			}
		}
	}

	int m; std::cin >> m;
	for (int i = 0; i < m; i++){
		long x, y, d; std::cin >> x >> y >> d;
		K::Point_2 curPt = K::Point_2(x, y);

		//First, check if already to close to a point
		K::Point_2 closest = tr.nearest_vertex(curPt) -> point();
		long curDist = (long) CGAL::squared_distance(curPt, closest);
		if (curDist < d){
			std::cout << "n";
			continue;
		}

		Triangulation::Face_handle curTriangle = tr.locate(curPt);
		if (tr.is_infinite(curTriangle) || curTriangle->info() >= 4*d){
			std::cout << "y";
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

	int n; std::cin >> n;
	while(n != 0){
		testcase(n);
		std::cin >> n;
	}
	return 0;
}