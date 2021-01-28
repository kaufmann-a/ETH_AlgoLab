#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <queue>
#include <vector>
#include <fstream>
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
typedef std::tuple<Index,Index,K::FT> 						   Edge;
typedef std::vector<Edge> 									   EdgeV;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>		graph;
typedef boost::graph_traits<graph>::vertex_descriptor		   vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator			   edge_it;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
	return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int nrFamilies(std::vector<std::vector<std::pair<int, long>>> &G, K::FT &givenSquaredDist, int n, int k){
	std::vector<bool> vis(n, false); 
	std::vector<int> conn_components; 

	for(int i = 0; i < n; i++){
		if(vis[i]) continue; 
		int size = 0; 
		std::queue<int> Q; 
		Q.push(i); 
		while(!Q.empty()){
			int u = Q.front(); Q.pop();
			vis[u] = true; 
			size++; 
			for(int i = 0; i < G[u].size(); i++){
				int v = get<0>(G[u][i]);
				if(!vis[v] && get<1>(G[u][i]) < givenSquaredDist) Q.push(v);
			}
		}
		conn_components.push_back(size); 
	}
	
	int size1 = 0; 
  	int size2 = 0; 
  	int size3 = 0; 
  	int fine = 0; 
  	for(int i = 0; i < conn_components.size(); i++){
    	if(conn_components[i] == 1) size1++; 
    	if(conn_components[i] == 2) size2++; 
    	if(conn_components[i] == 3) size3++; 
    	if(conn_components[i] >= k) fine++; 
  	}
	if(k == 1) return fine; 
  	else if(k == 2) return fine + size1 / 2; 
  	else if(k == 3){
    	if(size2 <= size1) return fine + size2 + (size1 - size2) / 3; 
    	else return fine + size1 + (size2 - size1) / 2; 
  	} else {
		int match2 = size2/2; 
		int match31 = std::min(size1, size3); 
		int rem1 = std::max(0, size1 - size3); 
		int rem2 = size2%2; 
		int rem3 = std::max(0, size3-size1); 
		int extra = 0; 
		if(rem1 > 1) extra = (rem1 + rem2 * 2) / 4; 
		else if(rem3 > 0) extra = (rem2 + rem3) / 2; 
		// else if(rem3 > 0) extra = (3 * rem3 + 2 * rem2) / 4; 
		return extra + match2 + match31 + fine; 
  	}
	
}

void testcase() {
	long n, k, f, s; std::cin >> n >> k >> f >> s;

	typedef std::pair<K::Point_2,Index> IPoint;
  	std::vector<IPoint> points;
  	points.reserve(n);
  	for (Index i = 0; i < n; ++i) {
		long x, y;
		std::cin >> x >> y;
		points.emplace_back(K::Point_2(x, y), i);
	}
	Delaunay t;
	t.insert(points.begin(), points.end());


	EdgeV edges;
  	edges.reserve(3*n); // there can be no more in a planar graph
  	for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
		Index i1 = e->first->vertex((e->second+1)%3)->info();
		Index i2 = e->first->vertex((e->second+2)%3)->info();
		// ensure smaller index comes first
    	if (i1 > i2) std::swap(i1, i2);
    	edges.emplace_back(i1, i2, t.segment(e).squared_length());
  	}
  	std::sort(edges.begin(), edges.end(), [](const Edge& e1, const Edge& e2) -> bool {
		  return std::get<2>(e1) <std::get<2>(e2);
	  });

	std::vector<std::vector<std::pair<int, long>>> G(n);
	boost::disjoint_sets_with_storage<> uf(n);
  	Index n_components = n;
	std::vector<K::FT> emst_edges;
  	for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
		// determine components of endpoints
		Index c1 = uf.find_set(std::get<0>(*e));
		Index c2 = uf.find_set(std::get<1>(*e));
		if (c1 != c2) {
			// this edge connects two different components => part of the emst
			uf.link(c1, c2);
			G[c1].push_back({c2, get<2>(*e)});
      		G[c2].push_back({c1, get<2>(*e)}); 
			emst_edges.push_back(std::get<2>(*e));
			if (--n_components == 1) break;
		}
  	}
	std::sort(emst_edges.begin(), emst_edges.end());
	int l = 0; int r = emst_edges.size()-1;
	long res = 0;
	while (l <= r){
		int m = (l + r) / 2;
    	int now = nrFamilies(G, emst_edges[m], n, k);
    	if(now >= f && m < emst_edges.size() - 1 && nrFamilies(G, emst_edges[m + 1], n, k) < f){ res = emst_edges[m]; break;}  
   		else if(now >= f && m == emst_edges.size() - 1){ res = emst_edges[m]; break;} 
    	else if(now >= f) l = m + 1; 
    	else if(now < f) r = m - 1;

	}
	

	K::FT squaredDist = s;
	int q2 = nrFamilies(G, squaredDist, n, k);
	
	std::cout << res << " " << q2 << std::endl;
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
