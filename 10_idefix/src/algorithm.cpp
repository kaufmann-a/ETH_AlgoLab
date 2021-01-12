#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>					graph;
typedef boost::graph_traits<graph>::vertex_descriptor		vertex_desc;		// Vertex Descriptor: with vecS vertex list, this is really just an int in the range [0, num_vertices(G)).	
typedef boost::graph_traits<graph>::edge_iterator		edge_it;		// to iterate over all edges

bool mySort (std::pair<int, long> &bone1, std::pair<int, long> &bone2){
	return bone1.second < bone2.second;
}


int nrBones (Delaunay t, long n, long s, std::vector<std::pair<int, long>> &bones_locations, EdgeV &edges){
	// reachable bones per tree
    std::vector<int> bones_count(n);
    for (auto &bone : bones_locations)
    {
        if (bone.second * 4 <= K::FT(s))
        {
            bones_count[bone.first]++;
        }
    }
	
	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;
	// ... and process edges in order of increasing length
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		// determine components of endpoints
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);
			if (std::get<2>(*e) <= s){
				// edge is not to long -> combine the bone count of both components
                int combined_bone_count = bones_count[c1] + bones_count[c2];
                bones_count[c1] = combined_bone_count;
                bones_count[c2] = combined_bone_count;
			}
			if (--n_components == 1) break;
		}
	}
	size_t max_count = 0;
    for (size_t count : bones_count)
    {
        max_count = std::max(max_count, count);
    }
	return max_count;
}

void testcase() {
	long n, m, s, k; std::cin >> n >> m >> s >> k;

	//Read in all oak trees and build delaunay triangulation
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

	EdgeV edges;
	std::vector<long> possible_distances_q;
	graph G(n);
	edges.reserve(3*n); // there can be no more in a planar graph
	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		K::Point_2 p1 = e->first->vertex((e->second+1)%3)->point();
		K::Point_2 p2 = e->first->vertex((e->second+2)%3)->point();
		// ensure smaller index comes first
		if (i1 > i2) std::swap(i1, i2);
		edges.emplace_back(i1, i2, t.segment(e).squared_length());
		if (CGAL::squared_distance(p1, p2) <= s){
			boost::add_edge(i1, i2, G);
		}
		possible_distances_q.push_back(t.segment(e).squared_length());
	}
	std::sort(edges.begin(), edges.end(),
		[](const Edge& e1, const Edge& e2) -> bool {
			return std::get<2>(e1) < std::get<2>(e2);
			});

	std::vector<std::pair<int, long>> bones_locations; //Bone i is closest to tree j with dist k: bones_locations[i]=(j,k)
	std::vector<int> trees_with_bones(n, 0); //Tree i has j bones with smaller r: trees_with_bones[i] = j
	for (int i = 0; i < m; i++){
		int x, y; std::cin >> x >> y;
		K::Point_2 curBone = K::Point_2(x, y);

		auto nearestVertex = t.nearest_vertex(curBone);
		Index node_nr = nearestVertex->info();
		K::Point_2 pt = nearestVertex->point();
		possible_distances_q.push_back(4*CGAL::squared_distance(curBone, pt));
		//auto dista = CGAL::to_double(CGAL::squared_distance(curBone, pt));
		bones_locations.push_back(std::make_pair(node_nr, CGAL::squared_distance(curBone, pt)));
		long radius = s/4;
		if (CGAL::squared_distance(curBone, pt) <= radius){
			trees_with_bones[node_nr] += 1;
		}
	}

	
	// reachable bones per tree
    std::vector<int> bones_count(n);
    for (auto &bone : bones_locations)
    {
        if (bone.second * 4 <= K::FT(s))
        {
            bones_count[bone.first]++;
        }
    }
	
	boost::disjoint_sets_with_storage<> uf(n);
	Index n_components = n;
	// ... and process edges in order of increasing length
	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		// determine components of endpoints
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);
			if (std::get<2>(*e) <= s){
				// edge is not to long -> combine the bone count of both components
                int combined_bone_count = bones_count[c1] + bones_count[c2];
                bones_count[c1] = combined_bone_count;
                bones_count[c2] = combined_bone_count;
			}
			if (--n_components == 1) break;
		}
	}
	size_t max_count = 0;
    for (size_t count : bones_count)
    {
        max_count = std::max(max_count, count);
    }

	std::sort(possible_distances_q.begin(), possible_distances_q.end());

	//Do binsearch on the radius
	long left = 0; long right = possible_distances_q.size()-1;
	while (left < right){
		long middle_index = left+(right-left)/2;
		long middle = possible_distances_q[middle_index];

		int best = nrBones(t, n, middle, bones_locations, edges);
		if (best >= k){
			right = middle_index;
		} else {
			left = middle_index+1;
		}

	}

	std::cout << max_count << " " << possible_distances_q[right] << std::endl;
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
