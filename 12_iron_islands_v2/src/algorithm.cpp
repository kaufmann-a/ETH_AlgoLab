#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

int most_nr_islands_pref(std::vector<std::pair<int, int>> &array1, std::vector<std::pair<int, int>> &array2, int pyke){
	int l = pyke; int r = array1.size()-1;
	int max_sum = 0;

	while (l <= r){
		//First check if comi possible
		if (array1[l].first != -1 && array1[l].second != -1 && array2[r].first != -1 && array2[r].second != -1){
			//Second, check if differnt waterways
			if (array1[l].first != array2[r].first){
				int cur_sum = array1[l].second + array2[r].second;
				if (cur_sum > max_sum){
					max_sum = cur_sum;
				}
			}
		}
		l++; r--;
	}
	return max_sum-1;
}

void testcase() {
	int n, k, w; std::cin >> n >> k >> w;

	std::vector<int> c(n);
	for (int i = 0; i < n; i++){
		std::cin >> c[i];
	}
	
	std::vector<std::vector<int>> water_ways(w);
	std::vector<std::pair<int, int>> best_prefix(k+1, std::pair<int, int>(-1, -1)); //pair.first = nr.waterway, pair.second = nr.islands
	std::vector<std::pair<int, int>> second_best_prefix(k+1, std::pair<int,int>(-1, -1)); //""
	
	for (int i = 0; i < w; i++){
		int l; std::cin >> l;
		int prefix_sum_cur_waterway = 0;
		
		for (int j = 0; j < l; j++){
			int isl_numbr; std::cin >> isl_numbr;
			int nr_men_cur_isl = c[isl_numbr];

			//Push back nr men to current waterway
			water_ways[i].push_back(nr_men_cur_isl);

			//Insert into prefix-sum arrays
			prefix_sum_cur_waterway += nr_men_cur_isl;
			std::pair<int, int> pot = std::pair<int, int>(i, j+1);
			if (prefix_sum_cur_waterway > k){
				continue;
			}
			if (best_prefix[prefix_sum_cur_waterway].first == -1 &&  best_prefix[prefix_sum_cur_waterway].second == -1){ //not inserted yet
				best_prefix[prefix_sum_cur_waterway] = pot;
			} else if (best_prefix[prefix_sum_cur_waterway].second < pot.second) { //There is already stored a pair, check if new better
				second_best_prefix[prefix_sum_cur_waterway] = best_prefix[prefix_sum_cur_waterway];
				best_prefix[prefix_sum_cur_waterway] = pot;
			} else if (second_best_prefix[prefix_sum_cur_waterway].second < pot.second){ //Best was better, check if better than second best
				second_best_prefix[prefix_sum_cur_waterway] = pot;
			}
		}
	}	

	//Now do sliding window on the waterways
	int max_islands = 0;
	for (int i = 0; i < w; i++){
		std::vector<int> cur_water_way = water_ways[i];
		int l = 0; int r = 0; 
		int curNrMen = 0;
		while (r < cur_water_way.size()){
			while (curNrMen < k && r < cur_water_way.size()){
				curNrMen += cur_water_way[r];
				r++;
			}
			while (curNrMen > k && l < r){
				curNrMen -= cur_water_way[l];
				l++;
			}
			if (curNrMen == k){
				int nrIslands = r-l;
				if (nrIslands > max_islands){
					max_islands = nrIslands;
				}
				curNrMen -= cur_water_way[l];
				l++;
			}
		}
	}

	//Now check the prefix sum arrays
	int v1 = most_nr_islands_pref(best_prefix, best_prefix, c[0]);
	int v2 = most_nr_islands_pref(best_prefix, second_best_prefix, c[0]);
	int v3 = most_nr_islands_pref(second_best_prefix, best_prefix, c[0]);
	int v4 = most_nr_islands_pref(second_best_prefix, second_best_prefix, c[0]);
	max_islands = std::max(max_islands, std::max(std::max(std::max(v1, v2), v3), v4));
	std::cout << max_islands << std::endl;


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
