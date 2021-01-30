#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

int UP = 1; int DOWN = -1;

struct Biker{
	int index = 0;
	long y_start_pos = 0;
	int up_down = 0;
	EK::FT slope = 0;

	Biker(int ind, long stps, int ud, EK::FT sl) : index(ind), y_start_pos(stps), up_down(ud), slope(sl){};
};

bool bikerSort (const Biker &biker1, const Biker &biker2){
	return biker1.y_start_pos > biker2.y_start_pos;
}

bool bikerSortIndex (const Biker &biker1, const Biker &biker2){
	return biker1.index < biker2.index;
}

void testcase() {
	int n; std::cin >> n;

	std::vector<Biker> bikers;
	for (int i = 0; i < n; i++){
		long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
		int up_down = 0;
		//up_down:
		if (y0 <= y1){
			up_down = UP;
		} else {
			up_down = DOWN;
		}
		EK::FT y_segment = std::abs(y1-y0);
		EK::FT x1_exact = x1;
		EK::FT slope = y_segment/x1_exact;
		Biker cur_biker(i, y0, up_down, slope);
		bikers.push_back(cur_biker);
	}

	//Sort biker according to their start position, biker with highest y0 comes first
	std::sort(bikers.begin(), bikers.end(), bikerSort);

	std::vector<Biker> sunriseBikers;
	for (int i = 0; i < n; i++){
		Biker curBiker = bikers[i];
		bool curBikerDone = false;
		while (!curBikerDone){
			if (sunriseBikers.empty()){
				sunriseBikers.push_back(curBiker);
				break;
			}
			Biker lastSunriseBiker = sunriseBikers[sunriseBikers.size()-1];
			if (curBiker.up_down == UP){ // CurBiker rides up
				if (lastSunriseBiker.up_down == UP){
					if (lastSunriseBiker.slope >= curBiker.slope){
						sunriseBikers.push_back(curBiker);
						curBikerDone = true;
					} else if (lastSunriseBiker.slope < curBiker.slope){
						curBikerDone = true;
					}
				} else if (lastSunriseBiker.up_down == DOWN){
					if (lastSunriseBiker.slope  >= curBiker.slope){
						sunriseBikers.pop_back();
					} else if (lastSunriseBiker.slope < curBiker.slope){
						curBikerDone = true;
					}
				}
			} else if (curBiker.up_down == DOWN){ //Cur biker rides down
				if (lastSunriseBiker.up_down == UP){
					sunriseBikers.push_back(curBiker);
					curBikerDone = true;
				} else if (lastSunriseBiker.up_down == DOWN){
					if (lastSunriseBiker.slope > curBiker.slope){
						sunriseBikers.pop_back();
					} else if (lastSunriseBiker.slope <= curBiker.slope){
						sunriseBikers.push_back(curBiker);
						curBikerDone = true;
					}
				}
			}
		}
	}

	std::sort(sunriseBikers.begin(), sunriseBikers.end(), bikerSortIndex);

	for (uint i = 0; i < sunriseBikers.size(); i++){
		std::cout << sunriseBikers[i].index << " ";
	}
	std::cout << std::endl;

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
