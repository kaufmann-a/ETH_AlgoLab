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
#include <limits.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase(int n, int d) {
	
	Program lp (CGAL::SMALLER, false, 0, false, 0);

	for (int i = 0; i < n; i++){
		int curSum = 0;
		for (int j = 0; j < d; j++){
			int a; std::cin >> a;
			lp.set_a(j, i, a);
			curSum += a*a;
		}
		lp.set_a(d, i, std::floor(std::sqrt(curSum)));
		int b; std::cin >>b;
		lp.set_b(i, b);
	}

	lp.set_c(d, -1);
	lp.set_l(d, true, 0);

	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_optimal()){
		double sol = CGAL::to_double(-s.objective_value());
		std::cout << (std::floor(sol)) << std::endl;
	} else if(s.is_infeasible()){
		std::cout << "none" << std::endl;
	} else if (s.is_unbounded()){
		std::cout << "inf" << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int n, d;
	std::cin >> n >> d;
	while (n != 0 && d!= 0){
		testcase(n, d);
		std::cin >> n >> d;
	}
	return 0;
}
