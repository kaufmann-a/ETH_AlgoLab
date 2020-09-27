#include<iostream>
#include<fstream>
#include<vector>

int solve(std::vector<std::vector<int>> &dp, std::vector<int> &coins, int left, int right){

	if (left > right){
		return 0;
	}

	if (dp[left][right] != -1){
		return dp[left][right];
	}



	if (left == right){
		dp[left][right] = coins[left];
		return coins[left];
	}

	int left_left = coins[left] + solve(dp, coins, left+2, right);
	int left_right = coins[left] + solve(dp, coins, left+1, right-1);
	int right_left = coins[right] + solve(dp, coins, left+1, right-1);
	int right_right = coins[right] + solve(dp, coins, left, right-2);

	//Oponent wants to minimize my win
	int lft = std::min(left_left, left_right);
	int rght = std::min(right_left, right_right);

	//I want to optimize my win
	dp[left][right] = std::max(lft, rght);
	return dp[left][right];
}


void testcase(){
	int n; std::cin >> n;
	std::vector<int> coins(n);
	std::vector<std::vector<int>> dp(n);
	for (int i = 0; i < n; i++){
		dp.at(i).resize(n, -1);
	}

	for (int i = 0; i < n; i++){
		std::cin >> coins.at(i);
	}

	int res = solve(dp, coins, 0, n-1);
	std::cout << dp.at(0).at(n-1) << std::endl;

}


int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}



	return 0;
}
