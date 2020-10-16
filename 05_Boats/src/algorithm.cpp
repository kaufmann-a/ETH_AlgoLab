#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

struct Boat {
	int nr;
	std::pair<int, int> pos;

	Boat(int nr, std::pair<int, int> pos) : nr(nr), pos(pos) {}
};

//Order boats by end of the boat
bool operator < (const Boat &p1, const Boat &p2){
	return p1.pos.second < p2.pos.second;
}


void testcase() {
	int n; std::cin >> n;

	std::vector<Boat> boats;
	for (int i = 0; i < n; i++){
		int l, p; std::cin >> l >> p;
		for (int j = 0; j <= l; j++){
			boats.push_back(Boat(i, std::make_pair(p-l+j, p+j)));
		}
	}

	std::sort(boats.begin(), boats.end());
	// for (const Boat &boat : boats){
	// 	std::cout << boat.pos.first << " " << boat.pos.second << std::endl;
	// }

	std::vector<bool> boatPlaced(n, false);
	int nrBoatsPlaced = 1;
	int curRightEdge = boats[0].pos.second;
	boatPlaced[0] = true;
	for (const Boat &boat : boats){
		if (boat.pos.first >= curRightEdge && !boatPlaced[boat.nr]){
			nrBoatsPlaced++;
			curRightEdge = boat.pos.second;
			boatPlaced[boat.nr] = true;
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
