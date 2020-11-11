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
#include <math.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase() {
	long x, y, n; std::cin >> x >> y >> n;
	Program lp (CGAL::SMALLER, false, 0, false, 0);


	IT X = 0; IT Y = 1; IT R = 2;

	for (int i = 0; i < n; i++){
		long a, b, c, v; std::cin >> a >> b >> c >> v;
		
		//First find equation of halfplane where asterix is
		
		if (a*x+b*y+c > 0){ //Turn constraint so that ax+by+c<=0
			a = -a;
			b = -b;
			c = -c;
		}
		double res_rad = std::sqrt(a*a+b*b)/ v;
		lp.set_a(X, i, a/v); lp.set_a(Y, i, b/v); lp.set_a(R, i, res_rad); lp.set_b(i, -c);
	}
	lp.set_c(R, -1);
	lp.set_l(R, true, 0);

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_optimal()){
		double sol = CGAL::to_double(-s.objective_value());
		std::cout << (std::floor(sol)) << std::endl;
	} else if(s.is_infeasible()){
		std::cout << "impossible" << std::endl;
	} else if (s.is_unbounded()){
		std::cout << "impossible" << std::endl;
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
