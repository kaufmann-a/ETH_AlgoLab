#include<iostream>
#include<fstream>
#include<vector>
#include<set>

int solve(std::vector<std::vector<int>> &dp, std::vector<int> &coins, int left, int right, int currPers, int k ,int totP){

	if (left > right){
		return 0;
	}

	if (left == right){
		if (currPers == k){
            return coins[left];
        } else {
            return 0;
        }
	}

    if (dp[left][right] != -1){
        return dp[left][right];
    }

    int res = 0;
    int nextPerson = (currPers + 1) % totP;
    if (currPers != k){
        res = std::min(solve(dp, coins, left+1, right, nextPerson, k, totP), solve(dp, coins, left, right-1, nextPerson, k, totP));
    } else {
        res = std::max(coins[left] + solve(dp, coins, left+1, right, nextPerson, k, totP), coins[right] + solve(dp, coins, left, right-1, nextPerson, k, totP));
    }
    dp[left][right] = res;
    return res;

}


void testcase(){
	int n, m, k; std::cin >> n; std::cin >> m; std::cin >> k;
	std::vector<int> coins(n);
	std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));

	for (int i = 0; i < n; i++){
		std::cin >> coins[i];
	}

	int res = solve(dp, coins, 0, n-1, 0, k, m);
	std::cout << res << std::endl;

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
