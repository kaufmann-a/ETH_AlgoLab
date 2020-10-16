#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

//Order boats by end of the boat
bool sortbysec (const std::pair<int, int> &p1, const std::pair<int, int> &p2){
	return p1.second < p2.second;
}


void testcase() {
	int n; std::cin >> n;

	std::vector<std::pair<int, int>> boats;
	for (int i = 0; i < n; i++){
		int l, p; std::cin >> l >> p;
		boats.push_back(std::make_pair(p-l, p));
	}

	std::sort(boats.begin(), boats.end(), sortbysec);
	int nrBoatsPlaced = 1;
	int curRightEdge = boats[0].second;
	int curBoat = 1;
	while (curBoat < n){
		//Place next boat
		int nextMinRightEdge;
		if (curRightEdge + boats[curBoat].second - boats[curBoat].first < boats[curBoat].second){
			nextMinRightEdge = boats[curBoat].second;
		} else {
			nextMinRightEdge = curRightEdge + boats[curBoat].second - boats[curBoat].first;
		}
		bool needToIterate = false;
		while(nextMinRightEdge > boats[curBoat].second && curBoat < n) {
			//Place boat optimaly
			needToIterate = true;
			int temp;
			if (curRightEdge + boats[curBoat].second - boats[curBoat].first < boats[curBoat].second){
				temp = boats[curBoat].second;
			} else {
				temp = curRightEdge + boats[curBoat].second - boats[curBoat].first;
			}
			if (temp < nextMinRightEdge){
				nextMinRightEdge = temp;
			}
			curBoat++;
		}
		curRightEdge = nextMinRightEdge;
		nrBoatsPlaced++;
		if (!needToIterate){
			curBoat++;
		}
	}
	std::cout << nrBoatsPlaced << std::endl;
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
