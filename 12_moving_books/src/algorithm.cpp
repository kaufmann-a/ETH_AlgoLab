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
	int n, m; std::cin >> n >> m;

	std::vector<int> friends(n);
	std::multiset<int> boxes;

	for (int i = 0; i < n; i++){
		std::cin >> friends[i];
	}
	for (int i = 0; i < m; i++){
		int box; std::cin >> box;
		boxes.insert(box);
	}
	std::sort(friends.begin(), friends.end());
	std::multiset<int>::iterator end = boxes.end(); end--;
	if (friends[n-1] < *end){
		std::cout << "impossible" << std::endl;
	} else {
		int time = 0;
		while(boxes.size() > 0){
			for(int friend_nr = n-1; friend_nr >= 0; friend_nr--){
				std::multiset<int>::iterator it = boxes.begin();
			
				if (*it <= friends[friend_nr]){
					std::multiset<int>::iterator endd = boxes.end();
					endd--;
					if (*endd > friends[friend_nr]){
						std::multiset<int>::iterator element = boxes.find(*boxes.upper_bound(friends[friend_nr]));
						element--;
						boxes.erase(element);
					} else {
						std::multiset<int>::iterator element = boxes.end();
						element--;
						boxes.erase(element);
					}
					if (boxes.empty()){
						break;
					}
				} else {
					break;
				}

			}
			time += 3;
		}
		std::cout << time-1 << std::endl;
	}
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
