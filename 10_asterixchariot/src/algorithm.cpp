#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>


void solve(std::vector<std::vector<int>> &adj_list, std::vector<std::vector<int>> &dp, std::vector<int> &c, int cur_node){
	//First check if leave
	if (adj_list[cur_node].size() == 0){
		dp[0][cur_node] = c[cur_node];
		dp[1][cur_node] = std::numeric_limits<int>::max();
		dp[2][cur_node] = 0;
		return;
	}
	//Check if dp already calculated
	if (dp[0][cur_node] != -1 && dp[1][cur_node] != -1 && dp[2][cur_node] != -1){
		return;
	}

	//solve for subnodes
	for (size_t i = 0; i < adj_list[cur_node].size(); i++){
		solve(adj_list, dp, c, adj_list[cur_node][i]);
	}

	std::vector<int> children = adj_list[cur_node];
	//first dp[0][curNode]
	int cur_cost_0 = c[cur_node];
	for (size_t i = 0; i < adj_list[cur_node].size(); i++){
		int curChild = children[i];
		cur_cost_0 += std::min(std::min(dp[0][curChild], dp[1][curChild]), dp[2][curChild]);
	}

	//dp[1][curNode]
	int cur_cost_1 = 0;
	int cur_diff = std::numeric_limits<int>::max();
	size_t index_i1 = 0;
	bool found_index_anyway = false;
	for (size_t i = 0; i < adj_list[cur_node].size(); i++){
		int dp0 = dp[0][adj_list[cur_node][i]];
		int dp1 = dp[1][adj_list[cur_node][i]];
		if (dp0 <= dp1){
			found_index_anyway = true;
		} else {
			if (std::abs(dp0 - dp1) < cur_diff){
				cur_diff = std::abs(dp0 - dp1);
				index_i1 = i;
			}
		}
	}

	if (found_index_anyway){
		for (size_t i = 0; i < children.size(); i++){
			int curChild = children[i];
			cur_cost_1 += std::min(dp[0][curChild], dp[1][curChild]);
		}
	} else {
		for (size_t i = 0; i < children.size(); i++){
			int curChild = children[i];
			if (i == index_i1){
				cur_cost_1 += dp[0][curChild];
			} else {
				cur_cost_1 += std::min(dp[0][curChild], dp[1][curChild]);
			}
		}
	}

	//dp[2][curNode]
	int cur_cost_2 = 0;
	for (size_t i = 0; i < children.size(); i++){
		int curChild = children[i];
		cur_cost_2 += std::min(dp[0][curChild], dp[1][curChild]);
	}

	//Set dp entries
	dp[0][cur_node] = cur_cost_0;
	dp[1][cur_node] = cur_cost_1;
	dp[2][cur_node] = cur_cost_2;

}

void testcase() {
	int n; std::cin >> n;

	std::vector<std::vector<int>> adj_list (n);
	for (int i = 0; i < n-1; i++){
		int u, v; std::cin >> u >> v;
		adj_list[u].push_back(v);
	}

	std::vector<int> c(n);
	for (int i = 0; i < n; i++){
		std::cin >> c[i];
	}

	std::vector<std::vector<int>> dp(3, std::vector<int>(n, -1));
	solve(adj_list, dp,  c, 0);
	std::cout << std::min(dp[0][0], dp[1][0]) << std::endl;

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
