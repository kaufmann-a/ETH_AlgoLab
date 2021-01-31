#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>		graph;		// Use vecS for the VertexList! Choosing setS for the OutEdgeList disallows parallel edges.	
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::out_edge_iterator		out_edge_it;		// to iterate over all edges

int SIDE1 = -1; int SIDE2 = 1;

bool isBipartite(graph &G, std::vector<int> &side, int &size_side1, int &size_side2){
	bool bipartite = true;
	std::vector<bool> visited(boost::num_vertices(G));
	//Iterate over all verteces
	for (uint i = 0; i < boost::num_vertices(G); i++){
		if (!visited[i]){ //Just procede if not yet visited
			std::queue<int> Q;
			Q.push(i);
			side[i] = SIDE1; size_side1++;
			while(!Q.empty()){
				int first = Q.front(); Q.pop(); visited[first] = true;
				int side_first = side[first];
				
				out_edge_it oe_beg, oe_end;
				for(boost::tie(oe_beg, oe_end) = boost::out_edges(first, G); oe_beg != oe_end; ++oe_beg){
					int target = boost::target(*oe_beg, G);
					if (visited[target]){ //If already visited check if right side
						if (!(side[target] == -1*side_first)){ //Must be on other side -1*side flips side
							bipartite = false;
							break;
						}
					} else { //Was not yet visited, assign side and push to queue
						int newSide = -1*side_first;
						side[target] = newSide;
						Q.push(target);
						if (newSide == SIDE1){
							size_side1++;
						} else {
							size_side2++;
						}
					}
				}
			}
		}
	}
	return bipartite;
}

bool checkValiditySide (Delaunay &t_side, long r){
	bool valid = true;
	for (auto e = t_side.finite_edges_begin(); e != t_side.finite_edges_end(); ++e) {
		double length = t_side.segment(e).squared_length();
		if (length <= r){
			valid = false;
			break;
		}
	}
	return valid;
}

void testcase() {
	size_t n, m; long r; std::cin >> n >> m >> r;
	r = r*r; //We need squared length from r

	//Read in stations and build delaunay triangulation
	typedef std::pair<K::Point_2,Index> IPoint;
	std::vector<IPoint> points;
	points.reserve(n);
	for (Index i = 0; i < n; ++i) {
		int x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}
	Delaunay t;
	t.insert(points.begin(), points.end());

	//Iterate over finite edges and build boost graph from it
	graph G(n);
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		double length = t.segment(e).squared_length();
		if (length <= r){
			boost::add_edge(i1, i2, G);
		}
	}

	//Check if graph is bipartite
	std::vector<int> side(n, 0);
	int size_side1 = 0; int size_side2 = 0;
	bool bipartite = isBipartite(G, side, size_side1, size_side2);

	//If bipartite, iterate over all verteces again, build delaunay for each side and check if network valid
	bool valid_network = false;
	if (bipartite){
		std::vector<IPoint> points_side1;
		points_side1.reserve(size_side1);

		std::vector<IPoint> points_side2;
		points_side2.reserve(size_side2);

		for(size_t i = 0; i < n; i++){
			int ind = points[i].second;
			if (side[ind] == SIDE1){
				points_side1.emplace_back(points[i]);
			} else {
				points_side2.emplace_back(points[i]);
			}
		}

		Delaunay t_side1;
		t_side1.insert(points_side1.begin(), points_side1.end());
		bool side1_valid = checkValiditySide(t_side1, r);

		Delaunay t_side2;
		t_side2.insert(points_side2.begin(), points_side2.end());
		bool side2_valid = checkValiditySide(t_side2, r);

		if (side1_valid && side2_valid){
			valid_network = true;
		}
	}

	//If valid network runn connected components
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	if (valid_network){
		boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
	}
	
	//Read in clues
	for (size_t i = 0; i < m; i++){
		long x_a, y_a, x_b, y_b; std::cin >> x_a >> y_a >> x_b >> y_b;
		K::Point_2 pos_a = K::Point_2(x_a, y_a);
		K::Point_2 pos_b = K::Point_2(x_b, y_b);

		//First check if a and b are in range
		bool can_communicate = false;
		if (valid_network){
			if (CGAL::squared_distance(pos_a, pos_b) <= r){
				can_communicate = true;
			} else {
				int closest_stat_a = t.nearest_vertex(pos_a)->info();
				int closest_stat_b = t.nearest_vertex(pos_b)->info();
				K::Point_2 closest_stat_a_point = points[closest_stat_a].first;
				K::Point_2 closest_stat_b_point = points[closest_stat_b].first;
				if (CGAL::squared_distance(pos_a, closest_stat_a_point) <= r && CGAL::squared_distance(pos_b, closest_stat_b_point) <= r){
					if (component_map[closest_stat_a] == component_map[closest_stat_b]){
						can_communicate = true;
					}
				}
			}
		}
		
		if (can_communicate){
			std::cout << "y";
		} else {
			std::cout << "n";
		}
	}
	std::cout << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test4.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
