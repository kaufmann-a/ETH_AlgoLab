#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::property<boost::vertex_out_degree_t, int>, boost::property<boost::edge_weight_t, long>> weighted_graph;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::vertex_iterator vertex_it;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

void testcase() {
	int n, m, x, k; std::cin >> n >> m >> x >> k;

	//First read in input to create graph
	weighted_graph game_board(n);
	for (int i = 0; i < m; i++){
		int u, v; long p; std::cin >> u >> v; std::cin >> p;
		boost::add_edge(u, v, p, game_board);
	}

	//Now, find all Weayaya verteces
	vertex_it v_beg, v_end;
	for (boost::tie(v_beg, v_end) = boost::vertices(game_board); v_beg != v_end; ++v_beg){
		int whatever = 0;
		out_edge_it oe_beg, oe_end;
		int nr_out_edges = 0;
		for (boost::tie(oe_beg, oe_end) = boost::out_edges(*v_beg, game_board); oe_beg != oe_end; ++oe_beg){
			assert(boost::source(*oe_beg, game_board) == *v_beg);
			nr_out_edges++;
		}
		boost::put(boost::vertex_out_degree_t(), game_board, *v_beg, nr_out_edges);
	}

	//Create new graph without edges having no out edges
	weighted_graph game_board_opt(n);
	vertex_it v_beg_store, v_beg_2, v_end_2;
	boost::tie(v_beg_store, v_end_2) = boost::vertices(game_board);
	for (boost::tie(v_beg_2, v_end_2) = boost::vertices(game_board); v_beg_2 != v_end_2; ++v_beg_2){
		int out_edges = boost::get(boost::vertex_out_degree_t(), game_board, *v_beg_2);
		if (out_edges != 0){
			out_edge_it oe_beg, oe_end;
			for (boost::tie(oe_beg, oe_end) = boost::out_edges(*v_beg_2, game_board); oe_beg != oe_end; ++oe_beg){
				if (boost::get(boost::vertex_out_degree_t(), game_board, boost::target(*oe_beg, game_board)) == 0){
					boost::add_edge(*v_beg_2, 0, boost::get(boost::edge_weight_t(), game_board, *oe_beg),game_board_opt);
				} else {
					boost::add_edge(boost::source(*oe_beg, game_board), boost::target(*oe_beg, game_board), boost::get(boost::edge_weight_t(), game_board, *oe_beg), game_board_opt);				
				}
			}
		}
	}

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
