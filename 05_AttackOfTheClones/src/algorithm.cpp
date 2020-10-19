#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <limits>

bool sortby (std::pair<int, int> &p1, std::pair<int, int> &p2){
	return p1.second < p2.second;
}

void testcase() {
	int n, m; std::cin >> n >> m;
	
	std::vector<std::pair<int, int>> jedis_normal;

  //Variables for jedis crossing zero point
  int curShortest = std::numeric_limits<int>::max(); int curShortest_a = 0; int curShortest_b = 0; bool foundShortest = false;
  int curMinb_b = m; int curMinb_a = 0; bool foundMin = false;
  int curMaxa_a = 0; int curMaxa_b = 0; bool foundMax = false;

  for (int i = 0; i < n; i++){
		int a, b; std::cin >> a >> b;

    //Check if normal jedi or if it crosses the zero point
    if (b >= a){ //Normal jedi
      jedis_normal.push_back(std::make_pair(a, b));
    } else {
      //This jedi crosses the zero point, just add if it is of interest (shortest segment, smallest a, smallest b)
      if (m-a+b < curShortest){
        curShortest = m-a+b;
        curShortest_a = a; curShortest_b = b;
        foundShortest = true;
      }
      if (b < curMinb_b){
        curMinb_b = b; curMinb_a = a;
        foundMin = true;
      }
      if (a > curMaxa_a){
        curMaxa_a = a; curMaxa_b = b;
        foundMax = true;
      }
    }
	}

	std::sort(jedis_normal.begin(), jedis_normal.end(), sortby);
	
  //We now have 4 possibilities:
  //1. No jedi crossing zeropoint in solution
  //2. Jedi with shortest segment crossing zeropoint in solution
  //3. Jedi with smalles b, crossing zeropoint in solution
  //3. Jedi with largest a, crossing zeropoint in solution
  std::vector<std::pair<bool, std::pair<int, int>>> possibilities = {std::make_pair(true, std::make_pair(0, m+1)), std::make_pair(foundShortest, std::make_pair(curShortest_b, curShortest_a)), std::make_pair(foundMin, std::make_pair(curMinb_b, curMinb_a)), std::make_pair(foundMax, std::make_pair(curMaxa_b, curMaxa_a))};
  
  int maxJedis = 0;
  for (int i = 0; i < 4; i++){
    if (!possibilities[i].first){
      continue;
    }
    int nrJedis = 1;
    if (i == 0){
      nrJedis = 0;
    }
    int leftBorder = possibilities[i].second.first;
    int rightBorder = possibilities[i].second.second;
    std::vector<std::pair<int, int>>::iterator it = jedis_normal.begin();
    while (leftBorder < rightBorder && it != jedis_normal.end()){
      if (it->first > leftBorder && it->second < rightBorder){
        nrJedis++;
        leftBorder = it->second;
      }
      it++;
    }
    if (nrJedis > maxJedis){
      maxJedis = nrJedis;
    }
  }

  std::cout << maxJedis << std::endl;

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());
	
	int t; std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}