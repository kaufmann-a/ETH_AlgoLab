#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

void testcase() {
	int n, k; std::cin >> n >> k;

	std::vector<std::pair<int, int>> b;
	int largest_bev = 0;
	for (int i = 0; i < n; i++){
		int c, v; std::cin >> c >> v;
		b.push_back(std::make_pair(c, v));
		if (v > largest_bev) {
			largest_bev = v;
		}
	}
	std::vector<std::vector<int>> dp(k + 1 + largest_bev, std::vector<int>(3, -1));
	//Init middle column to 0
	for (size_t i = 0; i < dp.size(); i++){
		dp[i][1] = 0;
	}

	//Init dp
	dp[0][0] = 0;

	//Run dp: dp[i][0] = curCost, dp[i][1] = curNrDistinctBeverages, dp[i][2] = index cur beverage
	for (size_t i = 0; i < b.size(); i++){ //Iterate over beverages
		for (size_t j = 0; j < k; j++){ //Iterate over dp-table
			if (dp[j][0] != -1){ //We could potentially add a beverage here
				int new_vol = j + b[i].second;
				int new_cost = dp[j][0] + b[i].first;
				int new_bev = 0;
				if (dp[j][2] != i){
					new_bev = 1;
				}
				if (dp[new_vol][0] > new_cost || dp[new_vol][0] == -1){ //Case using beverage is cheaper
					dp[new_vol][0] = new_cost;
					dp[new_vol][2] = i;
					dp[new_vol][1] = dp[j][1] + new_bev;
				} else if (dp[new_vol][0] == new_cost){ // just potentially new beverage
					if (dp[j][1]+new_bev > dp[new_vol][1]){
						dp[new_vol][1] = dp[j][1] + new_bev;
						dp[new_vol][2] = i;
					}
				}
			}
		}
	}

	int min_cost = std::numeric_limits<int>::max();
	int max_bev = 0;
	for (size_t i = k; i < dp.size(); i++){
		if (dp[i][0] < min_cost && dp[i][0] != -1){
			min_cost = dp[i][0];
			max_bev = dp[i][1];
		} else if (dp[i][0] == min_cost && dp[i][1] > max_bev){
			max_bev = dp[i][1];
		}
	}

	std::cout << min_cost << " " << max_bev << std::endl;

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
