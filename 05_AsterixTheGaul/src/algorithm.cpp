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

	//Check all subsets for each amount of gulps, stop when solution found
	bool wins = false;
	long nrGulps = 0;
	for (int i = 0; i <= m; i++){
		wins = asterixWins(M, M.size(), T, D, S[i]);
		if (wins){
			nrGulps = i;
			break;
		}
	}

	if (wins){
		std::cout << nrGulps << std::endl;
	} else {
		std::cout << "Panoramix captured" << std::endl;
	}

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
