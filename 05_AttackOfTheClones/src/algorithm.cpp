#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

bool sortby (std::pair<int, int> &p1, std::pair<int, int> &p2){
	return p1.second < p2.second;
}

void testcase() {
	int n, m; std::cin >> n >> m;
	
	std::vector<std::pair<int, int>> jedis(n);
	for (int i = 0; i < n; i++){
		int a, b; std::cin >> a >> b;
		jedis[i] = std::make_pair(a, b);
	}

	std::sort(jedis.begin(), jedis.end(), sortby);
	
	//find first interval, starting at 1
	std::vector<std::pair<int, int>>::iterator it_find_first = jedis.begin();
	int first = 0;
	while (it_find_first->first > it_find_first->second){
		it_find_first++;
		first++;
	}

	//Try all segments up to first previously found
	int max_nrOfJedis = 1;
	for (int i = 0; i <= first; i++){
		int nrOfJedis = 1;
		int curRightSide = jedis[i].second;
		int curLeftSide = jedis[i].first;
		int nextJediToTry = first;

		//Calculate left bound
		if (jedis[i].first < jedis[i].second){
			curLeftSide = m+1;
		} else {
			curLeftSide = jedis[i].first;
		}

		while (jedis[nextJediToTry].second < curLeftSide && nextJediToTry < n){
			//Various checks:
			bool noOverlap = jedis[nextJediToTry].first > curRightSide;
			bool fristSmalerSecond = jedis[nextJediToTry].first <= jedis[nextJediToTry].second;
			if (noOverlap && fristSmalerSecond){
				curRightSide = jedis[nextJediToTry].second;
				nrOfJedis++;
			}
			nextJediToTry++;
		}
		if (nrOfJedis > max_nrOfJedis){
			max_nrOfJedis = nrOfJedis;
		}
	}
	std::cout << max_nrOfJedis << std::endl;

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
