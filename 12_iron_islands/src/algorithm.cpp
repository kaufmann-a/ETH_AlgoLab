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
	std::vector<int> index_summ_bigger_k(w);
	int sumsize = 0;
	if (k >= c[0]){
		sumsize = k-c[0];
	}
	std::vector<std::tuple<int, int>> preFixSums(sumsize+1, std::tuple<int, int>(-1, -1)); //tuple: dim0=nrWaterWay, dim1=index
	std::vector<std::tuple<int, int>> preFixSums2(sumsize+1, std::tuple<int, int>(-1, -1));
	for (int i = 0; i < w; i++){
		int l; std::cin >> l;
		int curSum = 0;
		bool indexSaved = false;
		for (int j = 0; j < l; j++){
			int curNr; std::cin >> curNr;
			ways[i].push_back(c[curNr]);
			if (j != 0){
				curSum += c[curNr];
			}
			if (curSum >= k && !indexSaved){
				index_summ_bigger_k[i] = j-1;
				indexSaved = true;
			}
			if (curSum < sumsize){
				if (std::get<1>(preFixSums[curSum]) < j){
					preFixSums2[curSum] = std::make_tuple(std::get<0>(preFixSums[curSum]), std::get<1>(preFixSums[curSum]));
					preFixSums[curSum] = std::make_tuple(i, j);
				}
			}
		}
		if (!indexSaved){
			index_summ_bigger_k[i] = ways[i].size()-1;
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
	int rightPointer = sumsize;
	int leftPointer = 0;
	while (rightPointer >= leftPointer){
		int a1_w_left = std::get<0>(preFixSums[leftPointer]);
		int a1_w_right = std::get<0>(preFixSums[rightPointer]);
		int a1_ind_left = std::get<1>(preFixSums[leftPointer]);
		int a1_ind_right = std::get<1>(preFixSums[rightPointer]);

		int a2_w_left = std::get<0>(preFixSums2[leftPointer]);
		int a2_w_right = std::get<0>(preFixSums2[rightPointer]);
		int a2_ind_left = std::get<1>(preFixSums2[leftPointer]);
		int a2_ind_right = std::get<1>(preFixSums2[rightPointer]);
		int range = 0;
		if (a1_w_left != a1_w_right && a1_w_left != -1 && a1_w_right != -1) curLongestWay = std::max(curLongestWay, a1_ind_left + a1_ind_right + 1);
		if (a1_w_left != a2_w_right && a1_w_left != -1 && a2_w_right != -1) curLongestWay = std::max(curLongestWay, a1_ind_left + a2_ind_right + 1);
		if (a2_w_left != a1_w_right && a2_w_left != -1 && a1_w_right != -1) curLongestWay = std::max(curLongestWay, a2_ind_left + a1_ind_right + 1);
		if (a2_w_left != a2_w_right && a2_w_left != -1 && a2_w_right != -1) curLongestWay = std::max(curLongestWay, a2_ind_left + a2_ind_right + 1);
		rightPointer--;
		leftPointer++;
	}

	// for (int waterWay1 = 1; waterWay1 < w; waterWay1++){
	// 	if (ways[waterWay1][0] >= k) break;
	// 	for (int waterWay2 = 0; waterWay2 < waterWay1; waterWay2++){
	// 		//Now invert second array and add first array for convenience
	// 		std::vector<int> curArray;
	// 		int secondArrayLimitLeft = index_summ_bigger_k[waterWay2];
	// 		int firstArrayRight = index_summ_bigger_k[waterWay1];
	// 		while(secondArrayLimitLeft > 0){
	// 			curArray.push_back(ways[waterWay2][secondArrayLimitLeft]);
	// 			secondArrayLimitLeft--;
	// 		}
	// 		int curRightIt = 0;
	// 		while(curRightIt <= firstArrayRight){
	// 			curArray.push_back(ways[waterWay1][curRightIt]);
	// 			curRightIt++;
	// 		}
	// 		int curLongest = findLongestWay(curArray, k);
	// 		if (curLongest > curLongestWay){
	// 			curLongestWay = curLongest;
	// 		}
	// 	}
	// }
	std::cout << curLongestWay << std::endl;
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
