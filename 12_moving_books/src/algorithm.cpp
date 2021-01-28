#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>


struct mysorter{
	bool operator() (const int &i1, const int &i2){
		return i1 < i2;
	}
};

bool sortFriends(const int &friend1, const int &friend2){
	return friend1 > friend2;
}

void testcase() {
	int n, m; std::cin >> n >> m;

	std::vector<int> friends(n, 0);
	for (int i = 0; i < n; i++){
		std::cin >> friends[i];
	}

	std::sort(friends.begin(), friends.end(), sortFriends);

	std::multiset<int, mysorter> boxes;
	for (int i = 0; i < m; i++){
		int w; std::cin >> w;
		boxes.insert(w);
	}

	//First check if strongest friend can lift heaviest box - only case in which we output impossible
	bool impossible = false;
	if (friends[0] < *boxes.rbegin()){
		impossible = true;
	}

	int curTime = 0;
	if (!impossible){
		while(!boxes.empty()){
			for (uint i = 0; i < friends.size(); i++){
				auto pointerToBox = boxes.upper_bound(friends[i]);
				if (boxes.empty()){ //We finnished all boxes
					break;
				} else if (pointerToBox == boxes.begin()){ //Friend is to weak to lift the box
					break;
				} else {
					pointerToBox--;
					boxes.erase(pointerToBox);
				} 
			}
			curTime+=3;
		}
		curTime -= 1;
	}

	if (!impossible){
		std::cout << curTime << std::endl;
	} else {
		std::cout << "impossible" << std::endl;
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
