#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

bool asterixWins(std::vector<std::pair<long, long>> &M, int subsetSize, long timeRemaining, long distanceRemaining, long s){
	if (distanceRemaining <= 0 && timeRemaining > 0){
		return true;
	}
	if ((subsetSize == 0 && distanceRemaining > 0) || timeRemaining <= 0 ){
		return false;
	}
	long time = timeRemaining - M[subsetSize-1].second;
	long dist = distanceRemaining - M[subsetSize-1].first-s;
	return asterixWins(M, subsetSize-1, time, dist, s) || asterixWins(M, subsetSize-1, timeRemaining, distanceRemaining, s);
}


void testcase() {
	int n, m; long D, T; std::cin >> n >> m >> D >> T;

	std::vector<std::pair<long, long>> M(n);
	std::vector<long> S(m+1);

	for (int i = 0; i < n; i++){
		long d, t; std::cin >> d >> t;
		M[i] = std::make_pair(d, t);
	}
	S[0] = 0;
	for (int i = 1; i <= m; i++){
		long s; std::cin >> s;
		S[i] = s;
	}

	//First check for 0 gulps
	bool zeroGulps = asterixWins(M, M.size(), T, D, 0);

	//Check all subsets for each amount of gulps, stop when solution found
	long nrGulps = 0;
	bool onceWon = false;
	if (zeroGulps){
		onceWon = true;
	}

	if (!zeroGulps){
		bool wins = false;
		int leftP, rightP; leftP = 0; rightP = m-1;
		while (leftP < rightP){
			int mid = leftP + (rightP - leftP)/2;
			bool wins = asterixWins(M, M.size(), T, D, S[mid]);
			if (wins){
				nrGulps = mid;
				rightP = mid;
				onceWon = true;
			} else {
				leftP = mid+1;
			}
		}
	}

	if (onceWon){
		std::cout << nrGulps << std::endl;
	} else {
		std::cout << "Panoramix captured" << std::endl;
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
