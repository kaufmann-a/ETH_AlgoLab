#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

long solveDP(std::vector<std::vector<std::pair<int, long>>> &graph, std::vector<std::vector<long>> &dp, int k, int node){
  if (k == 0){
    return 0;
  }

  if (dp[node][k] != -1){
    return dp[node][k];
  }

  //Check if cur node is Weayaya
  if (graph[node].size() == 0){
    long best = solveDP(graph, dp, k, 0);
    dp[node][k] = best;
    return best;
  } else {
    long max = 0;
    for (uint i = 0; i < graph[node].size(); i++){
      long nextVal = solveDP(graph, dp, k-1, graph[node][i].first ) + graph[node][i].second;
      if (nextVal > max){
        max = nextVal;
      }
    }
    dp[node][k] = max;
    return dp[node][k];  
  }
}



void testcase() {
	int n, m, k; long x; std::cin >> n >> m >> x >> k;

	std::vector<std::vector<std::pair<int, long>>> graph(n);
	std::vector<std::vector<long>> dp(n, std::vector<long>(k+1, -1));

	//Build graph
	for (int i = 0; i < m; i++){
		int u, v; long p; std::cin >> u >> v >> p;
		graph.at(u).push_back(std::make_pair(v, p));
	}
	int nrMoves = -1;

	for (int i = 0; i <= k; i++){
		long value = solveDP(graph, dp, i, 0);
		if (value >= x){
			nrMoves = i;
			break;
		}

	}

	if (nrMoves == -1){
		std::cout << "Impossible" << std::endl;
	} else {
		std::cout << nrMoves << std::endl;
	}
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test2.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
