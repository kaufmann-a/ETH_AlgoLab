#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>

void testcase() {
	int n, m, k; std::cin >> n >> m >> k;

  std::vector<int> h(n);
  std::vector<std::vector<int>> G(n);
  std::vector<int> parents(n, 0);
  std::vector<int> out_deg(n);

  for (int i = 0; i < n; i++){
    std::cin >> h[i];
  }

  for (int i = 0; i < n-1; i++){
    int u, v; std::cin >> u >> v;
    G[u].push_back(v);
    parents[v] = u;
    out_deg[u]++;
  }

  //Now we iterate thorugh the graph backwards and apply sliding window
  std::set<int> solFound; //this set is used to indicate when there is valid route from node i
  std::vector<bool> visited(n, false); //this array is used to indicate when range already processed (is set to true when right pointer was there and rangesize = m)
  for (int i = 0; i < n; i++){
    if (out_deg[i] != 0){
      continue; //We search startingpoints wiht outdeg=0 as these are the endpoints of the tree
    }
    int leftPointer = i;
    int rightPointer = i;
    int intervalSize = 1;
    std::multiset<int> curTemps;
    curTemps.insert(h[leftPointer]);
    while(visited[rightPointer] == false){
      int min = *curTemps.begin();
      int max = *curTemps.rbegin();
      if (max - min > k){
        visited[rightPointer] = true;
        curTemps.erase(curTemps.find(h[rightPointer]));
        rightPointer = parents[rightPointer];
        intervalSize--;
      } else if (intervalSize < m){
        if (leftPointer != parents[leftPointer]){
          intervalSize++;
          leftPointer = parents[leftPointer];
          curTemps.insert(h[leftPointer]);
        } else {
          visited[rightPointer] = true;
          rightPointer = parents[rightPointer];
        }
      } else if (intervalSize == m){

        //Check if cur temprange doesn't exceed k
        if (max - min <= k) { //we found a solution
          solFound.insert(leftPointer);
        }
        curTemps.erase(curTemps.find(h[rightPointer]));
        visited[rightPointer] = true;
        rightPointer = parents[rightPointer];
        intervalSize--;
      }
    }
  }
  if (solFound.size() == 0){
    std::cout << "Abort mission";
  }
  for (std::set<int>::iterator it = solFound.begin(); it != solFound.end(); it++){
    std::cout << *it << " ";
  }
  std::cout << std::endl;
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
