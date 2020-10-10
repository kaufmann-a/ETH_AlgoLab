#include<iostream>
#include<fstream>
#include<vector>
#include<unordered_set>
#include<limits>

int solveDP(std::vector<std::unordered_set<int>> &graph, std::vector<std::vector<int>> &dp, int curPos, int n, bool myMove){
	if (curPos == n){
		return 0;
	}

	if (myMove){
		//First check dp-table
		if (dp[0][curPos] != -1){
			return dp[0][curPos];
		}
		//Do the move which minimizes the nr of moves to the target
		int minSteps = std::numeric_limits<int>::max();
		for (int neighbour : graph[curPos]){
			int stepsFromNeighbour = solveDP(graph, dp, neighbour, n, !myMove);
			if (stepsFromNeighbour < minSteps){
				minSteps = stepsFromNeighbour;
			}
		}
		dp[0][curPos] = minSteps + 1;
		return dp[0][curPos];
	} else {
		//First check dp-table
		if (dp[1][curPos] != -1){
			return dp[1][curPos];
		}
		//Now the contrahend moves my meeple, thus he tackes the move which maximizes the distance to the target
		int maxSteps = 0;
		for (int neighbour : graph[curPos]){
			int stepsFromNeighbour = solveDP(graph, dp, neighbour, n, !myMove);
			if (stepsFromNeighbour > maxSteps){
				maxSteps = stepsFromNeighbour;
			}
		}
		dp[1][curPos] = maxSteps + 1;
		return dp[1][curPos];
	}
	return 0;
}

void testcase(){
  int n, m; std::cin >> n; std::cin >> m;
  int r, b; std::cin >> r; std::cin >> b;

  std::vector<std::unordered_set<int>> graph(n);
  std::vector<std::vector<int>> dp_red(2, std::vector<int>(n, -1));
  std::vector<std::vector<int>> dp_black(2, std::vector<int>(n, -1));

  for (int i = 0; i < m; i++){
    int u, v; std::cin >> u; std::cin >> v;
    graph[u-1].insert(v-1);
  }

  int res_Sherlock = solveDP(graph, dp_red, r-1, n-1, true);
  int res_Moriaty = solveDP(graph, dp_black, b-1, n-1, true);
  if (res_Moriaty == res_Sherlock){
	  if (res_Sherlock % 2 == 0){
		  std::cout << 1 << std::endl;
	  } else {
		  std::cout << 0 << std::endl;
	  }
  } else {
	  if (res_Sherlock > res_Moriaty){
		  std::cout << 1 << std::endl;
	  } else {
		  std::cout << 0 << std::endl;
	  }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::fstream in("./testsets/test2.in");
  std::cin.rdbuf(in.rdbuf());

  int t; std::cin >> t;
  for (int i = 0; i < t; i++){
    testcase();
  }

  return 0;
}

