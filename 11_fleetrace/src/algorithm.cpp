#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase() {
	long b, s, p; std::cin >> b >> s >> p;

	// create an LP with Ax <= b, lower bound 0 and no upper bounds
  	Program lp (CGAL::SMALLER, true, 0, false, 0); 

	long dimNr = 0;
	for (int i = 0; i < p; i++){
		long bi, si, ci; std::cin >> bi >> si >> ci;
		lp.set_a(dimNr, bi, 1);
		lp.set_a(dimNr, b+si, 1);
		lp.set_c(dimNr, -ci);
		//lp.set_u(dimNr, true, 1);
		dimNr++;
	}
	for (int i = 0; i < p; i++){
		lp.set_b(i, 1);
	}
	CGAL::Quadratic_program_options options;
	options.set_pricing_strategy((CGAL::QP_BLAND));
	// solve the program, using ET as the exact type
  	Solution sol = CGAL::solve_linear_program(lp, ET()), options;
	
	if (sol.is_infeasible()){
		std::cout << "This is kind of strange" << std::endl;
	} else if (sol.is_optimal()){
		long val = std::floor(CGAL::to_double(-sol.objective_value()));
		std::cout << val << std::endl;
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
