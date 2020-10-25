#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph; 

void testcase() {
	int w, h; std::cin >> w >> h;
	std::vector<std::vector<char>> garden(h, std::vector<char>(w));
	graph G(w*h);
	int countTiles = 0;
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			char tile; std::cin >> tile;
			garden[i][j] = tile;

			int curNode = i*w+j;
			int nodeOneRowUp = (i-1)*w+j;
			int nodeToLeft = i*w+j-1;
			//Check if current pos needs tile
			if (tile == 'x'){
				continue;
			} else {
				countTiles++;
			}

			if (i != 0){
				if (garden[i-1][j] == '.'){
					boost::add_edge(curNode, nodeOneRowUp, G);
				}			
			}
			if (j != 0){
				if (garden[i][j-1] == '.'){
					boost::add_edge(curNode, nodeToLeft, G);
				}
			}
		}
	}

	std::vector<boost::graph_traits<graph>::vertex_descriptor> mate(w*h);

	boost::checked_edmonds_maximum_cardinality_matching(G, &mate[0]);
	int matchingsize = boost::matching_size(G, &mate[0]);
	if (countTiles % 2 == 0){
		if (matchingsize == countTiles/2){
			std::cout << "yes" << std::endl;
		} else {
			std::cout << "no" << std::endl;
		}
	} else {
		std::cout << "no" << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in ("./testsets/test2.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
