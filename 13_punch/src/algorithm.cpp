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

	std::vector<std::pair<int, int>> beverages(n, {0, 0});
	std::vector<std::tuple<int, int, int>> dp(k + 1, {-1, 0, -1}); //dim0 = cost, dim1 = nrDivBev, dim2 = bev at pos i

	for (int i = 0; i < n; i++){
		int c, v; std::cin >> c >> v;
		beverages[i] = {c, v};
	}
	std::get<0>(dp[0]) = 0; 
	for (int i = 0; i < n; i++){
		int cost_bev_i = beverages[i].first;
		int vol_bev_i = beverages[i].second;
		
		for (int j = 0; j < k; j++){
			if (std::get<0>(dp[j]) != -1){
				int newCost = std::get<0>(dp[j]) + beverages[i].first;
				int newVolume =  std::min(j + beverages[i].second, k);
				int newCombi = 0;
				if (std::get<2>(dp[j]) != i) newCombi = 1;
				int newNrBev = std::get<1>(dp[j]) + newCombi;
				if (std::get<0>(dp[newVolume]) > newCost || std::get<0>(dp[newVolume]) == -1){
					std::get<0>(dp[newVolume]) = newCost;
					std::get<1>(dp[newVolume]) = newNrBev;
					std::get<2>(dp[newVolume]) = i;
				} else if (std::get<0>(dp[newVolume]) == newCost && std::get<1>(dp[newVolume]) < newNrBev){
					std::get<1>(dp[newVolume]) = newNrBev;
					std::get<2>(dp[newVolume]) = i;
				}
			}
		}
	}
	std::cout << std::get<0>(dp[k]) << " " << std::get<1>(dp[k]) << std::endl;
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
