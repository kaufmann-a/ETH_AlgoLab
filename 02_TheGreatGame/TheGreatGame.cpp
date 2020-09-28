#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_set>

int solve(std::vector<std::unordered_set<int>> &graph, int r, int b, int movenr, int n, std::vector<std::vector<int>> &dp){


	if (r == n){
		return 0;
	}
	if (b == n){
		return 1;
	}

	if (movenr % 4 == 0){
//		if (dp[0][r] != -1){
//			return dp[0][r];
//		}
		//Holmes moves red meeple, thus when he can move it to a place where he has a winning strategy he wins
		int strategyHolmes = 1; //init wiht 1 (no winning strategy)
		for (std::unordered_set<int>::iterator it = graph.at(r).begin(); it != graph.at(r).end(); it++){

			if (solve(graph, *it, b, movenr+1, n, dp) == 0){
				dp[0][r] = 0;
				strategyHolmes = 0;
				break;
			}
		}
		return strategyHolmes;
	} else if (movenr % 4 == 1){ //Moraty moves the black meeple, thus if he can move it to a place where he has a winning strategy he wins
//		if (dp[1][b] != -1){
//			return dp[1][b];
//		}
		int strategyMoraty = 0; //init with 0 (no winning strategy)
		for (std::unordered_set<int>::iterator it = graph.at(b).begin(); it != graph.at(b).end(); it++){
			if (solve(graph, r, *it, movenr+1, n, dp) == 1){
				dp[1][b] = 1;
				strategyMoraty = 1;
				break;
			}
		}
		return strategyMoraty;
	} else if (movenr % 4 == 2){ //Holmes moves the black meeple, he has to move it to a place where Moraty has no winning strategy
//		if (dp[1][b] != -1){
//			return dp[1][b];
//		}
		int strategyMoraty = 1; //init with 1 (Moraty has winning strategy)
		for (std::unordered_set<int>::iterator it = graph.at(b).begin(); it != graph.at(b).end(); it++){
			if (solve(graph, r, *it, movenr+1, n, dp) == 0){
				dp[1][b] = 0;
				strategyMoraty = 0;
				break;
			}
		}
		return strategyMoraty;
	} else if (movenr % 4 == 3){ //Moratty moves the red meeple, he has to move it to a place where Holmes has no winning strategy
//		if (dp[0][r] != -1){
//			return dp[0][r];
//		}
		int strategyHolmes = 0; //init with 0 (Holmes has strategy
		for (std::unordered_set<int>::iterator it = graph.at(r).begin(); it != graph.at(r).end(); it++){
			if (solve(graph, *it, b, movenr+1, n, dp) == 1){
				dp[0][r] = 1;
				strategyHolmes = 1;
				break;
			}
		}
		return strategyHolmes;
	}
}

void testcase(){
  int n, m; std::cin >> n; std::cin >> m;
  int r, b; std::cin >> r; std::cin >> b;

  std::vector<std::unordered_set<int>> graph(n);
  std::vector<std::vector<int>> dp(2, std::vector<int>(n, -1));

  for (int i = 0; i < m; i++){
    int u, v; std::cin >> u; std::cin >> v;
    graph[u-1].insert(v-1);
  }

  int res = solve(graph, r-1, b-1, 0, n-1, dp);
  std::cout << res << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::fstream in("./testsets/test1.in");
  std::cin.rdbuf(in.rdbuf());

  int t; std::cin >> t;

  for (int i = 0; i < t; i++){
    testcase();
  }
}

