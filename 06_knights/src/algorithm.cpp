#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property, 
	boost::property<boost::edge_capacity_t, long, 
	boost::property<boost::edge_residual_capacity_t, long, 
	boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

class edge_adder {
	graph &G;

	public:
		explicit edge_adder(graph &G) : G(G) {}
	
	void add_edge(int from, int to, long capacity){
		auto c_map = boost::get(boost::edge_capacity, G);
		auto r_map = boost::get(boost::edge_reverse, G);
		const auto e = boost::add_edge(from, to, G).first;
		const auto rev_e = boost::add_edge(to, from, G).first;
		c_map[e] = capacity;
		c_map[rev_e] = 0;
		r_map[e] = rev_e;
		r_map[rev_e] = e;
	}
};

void testcase() {
	int n, m, k, c; std::cin >> m >> n >> k >> c;

	//Construct cave: Node 0: source, node n*m*2+1: Sink, Node i*j and (i*j)+m*n = intersection i,j
	graph G(n*m*2+2);
	edge_adder adder(G);

	int source = 0;
	int sink = 2*m*n+1;

	for (int i = 0; i < m; i++){
		for (int j = 1; j <= n; j++){
			
			//Check type of intersection
			bool colEdge_left = i == 0;
			bool colEdge_right =  i == m-1;
			bool rowEdge_top = j == 1;
			bool rowEdge_bottom = j == n;

			int curIn = i*m+j;
			int curOut = i+m+j+m*n;
			int inUpperRow = curIn - 1;
			int inLeftColumn = curIn - m;
			int outUpperRow = curOut - 1;
			int outLeftColumn = curOut - m;
			
			adder.add_edge(curIn, curOut, c); //Edges to limit nr of knights passing a vertex
			//Edges connecting intersections
			if (colEdge_left || colEdge_right){ //Corner
				adder.add_edge(curOut, sink, 1);
				adder.add_edge(curOut, sink, 1);
			} 
			if (rowEdge_top || rowEdge_bottom){
				adder.add_edge(curOut, sink, 1);
				adder.add_edge(curOut, sink, 1);
			}

			if (!rowEdge_top){
				adder.add_edge(curOut, inUpperRow, 1);
				adder.add_edge(outUpperRow, curIn, 1);
			} 
			if (!colEdge_left){
				adder.add_edge(curOut, inLeftColumn, 1);
				adder.add_edge(outLeftColumn, curIn, 1);
			} 
		}
	}

	for (int a = 0; a < k; a++){
		int i, j; std::cin >> i >> j;
		int curIn = i*m+j;
		adder.add_edge(source, curIn, 1);
	}

	long flow = boost::push_relabel_max_flow(G, source, sink);

	std::cout << flow << std::endl;

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
