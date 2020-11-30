#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

int findLongestWay(std::vector<int> &curArray, int k){
	int rightPos = 0;
	int leftPos = 0;
	int curSum = 0;
	int curLongest = 0;
	while (rightPos < curArray.size()){
		while (curSum < k && rightPos < curArray.size()){
			curSum += curArray[rightPos];
			rightPos++;
		}
		
		while (curSum > k && leftPos <= rightPos){
			curSum -= curArray[leftPos];
			leftPos++;
		}

		if (curSum == k){
			//Check if new longest way found
			int curLength = rightPos - leftPos;
			if (curLength > curLongest){
				curLongest = curLength;
			}
			if (rightPos < curArray.size()){
				curSum += curArray[rightPos];
				rightPos++;
			}
			
		}
	}
	return curLongest;
}

void testcase() {
	int n, k, w; std::cin >> n >> k >> w;

	std::vector<int> c(n);
	for (int i = 0; i < n; i++){
		std::cin >> c[i];
	}
	std::vector<std::vector<int>> ways(w);
	std::vector<std::vector<int>> ways_summed(w);
	for (int i = 0; i < w; i++){
		int l; std::cin >> l;
		for (int j = 0; j < l; j++){
			int curNr; std::cin >> curNr;
			ways[i].push_back(c[curNr]);
			if (curNr == 0){
				ways_summed[i].push_back(c[curNr]);
			} else {
				ways_summed[i].push_back(ways_summed[i][j-1] + c[curNr]);
			}
		}
	}

	//Find all solutions just contained in one waterway
	int curLongestWay = 0;
	for (int nrWay = 0; nrWay < ways.size(); nrWay++){
		int findLongest = findLongestWay(ways[nrWay], k);
		if (findLongest > curLongestWay){
			curLongestWay = findLongest;
		}
	}

	//Find all solutions which contain two waterways
	for (int waterWay1 = 1; waterWay1 < w; waterWay1++){
		if (ways[waterWay1][0] >= k) break;
		for (int waterWay2 = 0; waterWay2 < waterWay1; waterWay2++){
			//First find find for each array index at which sum is bigger k, set index to that index-1
			int firstArrayLimitRight = 0;
			while (ways_summed[waterWay1].size() > firstArrayLimitRight && ways_summed[waterWay1][firstArrayLimitRight] < k){
				firstArrayLimitRight++;
			}
			firstArrayLimitRight--;
			int secondArrayLimitLeft = 0;
			while (ways_summed[waterWay2].size() > secondArrayLimitLeft && ways_summed[waterWay2][secondArrayLimitLeft] < k){
				secondArrayLimitLeft++;
			}
			secondArrayLimitLeft--;

			//Now invert second array and add first array for convenience
			std::vector<int> curArray;
			while(secondArrayLimitLeft > 0){
				curArray.push_back(ways[waterWay2][secondArrayLimitLeft]);
				secondArrayLimitLeft--;
			}
			int curRightIt = 0;
			while(curRightIt <= firstArrayLimitRight){
				curArray.push_back(ways[waterWay1][curRightIt]);
				curRightIt++;
			}
			int curLongest = findLongestWay(curArray, k);
			if (curLongest > curLongestWay){
				curLongestWay = curLongest;
			}

		}
	}
	std::cout << curLongestWay << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
