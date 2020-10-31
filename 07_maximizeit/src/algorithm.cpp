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


typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(IT p, IT a, IT b) {

	
	if (p == 1){ //Maximization problem
		Program lp (CGAL::SMALLER, true, 0, false, 0);	
		//set the coefficients of a and b
		const int X = 0;
		const int Y = 1;
		lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4);
		lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);
		lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1);
		lp.set_c(X, a);
		lp.set_c(Y, -b);
		lp.set_c0(0);

		//solve the program using ET as the exact type
		Solution s = CGAL::solve_linear_program(lp, ET());
		if (s.is_optimal()){
			double sol = -1 * CGAL::to_double(s.objective_value());
			std::cout << (int) std::floor(sol) << std::endl;
		} else if (s.is_infeasible()){
			std::cout << "no" << std::endl;
		} else if (s.is_unbounded()){
			std::cout << "unbounded" << std::endl;
		}
	} else if (p == 2){ //Minimization problem
		Program lp (CGAL::SMALLER, false, 0, true, 0);

		//set the coefficeients of a and b
		const int X = 0;
		const int Y = 1;
		const int Z = 2;

		lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_a(Z, 0, 0); lp.set_b(0, 4);
		lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b);
		lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_a(Z, 2, 0); lp.set_b(2, 1);
		lp.set_c(X, a);
		lp.set_c(Y, b);
		lp.set_c(Z, 1);
		lp.set_c0(0);

		Solution s = CGAL::solve_linear_program(lp, ET());
		if (s.is_optimal()){
			double sol = CGAL::to_double(s.objective_value());
			std::cout << (int) std::ceil(sol) << std::endl;
		} else if (s.is_infeasible()){
			std::cout << "no" << std::endl;
		} else if (s.is_unbounded()){
			std::cout << "unbounded" << std::endl;
		}
	}


	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test.in");
	std::cin.rdbuf(in.rdbuf());
	IT p, a, b; std::cin >> p >> a >> b;
	while(p != 0 && a != 0 && b != 0){
		testcase(p, a, b);
		std::cin >> p >> a >> b;
	}
	return 0;
}
