#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>
#include <map>

struct edge_component_t {
	typedef boost::edge_property_tag kind;
};

void testcase() {
	//Typedefs
	edge_component_t edge_component;
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<edge_component_t, std::size_t>> graph;
	typedef boost::graph_traits<graph>::edge_iterator edge_it;

	//Read graph
	int n, m; std::cin >> n; std::cin >> m;
	graph G(n);

	for (int i = 0; i < m; i++){
		int e1, e2; std::cin >> e1; std::cin >> e2;
		boost::add_edge(e1, e2, G);
	}

	//Calculate biconnected components
	boost::property_map<graph, edge_component_t>::type component = boost::get(edge_component, G);
	std::size_t num_cops = boost::biconnected_components(G, component);

	//Insert all vertexes to component they belong to
	std::vector<std::set<int>> list(num_cops);
	edge_it e_beg, e_end;
	for (boost::tie(e_beg, e_end) = boost::edges(G); e_beg != e_end; ++e_beg){
		//std::cout << boost::source(*e_beg, G) << " " << boost::target(*e_beg, G) << " Component: " << component[*e_beg] << std::endl;
		int comp = component[*e_beg];
		int source = boost::source(*e_beg, G);
		int target = boost::target(*e_beg, G);
		list[comp].insert(source);
		list[comp].insert(target);
	}

	//Search bridges
	std::map<int, std::set<int>> printout;
	int nrBridges = 0;
	for (std::vector<std::set<int>>::iterator it = list.begin(); it != list.end(); it++){
		std::set<int> cur = *it;
		if (cur.size() == 2){
			std::set<int>::iterator it_s = cur.begin();
			int first = *it_s;
			it_s++;
			int second = *it_s; 
			if (first < second){
				if (printout.find(first) == printout.end()){
					std::set<int> temp;
					temp.insert(second);
					printout.insert(std::make_pair(first, temp));
					nrBridges++;
				} else {
					printout[first].insert(second);
					nrBridges++;
				}
			} else {
				if (printout.find(second) == printout.end()){
					std::set<int> temp;
					temp.insert(first);
					printout.insert(std::make_pair(second, temp));
					nrBridges++;
				} else {
					printout[second].insert(first);
					nrBridges++;
				}
			}
		}
	}

	//Output bridges
	std:: cout << nrBridges << std::endl;
	for (std::map<int, std::set<int>>::iterator it = printout.begin(); it != printout.end(); it++){
		int frst = it->first;
		std::set<int> curSet = it->second;
		for (std::set<int>::iterator it_s = curSet.begin(); it_s != curSet.end(); it_s++){
			std::cout << frst << " " << *it_s << std::endl;
		}
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
