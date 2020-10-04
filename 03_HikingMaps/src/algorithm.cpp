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

void testcase() {
	int m; std::cin >> m;
	int n; std::cin >> n;
	//std::cout << "m: " << m << " n: " << n << std::endl;

	//Read in all paths
	std::vector<IK::Segment_2> segments(m-1);
	int x; std::cin >> x;
	int y; std::cin >> y;
	IK::Point_2 start(x, y);
	for (int i = 0; i < m-1; i++){
		int x_; std::cin >> x_;
		int y_; std::cin >> y_;
		IK::Point_2 end(x_, y_);

		IK::Segment_2 path(start, end);
		segments[i] = path;
		start = end;
	}

	//Read in all map parts
	std::vector<IK::Triangle_2> mapParts(n);
	for (int i = 0; i < n; i++){
		std::vector<IK::Line_2> threeLines(3);
		for (int j = 0; j < 3; j++){
			int a, b, c, d; std::cin >> a; std::cin >> b; std::cin >> c; std::cin >> d;
			threeLines[j] = IK::Line_2(IK::Point_2(a, b), IK::Point_2(c, d));
		}
		std::vector<IK::Point_2> corners(3);
		for (int j = 0; j < 3; j++){
			auto o = CGAL::intersection(threeLines[j], threeLines[(j + 1) % 3]);
			if (const IK::Point_2* op = boost::get<IK::Point_2>(&*o)){
				corners[j] = *op;
			}
		}
		IK::Triangle_2 map(corners[0], corners[1], corners[2]);
		std::cout << "corn1: " << corners[0].x() << " " << corners[0].y() << "corn2: " << corners[1].x() << " " << corners[1].y() << "corn3: " << corners[2].x() << " " << corners[2].y() <<  std::endl;
		mapParts[i] = map;
	}

	//Check which maps contain which paths
	std::vector<std::vector<int>> map_nr_contains_paths_nr(n);
	for (int i = 0; i < n; i++){ //Loop over every map part
		for (int j = 0; j < m-2; j++){
			auto o = CGAL::intersection(mapParts[i], segments[j]);
			if (const IK::Segment_2* op = boost::get<IK::Segment_2>(&*o)){
				std::cout << "source_op: " << op->source().x() << " " << op->source().y() << " target_op: " << op->target().x() << " " << op->target().y() << std::endl;
				std::cout << "source_seg: " << segments[j].source().x() << " " << segments[j].source().y() << " target_seg: " << segments[j].target().x() << " " << segments[j].target().y() << std::endl;
				
				if (op->source() == segments[j].source() && op->target() == segments[j].target()){
					map_nr_contains_paths_nr[i].push_back(j);
				}
			}
		}
	}

	//Sliding window
	int leftIdx = 0; int rightIdx = 0;
	int best_e = std::numeric_limits<int>::max(); int best_b = 0;
	int pathLength = 0;
	while (rightIdx < n && pathLength < m) {
		//Fill up vector curPathPartsCovered, which tracks the segments currently covered by maps
		std::vector<int> curPathPartsCovered(m-1);
		for (uint i = 0; i < map_nr_contains_paths_nr[rightIdx].size(); i++){
			if (curPathPartsCovered[map_nr_contains_paths_nr[rightIdx][i]] == 0){
				pathLength++;
			}
			curPathPartsCovered[map_nr_contains_paths_nr[rightIdx][i]] += 1;
		}
		
		if (pathLength == m-1){
			if (rightIdx - leftIdx < best_e - best_b){
				best_e = rightIdx;
				best_b = leftIdx;
			}
		}

		while (pathLength == m-1 && leftIdx <= rightIdx){
			for (uint i = 0; i < map_nr_contains_paths_nr[leftIdx].size(); i++){
				curPathPartsCovered[map_nr_contains_paths_nr[leftIdx][i]] -= 1;
				if (curPathPartsCovered[map_nr_contains_paths_nr[leftIdx][i]] == 0){
					pathLength--;
				} 
			}
			leftIdx++;
		}
		rightIdx++;
	}
	int cost = best_e - best_b;
	std::cout << cost << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
}