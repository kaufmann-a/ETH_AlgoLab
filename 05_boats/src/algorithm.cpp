#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

bool mysort(std::pair<int, int> &p1, std::pair<int, int> &p2){
	return p1.first < p2.first;
}

void testcase() {
	int n; std::cin >> n;

	std::vector<std::pair<int, int>> boats;
	for (int i = 0; i < n; i++){
		int l, p; std::cin >> l >> p;
		boats.push_back(std::make_pair(p, l));
	}
	std::sort(boats.begin(), boats.end(), mysort);

	int cur_nr_boats = 1;
	int left = boats[0].first;
	bool pot_boat = false;
	int end_pot_boat = 0;
	int cur_boat = 1;
	while (cur_boat < boats.size()){
		int cur_ring = boats[cur_boat].first;
		int cur_length = boats[cur_boat].second;

		//First we need to check if ring current boat larger equal than end of a potential boat 
		//In this case we could place the last potential boat and also the current boat
		if (pot_boat){
			if (cur_ring >= end_pot_boat){ //we place potential boat, as cur ring already larger equal the end of a pot last boat
				cur_nr_boats++; //Increase boat count
				left = end_pot_boat; //Set new left ring
				pot_boat = false; //We don't have planned to place a boat anymore (just to handle the curent boat is left)
			}	
		}

		//we now calculate the new end for the current boat
		if (pot_boat){
			int temp_end_pot_boat = std::max(cur_ring, left+cur_length);
			if (temp_end_pot_boat < end_pot_boat){
				end_pot_boat = temp_end_pot_boat;
			}
		} else {
			end_pot_boat = std::max(cur_ring, left+cur_length);
			pot_boat = true;
		}

		//Maybe the new boat can be placed immediately
		if (end_pot_boat == cur_ring){
			cur_nr_boats++;
			left = end_pot_boat;
			pot_boat = false;
		}

		//Next boat
		cur_boat++;
	}

	if (pot_boat){ //Case when we planned to place the last boat but it wasn't placed anymore
		cur_nr_boats++;
	}
	std::cout << cur_nr_boats <<  std::endl;
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
