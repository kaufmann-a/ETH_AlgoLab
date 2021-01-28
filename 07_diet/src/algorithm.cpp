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

void testcase(int n, int m) {
	Program lp (CGAL::SMALLER, true, 0, false, 0);

	for (IT i = 0; i < n; i++){
		IT min, max; std::cin >> min >> max;
		lp.set_b(i, -min);
		lp.set_b(n+i, max);
	}

	//Loop over all different meals, each meal is one dimension in the lp
	//Width of matrix A is equal to nr of meals
	//Nr of constrains is 2*Nr of nutritiesnts
	for (int j = 0; j < m; j++){
		IT price; std::cin >> price;
		lp.set_c(j, price);
		for (int i = 0; i < n; i++) {
			int nutri; std::cin >> nutri;
			lp.set_a(j, i, -nutri);
			lp.set_a(j, n+i, nutri);
		}
	}
	//lp.set_c0(0);


	Solution s = CGAL::solve_linear_program(lp, ET());
	if (s.is_optimal()){
		double cost = CGAL::to_double(s.objective_value());
		std::cout << std::floor(cost) << std::endl;
	} else {
		std::cout << "No such diet." << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int n, m; std::cin >> n >> m;
	while ( n != 0 || m != 0){
		testcase(n, m);
		std::cin >> n >> m;
	}
	return 0;
}
