#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

bool point_in_map(const std::vector<K::Point_2> &part, const K::Point_2 &p){
	bool in_map = false;
	if (!CGAL::right_turn(part[0], part[1], p) && !CGAL::right_turn(part[2], part[3], p) && !CGAL::right_turn(part[4], part[5], p)){
		in_map = true;
	}
	return in_map;
}

void testcase() {
	int m, n; std::cin >> m >> n;

	//Read in path
	std::vector<K::Point_2> path;
	for (int i = 0; i < m; i++){
		long x, y; std::cin >> x >> y;
		path.push_back(K::Point_2(x, y));
	}

	//Read in map parts
	std::vector<std::vector<K::Point_2>> map_parts;
	for (int i = 0; i < n; i++){
		std::vector<K::Point_2> map;
		for (int j = 0; j < 6; j++){
			long x, y; std::cin >> x >> y;
			map.push_back(K::Point_2(x, y));
		}
		//Orientation check:
		for (int j = 0; j < 6; j+= 2){
			int comp = (j+2) % 6;
			if (!CGAL::left_turn(map[j], map[j+1], map[comp])){
				std::swap(map[j], map[j+1]);
			}
		}
		map_parts.push_back(map);
	}

	//Create vector which tracks legs in map parts
	std::vector<std::vector<int>> legs_in_map(n);
	for (int i = 0; i < n; i++){
		std::vector<K::Point_2> cur_map_part = map_parts[i];

		for (int j = 0; j < m-1; j++){
			int legNr = j;
			K::Point_2 p1 = path[j];
			K::Point_2 p2 = path[j+1];
			if (point_in_map(cur_map_part, p1) && point_in_map(cur_map_part, p2)){
				legs_in_map[i].push_back(j);
			}
		}
	}

	//Sliding window over legs in map
	std::vector<int> leg_tracker(m-1);
	std::set<int> cur_legs_covered;
	int l = 0; int r = 0;
	int best_interval = std::numeric_limits<int>::max();
	while (r < n){
		
		//Increase r until path covered
		while (cur_legs_covered.size() < (m-1) && r < n){
			//First add all legs at r to set 
			for (int i = 0; i < legs_in_map[r].size(); i++){
				int curLegNr = legs_in_map[r][i];
				leg_tracker[curLegNr] ++;
				cur_legs_covered.insert(curLegNr);
			}
			r++;
		}

		//Increase l until path not covered anymore
		while (cur_legs_covered.size() == (m-1)){
			//Remove map parts from leg-tracker, if last map part removed, remove map-part from set
			for (int i = 0; i < legs_in_map[l].size(); i++){
				int curLegNr = legs_in_map[l][i];
				leg_tracker[curLegNr] --;
				if (leg_tracker[curLegNr] == 0){
					cur_legs_covered.erase(cur_legs_covered.find(curLegNr));
				}
			}
			l++;
		}

		//Check if new best found
		if (r-l+1 < best_interval){
			best_interval = r-l+1;
		}
	}
	std::cout << best_interval << std::endl;
	
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
