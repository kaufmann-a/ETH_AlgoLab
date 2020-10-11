#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void testcase() {
	int n, c, f;
	std::cin >> n; std::cin >> c; std::cin >> f;
	graph G(n);

	//Read in all students
	std::vector<std::set<std::string>> students(n);
	for (int i = 0; i < n; i++){ //Loop over all students
		for (int j = 0; j < c; j++){ //Read all properties from student i
			std::string prop; std::cin >> prop;
			students[i].insert(prop);
		}

		for (int j = 0; j < i; j++){ //Loop over all previously read students
			int nr_common = 0;
			std::set<std::string>::iterator it_j = students[j].begin();
			std::set<std::string>::iterator it_i = students[i].begin();
			while (it_i != students[i].end()){
			//for (std::set<std::string>::iterator it_i = students[i].begin(); it_i != students[i].end(); it_i++){ //Compare properties of student i with properties of student j
				while(it_j != students[j].end() && *it_j < *it_i){
					it_j++;
				}
				if (it_j != students[j].end() && it_i != students[i].end()){
					if (*it_j == *it_i){
						nr_common++;
					}
				}
				
				it_i++;
			}
			if (nr_common > f){
				boost::add_edge(i, j, G);
			}
		}
	}

	//Run Edmonds algorithm
	std::vector<vertex_desc> mate_map(n);
	//const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
	boost::edmonds_maximum_cardinality_matching(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
	int matching_size = boost::matching_size(G, boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

	if (matching_size == n/2){
		std::cout << "not optimal" << std::endl;
	} else {
		std::cout << "optimal" << std::endl;
	}
	return;	
}

int main() { 
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t; 	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
