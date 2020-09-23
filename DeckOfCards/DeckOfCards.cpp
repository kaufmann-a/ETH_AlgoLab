#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<array>
#include<limits>
#include<vector>

int curbest = std::numeric_limits<int>::max();

void testcase(){
	int n, k; std::cin >> n; std::cin >> k;
	int i_ = 0; int j_ = 0;
	std::vector<int> cards(n);

	for (std::vector<int>::iterator it = cards.begin(); it != cards.end(); it++){
		std::cin >> *it;
	}

	int bestDiff = std::numeric_limits<int>::max();
	int best_i = 0; int best_j = 0;
	int curSum = cards.at(j_);
	while(i_ < n){

		int curDiff = std::abs(k- curSum);
		if (curDiff == 0) break;
		if (curDiff < bestDiff){
			best_i = i_; best_j = j_; bestDiff = curDiff;
		}
		if (curSum > k){
			curSum = curSum - cards.at(i_);
			if (i_ < (n-1)) i_++;
			if (j_ < i_){
				if (j_ < (n-1)) j_++;
				curSum = curSum + cards.at(j_);
			}
		} else {
			if (j_ < (n-1)) j_++;
			curSum = curSum + cards.at(j_);
		}
	}
	std::cout << i_ << " " << j_ << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}

	return 0;
}
