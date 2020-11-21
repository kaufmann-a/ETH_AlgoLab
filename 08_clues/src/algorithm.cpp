#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>
#include <queue>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

bool is_bipartite_bfs(const graph &G, std::vector<long> &vertex_side, long n, long &count0, long &count1){
	std::queue<int> Q;
	bool isBipartite = true;
	for (int i = 0; i< n; i++){
		if (vertex_side[i] == -1){
			Q.push(i);
			vertex_side[i] = 0;
			count0++;
			while (!Q.empty()){
				int curVertex = Q.front();
				int curSide = vertex_side[curVertex];
				Q.pop();
				out_edge_it oe_beg, oe_end;
				for (boost::tie(oe_beg, oe_end) = boost::out_edges(curVertex, G); oe_beg != oe_end; ++oe_beg){
					long neighbour = boost::target(*oe_beg, G);
					int sideNeighbour = (curSide+1) % 2;
					if (vertex_side[neighbour] == -1){
						vertex_side[neighbour] = sideNeighbour;
						if (sideNeighbour == 0) count0++;
						if (sideNeighbour == 1) count1++;
						Q.push(neighbour);
					} else {
						if (vertex_side[neighbour] != sideNeighbour) {
							isBipartite = false;
							break;
						}
					}
				}
			}
		}
	}
	return isBipartite;
}

void testcase() {
	long n, m, r; std::cin >> n >> m >> r;
	r = r*r; //We just need r squared

	// read points: first, we read all points and store them into a vector,
	// together with their indices
	typedef std::pair<K::Point_2,Index> IPoint;
	std::vector<IPoint> points;
	points.reserve(n);
	for (Index i = 0; i < n; ++i) {
		int x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}
	Delaunay t; //Triangulation of all radio stations
	t.insert(points.begin(), points.end());

	graph G(n);

	//Build boost graph, containing all edges between stations which are in range r
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		K::Point_2 p1 = e->first->vertex((e->second+1)%3)->point();
		K::Point_2 p2 = e->first->vertex((e->second+2)%3)->point();

		K::FT dist = CGAL::squared_distance(p1, p2);
		if (dist <= r){
			boost::add_edge(i1, i2, G);
		}
	}

	//Check if graph is bipartite, additionally track which vertex has frequency 0 and which vertex frequency 1
	long count0 = 0, count1 = 0;
	std::vector<long> vertex_side(n, -1); //map to assigne each vertex 0 or 1
	bool isBipartite = is_bipartite_bfs(G, vertex_side, n, count0, count1);
	
	//If the graph is bipartite, check if the stations with same frequencies can't reach any other station with same freq.
	//Therefore build triangulation for each side of bipartite graph and check distance of every edge in Triangulation
	if (isBipartite){
		std::vector<K::Point_2> side0;
		side0.reserve(count0);
		std::vector<K::Point_2> side1;
		side1.reserve(count1);

		for (int i = 0; i < n; i++){
			long curIndex = points[i].second;
			if (vertex_side[curIndex] == 0) side0.push_back(points[i].first);
			else if (vertex_side[curIndex] == 1) side1.push_back(points[i].first);
		}
		Delaunay tr_side0;
		tr_side0.insert(side0.begin(), side0.end());
		Delaunay tr_side1;
		tr_side1.insert(side1.begin(), side1.end());

		for (auto e = tr_side0.finite_edges_begin(); e != tr_side0.finite_edges_end(); ++e){
			if (tr_side0.segment(e).squared_length() <= r) {
				isBipartite = false;
				break;
			}
		}
		for (auto e = tr_side1.finite_edges_begin(); e != tr_side1.finite_edges_end(); ++e){
			if (tr_side1.segment(e).squared_length() <= r) {
				isBipartite = false;
				break;
			}
		}
	}

	
	std::vector<int> component_map(n);	// We MUST use such a vector as an Exterior Property Map: Vertex -> Component
	boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

	for (int i = 0; i < m; i++){
		long a_x, a_y, b_x, b_y; std::cin >> a_x >> a_y >> b_x >> b_y;
		K::Point_2 a = K::Point_2(a_x, a_y);
		K::Point_2 b = K::Point_2(b_x, b_y);

		if (!isBipartite) {
			std::cout << "n";
		} else if(CGAL::squared_distance(a, b) <= r){
			std::cout << "y";
		} else {
			Index i1 = t.nearest_vertex(a)->info();
			Index i2 = t.nearest_vertex(b)->info();
			K::FT dist_a = CGAL::squared_distance(t.nearest_vertex(a)->point(), a);
			K::FT dist_b = CGAL::squared_distance(t.nearest_vertex(b)->point(), b);
			if (dist_a <= r && dist_b <= r){
				if (component_map[i1] == component_map[i2]){
					std::cout << "y";
				} else {
					std::cout << "n";
				}
			} else {
				std::cout << "n";
			}
		}
	}
	std::cout << std::endl;
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
