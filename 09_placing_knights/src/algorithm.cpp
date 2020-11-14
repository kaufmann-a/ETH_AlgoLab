#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

	return matching_size;
}

void testcase() {
	int n; std::cin >> n;
	std::vector<std::vector<bool>> holes(n, std::vector<bool>(n, false));
	int hole_count = 0;
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			int curField; std::cin >> curField;
			if (curField == 1){
				holes[i][j] = true;
				hole_count++;
			}
		}
	}

	graph G(n*n);

	std::vector<int> helper = {-2, -1, 1, 2};
	//Build graph
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			int curPosKnight = n*i+j;

			if (!holes[i][j]){ //Don't process holes
				continue;
			}
			//Each knight can potentially reach 8 fields, check which fealds he can reach and add edges for those fields
			for (int nr1 = 0; nr1 < 4; nr1++){ //find combinations [i-1,j-2],[i-1,j+2],[i+1,j-2],...
				for (int nr2 = 0; nr2 < 4; nr2++){
					if (std::abs(helper[nr1]) + std::abs(helper[nr2]) == 3){ //exclude combinations such as [i-1,j-1]
						if (i+helper[nr1] >= 0 && i+helper[nr1] < n && j+helper[nr2] >= 0 && j+helper[nr2] < n){ //check if field is on ches board
							if (holes[i+helper[nr1]][j+helper[nr2]]){ //Check if field is not a hole
								int targetLine = i+helper[nr1];
								int targetCol = j+helper[nr2];
								boost::add_edge(curPosKnight,n*targetLine+targetCol, G);
							}
						}
					}
				}
			}
		}
	}

	int max_matching = maximum_matching(G);
	std::cout << hole_count - max_matching << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
