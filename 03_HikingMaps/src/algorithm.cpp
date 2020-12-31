#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

bool checkInCluded(std::vector<IK::Point_2> &tr, IK::Point_2 &pt){
	bool segmentIncluded = true;
	for (int side = 0; side < 5; side += 2){
		//Check orientation of points on side
		IK::Point_2 p1 = tr[side];
		IK::Point_2 p2 = tr[side+1];
		IK::Point_2 q = tr[(side+2)%6];
		bool leftTurn = CGAL::left_turn(p1, p2, q);
		if (leftTurn){
			bool incl = !(CGAL::right_turn(p1, p2, pt));
			if (!incl){
				segmentIncluded = false;
			}
		} else {
			bool incl = !(CGAL::right_turn(p2, p1, pt));
			if (!incl){
				segmentIncluded = false;
			}
		}
	}
	return segmentIncluded;
}

bool contains(const std::vector<IK::Point_2> &t, const IK::Point_2 &p){
  return !CGAL::right_turn(t[0], t[1], p) && !CGAL::right_turn(t[2], t[3], p) && !CGAL::right_turn(t[4], t[5], p);
}

void testcase() {
	int m; std::cin >> m;
	int n; std::cin >> n;
	//std::cout << "m: " << m << " n: " << n << std::endl;

	//Read in all paths
	std::vector<IK::Point_2> points;
	for (int i = 0; i < m; i++){
		int x; std::cin >> x;
		int y; std::cin >> y;
		points.push_back(IK::Point_2(x, y));
	}

	//Read in all map parts
	std::vector<std::vector<int>> map_nr_contains_paths_nr(n, std::vector<int>());
	for (int i = 0; i < n; i++){
		std::vector<IK::Point_2> triangle;
		for (int j = 0; j < 3; j++){
			int a, b, c, d; std::cin >> a; std::cin >> b; std::cin >> c; std::cin >> d;
			triangle.push_back(IK::Point_2(a, b));
			triangle.push_back(IK::Point_2(c, d));
		}
		// for(int j = 0; j < 6; j+=2){
		// 	if(CGAL::right_turn(triangle[j],triangle[j+1], triangle[(j+2)%6])) std::swap(triangle[j], triangle[j+1]);
		// }

		// for(int j = 0; j < m - 1; j++){
      	// 	if(contains(triangle, points[j]) && contains(triangle, points[(j + 1) % m])){
		// 		map_nr_contains_paths_nr[i].push_back(j);
		// 	}
		// }

				//Check out all legs if it is contained in map
		for (int j = 0; j < m-1; j++){
			bool segmentIncluded = true;
			IK::Point_2 segStart = points[j];
			IK::Point_2 segEnd = points[j+1];
			for (int side = 0; side < 5; side += 2){
				//Check orientation of points on side
				IK::Point_2 p1 = triangle[side];
				IK::Point_2 p2 = triangle[side+1];
				IK::Point_2 q = triangle[(side+2)%6];
				bool leftTurn = CGAL::left_turn(p1, p2, q);
				if (leftTurn){
					bool start = !(CGAL::right_turn(p1, p2, segStart));
					bool end = !(CGAL::right_turn(p1, p2, segEnd));
					if (!(start && end)){
						segmentIncluded = false;
					}
				} else {
					bool start = !(CGAL::right_turn(p2, p1, segStart));
					bool end = !(CGAL::right_turn(p2, p1, segEnd));
					if (!(start && end)){
						segmentIncluded = false;
					}
				}
			}
			if (segmentIncluded){
				map_nr_contains_paths_nr[i].push_back(j);
			}
		}


		//Check out all legs if it is contained in map
		bool first = checkInCluded(triangle, points[0]);
		for (int j = 1; j < m; j++){
			
			bool second = checkInCluded(triangle, points[j]);
			if (first && second){
				map_nr_contains_paths_nr[i].push_back(j-1);
			}
			first = second;
		}
	}

	int left, right;
	std::vector<int> segments_map_count(m-1);
	int curPathLength = 0;
	int bestInterval = n-1;
	while (right < n){
		while (curPathLength < (m-1) && right < n){
			for (int i = 0; i < map_nr_contains_paths_nr[right].size(); i++){
				int curPathSegment = map_nr_contains_paths_nr[right][i];
				if (segments_map_count[curPathSegment] == 0) {
					curPathLength++;
				}
				segments_map_count[curPathSegment]++;
			}
			right++;
		}


		while(curPathLength >= (m-1) && left < right){
			//Check if new best inverval
			if ((right-left) < bestInterval){
				bestInterval = right-left;
			}
			for (int i = 0; i < map_nr_contains_paths_nr[left].size(); i++){
				int curPathSegment = map_nr_contains_paths_nr[left][i];
				segments_map_count[curPathSegment]--;
				if (segments_map_count[curPathSegment] == 0){
					curPathLength--;
				}
			}
			left++;
		}
	}

	std::cout << bestInterval << std::endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}