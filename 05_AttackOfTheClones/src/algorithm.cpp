#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

bool sort_e(std::pair<int, int> &p1, std::pair<int, int> &p2){
	return p1.second < p2.second;
}

int schedule(std::vector<std::pair<int, int>> &jedis, int startPos, int endPos, int counter){
	int index = 0; int left = startPos; int nrJedis = counter;
	while (jedis[index].second < startPos){ //Find first jedi we could potentially use
		index++;
		if (index == jedis.size()){
			index--;
			break;
		}
	}

	while (jedis[index].second <= endPos && index < jedis.size()){
		if (jedis[index].first >= left && jedis[index].first <= jedis[index].second){ //We can use the yedi
			counter++;
			left = jedis[index].second +1;
		}
		index++;
	}
	return counter;
}

void testcase() {
	int n, m; std::cin >> n >> m;

	std::vector<std::pair<int, int>> sort_end;
	
	std::pair<int, int> earliest_finish = std::make_pair(-1, m);
	std::pair<int, int> latest_start = std::make_pair(0, -1);
	std::pair<int, int> shortest_seg = std::make_pair(0, m+1);
	bool found_overlap = false;
	for (int i = 0; i < n; i++){
		int a, b; std::cin >> a >> b;
		std::pair<int, int> p = std::make_pair(a, b);
		sort_end.push_back(p);
		if (a > b){
			found_overlap = true;
			if (p.second < earliest_finish.second) earliest_finish = p;
			if (p.first > latest_start.first) latest_start = p;	
			if (p.second - p.first+1  < shortest_seg.second - shortest_seg.first+1) shortest_seg = p;
		}
	}

	std::sort(sort_end.begin(), sort_end.end(), sort_e);
	int max = 0;
	if (found_overlap){
		int earliest_fin = schedule(sort_end, earliest_finish.second+1, earliest_finish.first-1, 1);
		int late_start = schedule(sort_end, latest_start.second+1, latest_start.first-1, 1);
		int short_seg = schedule(sort_end, shortest_seg.second+1, shortest_seg.first-1, 1);
		max = std::max(std::max(earliest_fin, late_start), short_seg);
	}
	max = std::max(max, schedule(sort_end, 1, m, 0));
	std::cout << max << std::endl;
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
