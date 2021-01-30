#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET> &x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


void testcase() {
	// create an LP with Ax <= b, lower bound 0 and no upper bounds
	Program lp (CGAL::SMALLER, true, 0, false, 0); 

	int n, m, c; std::cin >> n >> m >> c;
	int curConstraintNr = 0;

  	std::vector<K::Point_2> warehouses;
  	std::vector<K::Point_2> all_pts;
	std::vector<int> pureAlcoholinBeer;
  	warehouses.reserve(n);
	all_pts.reserve(n+m);
	//Read in warehousese
	for (int i = 0; i < n; i++){
		long x, y; int s, a; std::cin >> x >> y >> s >> a;
		warehouses.push_back(K::Point_2(x,y));
		all_pts.push_back(K::Point_2(x,y));
		for (int j = 0; j < m; j++){
			int curVar = i + n*j;
			lp.set_a(curVar, curConstraintNr, 1);
		}
		lp.set_b(curConstraintNr, s);
		pureAlcoholinBeer.push_back(a);
		curConstraintNr++;
	}

	//Read in the stadiums
  	std::vector<K::Point_2> stadiums;
  	stadiums.reserve(n);
	for (int i = 0; i < m; i++){
		long x, y; int d, u; std::cin >> x >> y >> d >> u;
		stadiums.push_back(K::Point_2(x, y));
		all_pts.push_back(K::Point_2(x, y));
		for (int j = 0; j < n; j++){
			int curVar = i*n+j;
			lp.set_a(curVar, curConstraintNr, 1);
			lp.set_a(curVar, curConstraintNr+1, -1);
			lp.set_a(curVar, curConstraintNr+2, pureAlcoholinBeer[j]);
		}
		lp.set_b(curConstraintNr, d);
		lp.set_b(curConstraintNr+1, -d);
		lp.set_b(curConstraintNr+2, 100*u);
		curConstraintNr += 3;
	}

	//Read in revenues
	std::vector<std::vector<double>> revenues(n, std::vector<double>(m, 0));
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			double rev; std::cin >> rev;
			revenues[i][j] = rev;
		}
	}

	//Read in contour lines
	Triangulation tri;
	tri.insert(all_pts.begin(), all_pts.end());

	for (int k = 0; k < c; k++){
		long x, y, r; std::cin >> x >> y >> r;
		K::Point_2 center = K::Point_2(x, y);
		K::Point_2 nearestVertex = tri.nearest_vertex(center)->point();
		if (CGAL::squared_distance(center, nearestVertex) <= r*r){
			//Check every stadium - warehouse pair
			for (int i = 0; i < n; i++){
				K::Point_2 cur_war = warehouses[i];
				bool war_in_circle = false;
				if (CGAL::squared_distance(center, cur_war) <= r*r){
					war_in_circle = true;
				}
				for (int j = 0; j < m; j++){
					K::Point_2 cur_stad = stadiums[j];
					bool stad_in_circle = false;
					if (CGAL::squared_distance(center, cur_stad) <= r*r){
						stad_in_circle = true;
					}
					if (war_in_circle != stad_in_circle){ //We have to cross contour
						revenues[i][j] -= 0.01;
					}
				}
			}
		}
	}

	//Create objective function
	for (int i = 0; i < n; i++){
		for (int j = 0; j < m; j++){
			int curVar = i+j*n;
			lp.set_c(curVar, -revenues[i][j]);
		}
	}

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_infeasible()){
		std::cout << "RIOT!" << std::endl;
	} else if (s.is_optimal()){
		CGAL::Quotient<ET> sol = -s.objective_value();
		double max = floor_to_double(-s.objective_value());
		long maxrev= max;
		std::cout << maxrev << std::endl;
	}
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test3.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
