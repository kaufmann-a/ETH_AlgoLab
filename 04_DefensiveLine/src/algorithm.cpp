#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

//i = nr attackers, j = start position in defensive line
int solve_recursive(std::vector<std::vector<int>> &dp, std::vector<int> &def_intervals, int i, int j, bool &fail){
	//Base case
	if (i ==0 || j == def_intervals.size()){
		return 0;
	}

	//DP-Value already there
	if (dp[i][j] != -1){
		return dp[i][j];
	}

	if (def_intervals[j] != -1){ //Case in which an interval starts at current position
		if (i == 1) { //We found an interval and just one player is remaining, thus there is a valid strategy
			fail = false;
		}
		int interval_size = def_intervals[j] - j + 1;
		int nr_attacked_using_cur_int = interval_size + solve_recursive(dp, def_intervals, i-1, j+interval_size, fail);
		int nr_attacked_not_using_cur_int = solve_recursive(dp, def_intervals, i, j+1, fail);
		dp[i][j] = std::max(nr_attacked_using_cur_int, nr_attacked_not_using_cur_int);
		return dp[i][j];
	} else { //No interval starts at current position
		dp[i][j] = solve_recursive(dp, def_intervals, i, j+1, fail);
		return dp[i][j];
	}
}

void testcase() {
	int n, m, k; std::cin >> n >> m >> k;

	std::vector<int> def_values(n, 0);
	for (int i = 0; i < n; i++){
		std::cin >> def_values[i];
	}

	//Do sliding window
	std::vector<int> def_intervals(n, -1);
	int l = 0; int r = 0;
	int curSum = 0;

	while (r < n){
		while (curSum < k && r < n){
			curSum += def_values[r];
			r++;
		}
		while(curSum > k){
			curSum -= def_values[l];
			l++;
		}
		if (curSum == k){
			def_intervals[l] = r-1;
			curSum -= def_values[l];
			l++;
		} 
	}

	//Run dp
	std::vector<std::vector<int>> dp(m+1, std::vector<int>(n, -1));
	bool fail = true;
	int nrAttacked = solve_recursive(dp, def_intervals, m, 0, fail);

	if (fail){
		std::cout << "fail" << std::endl;
	} else {
		std::cout << nrAttacked << std::endl;
	}

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
