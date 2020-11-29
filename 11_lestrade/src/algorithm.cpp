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
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef long                      Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	long z, u, v, w; std::cin >> z >> u >> v >> w;
	long a, g; std::cin >> a >> g;

	// create an LP with Ax <= b, lower bound 0 and no upper bounds
  	Program lp (CGAL::SMALLER, true, 0, true, 24);
	lp.set_b(0, -u);
	lp.set_b(1, -v);
	lp.set_b(2, -w);

	std::vector<std::pair<K::Point_2, Index>> gang;
	std::vector<std::tuple<long, long, long, long>> gang_info(g);
	gang.reserve(g);
	for (int i = 0; i < g; i++){
		long x_g, y_g, u_g, v_g, w_g; std::cin >> x_g >> y_g >> u_g >> v_g >> w_g;
		gang_info[i] = std::make_tuple(u_g, v_g, w_g, std::numeric_limits<long>::max());
		gang.emplace_back(K::Point_2(x_g, y_g), i);
	}
	Delaunay t;
	t.insert(gang.begin(), gang.end());

	for (int i = 0; i < a; i++){
		long x_a, y_a, z_a; std::cin >> x_a >> y_a >> z_a;
		K::Point_2 loc = K::Point_2(x_a, y_a);
		auto g_member = t.nearest_vertex(loc);
		long index = g_member->info();
		if (std::get<3>(gang_info[index]) > z_a){
			std::get<3>(gang_info[index]) = z_a;
		}
	}

	int counter = 0;
	for (int i = 0; i < g; i++){
		long wage = std::get<3>(gang_info[i]);
		if (wage != std::numeric_limits<long>::max()){
			long u_cur = std::get<0>(gang_info[i]);
			long v_cur = std::get<1>(gang_info[i]);
			long w_cur = std::get<2>(gang_info[i]);
			lp.set_a(counter, 0, -u_cur);
			lp.set_a(counter, 1, -v_cur);
			lp.set_a(counter, 2, -w_cur);
			lp.set_c(counter, wage);
			counter++;
		}
	}

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()){
		std::cout << "H" << std::endl;
	} else if (sol.is_optimal()) {
		if (CGAL::to_double(sol.objective_value()) <= z){
			//std::cout << CGAL::to_double(sol.objective_value()) << std::endl;
			std::cout << "L" << std::endl;
		} else {
			std::cout << "H" << std::endl;
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
