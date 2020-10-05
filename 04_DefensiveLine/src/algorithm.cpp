#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <set>

void testcase() {
	int n, m, k; std::cin >> n; std::cin >> m; std::cin >> k;

	std::vector<int> def_values(n);
	std::vector<int> dp(n+1, 0);
	dp[0] = 0;
	std::multiset<int> nr;

	for (int i = 0; i < n; i++){
		std::cin >> def_values[i];
		//std::cout << "value: " << def_values[i] << std::endl;
	}

	//Do sliding window approach
	int leftIdx = 0; 
	int rightIdx = 0;
	int curMaxPlayersAttacked = 0;
	int curSum = 0;
	while (rightIdx < n){
		while (rightIdx < n && curSum < k){
			curSum += def_values[rightIdx];
			//std::cout << "first while: " << curSum << std::endl;
			dp[rightIdx+1] = dp[rightIdx]; //due to basecase dp table indexes are always +1
			rightIdx++;
		}
		
		while (leftIdx < rightIdx && curSum >= k){
			if (curSum == k){
				//std::cout << "second while: " << curSum << std::endl;
				int tempPlayersAttacked = rightIdx - leftIdx + dp[leftIdx];
				//std::cout << "tempPlayersattacked: " << tempPlayersAttacked << std::endl;
				//std::cout << "curMaxPlayersattacked: " << curMaxPlayersAttacked << std::endl;
				if (tempPlayersAttacked > curMaxPlayersAttacked){
					dp[rightIdx] = tempPlayersAttacked;
					nr.insert(rightIdx - leftIdx);
					curMaxPlayersAttacked = tempPlayersAttacked;
				}
			} 
			
			curSum -= def_values[leftIdx];
			//std::cout << "curSum sendwhile: " << curSum << std::endl;
			leftIdx++;
		}
	}

	int sum = 0; 
	int count = 0;
	//std::cout << "m is: " << m << std::endl; 
	for (std::multiset<int>::reverse_iterator it = nr.rbegin(); it != nr.rend(); it++){
		if (count < m){
			sum += *it;
			//std::cout << "sumcalc: " << *it << std::endl;
		}
		count++;
	}

	if (count < m){
		std::cout << "fail" << std::endl;
	} else {
		std::cout << sum << std::endl;
	}
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;
	for (int i = 0; i < t; ++i){
		testcase();
	}
		

	return 0;
}
