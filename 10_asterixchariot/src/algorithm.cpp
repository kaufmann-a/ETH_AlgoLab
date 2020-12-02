#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <tuple>

std::tuple<int, int, int> minCost(std::vector<std::vector<int>> &G, std::vector<int> &cost, std::vector<std::tuple<int, int, int>> &dp, int curNode){
	//Check edge cases
	if (G[curNode].size() == 0){ //Has no children
		std::get<0>(dp[curNode]) = cost[curNode];
		std::get<1>(dp[curNode]) = std::pow(2, 14) + 1; //not allowed
		std::get<2>(dp[curNode]) = 0;
		return dp[curNode];
	}

	if (std::get<0>(dp[curNode]) != -1 && std::get<1>(dp[curNode]) != -1 && std::get<2>(dp[curNode]) != -1){
		return dp[curNode];
	}

	int min_with_i = cost[curNode];
	int min_without_i_with_i1 = 0;
	int min_without_i_without_i1 = 0;

	bool with_i1_taken = false;
	int minDiff = std::numeric_limits<int>::max();
	for (int curChild = 0; curChild < G[curNode].size(); curChild++){
		int nodeNrChild =  G[curNode][curChild];
		
		//Child dp values
		std::tuple<int, int, int> childValues = minCost(G, cost, dp, nodeNrChild);
		int dim0 = std::get<0>(childValues);
		int dim1 = std::get<1>(childValues);
		int dim2 = std::get<2>(childValues);
		min_with_i += std::min(dim0, std::min(dim1, dim2));

		if (dim0 <= dim1){
			with_i1_taken = true;
		}
		if (dim1 <= dim0){
			if (dim0-dim1 <= minDiff){
				minDiff = dim0-dim1;
			}
		}
		min_without_i_with_i1 += std::min(dim0, dim1);
		min_without_i_without_i1 += std::min(dim0, dim1);
	}
	if (!with_i1_taken){
		min_without_i_with_i1 += minDiff;
	}
	dp[curNode] = std::make_tuple(min_with_i, min_without_i_with_i1, min_without_i_without_i1);
	return(dp[curNode]);
}

void testcase() {
	int n; std::cin >> n;

	std::vector<std::vector<int>> adj_list(n);
	std::vector<int> cost(n);
	for (int i = 0; i < n-1; i++){
		int u, v; std::cin >> u >> v;
		adj_list[u].push_back(v);
	}
	for (int i = 0; i < n; i++){
		std::cin >> cost[i];
	}

	//dim0: minimum at node i, containing i
	//dim1: minimum at node i, not containing i but with all i+1
	//dim2: mimimum at node i, not containing i and not containing at least one i+1
	std::vector<std::tuple<int, int, int>> dp(n, std::tuple<int, int, int>(-1, -1, -1));
	std::tuple<int, int, int> curSol = minCost(adj_list, cost, dp, 0);
	std::cout << std::min(std::get<0>(curSol), std::get<1>(curSol)) << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/user_test.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
