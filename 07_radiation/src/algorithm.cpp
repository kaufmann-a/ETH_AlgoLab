#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void setup_lp(Program &lp, std::vector<std::vector<double>> &cells, int const_start, int degree, int invert){
	for (size_t i = 0; i < cells.size(); i++){
		double x = cells[i][0]; double y = cells[i][1]; double z = cells[i][2];
		int dim_count = 1;
		for (int v1 = 0; v1 <= degree; v1++){
			for (int v2 = 0; v2 <= degree-v1; v2++){
				for (int v3 = 0; v3 <= degree-v1-v2; v3++){
					double coeff = invert*std::pow(x, v1)*std::pow(y, v2)*std::pow(z, v3);
					lp.set_a(dim_count, const_start, coeff);
					dim_count++;
				}
			}	
		}
		lp.set_a(0, const_start, 1);
		const_start++;
	}
}


void testcase() {
	int h, t; std::cin >> h >> t;
	
	std::vector<std::vector<double>> healthy(h, std::vector<double>(3));
	std::vector<std::vector<double>> tumor(t, std::vector<double>(3));
	
	for (int i = 0; i < h; i++){
		double x, y, z; std::cin >> x >> y >> z;
		healthy[i][0] = x;
		healthy[i][1] = y; 
		healthy[i][2] = z;
	}

	for (int i = 0; i < t; i++){
		double x, y, z; std::cin >> x >> y >> z;
		tumor[i][0] = x;
		tumor[i][1] = y; 
		tumor[i][2] = z;
	}

	//Do binsearch on the nr. of degrees needed
	int left = 0; int right = 30;
	int lowest = 31;
	while (left < right) {
		int mid = left + (right-left)/2;
		Program lp (CGAL::SMALLER, false, 0, false, 0);
		setup_lp(lp, healthy, 0, mid, 1);
		setup_lp(lp, tumor, healthy.size(), mid, -1);
		lp.set_l(0, true, 1);
		CGAL::Quadratic_program_options options;
		options.set_pricing_strategy(CGAL::QP_BLAND);
		Solution sol = CGAL::solve_linear_program(lp, ET(), options);

		if (!sol.is_infeasible()){
			lowest = mid;
			right = mid;
		} else {
			left = mid+1;
		}
	}
	if (lowest == 31){
		std::cout << "Impossible!" << std::endl;
	} else {
		std::cout << lowest << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in ("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
