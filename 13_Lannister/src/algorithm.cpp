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

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET> &x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase() {
	Program lp (CGAL::SMALLER, false, 0, false, 0); 

	//Variables
	int bs = 0; int cs = 1; int c_ = 2; int d = 3;

	int n, m; long s; std::cin >> n >> m >> s;

	int const_nr = 0;
	long x_sum = 0;
	long y_sum = 0;
	//First read in nobel houses
	for (int i = 0; i < n; i++){
		int x, y; std::cin >> x >> y;
		lp.set_a(bs, const_nr, -y); lp.set_a(cs, const_nr, -1); lp.set_a(c_, const_nr, 0); lp.set_a(d, const_nr, 0); lp.set_b(const_nr, -x);
		const_nr++;
		lp.set_a(bs, const_nr, -x); lp.set_a(cs, const_nr, 0); lp.set_a(c_, const_nr, -1); lp.set_a(d, const_nr, -1); lp.set_b(const_nr, y);
		const_nr++;
		lp.set_a(bs, const_nr, x); lp.set_a(cs, const_nr, 0); lp.set_a(c_, const_nr, 1); lp.set_a(d, const_nr, -1); lp.set_b(const_nr, -y);
		const_nr++;
		
		x_sum -= x;
		y_sum += y;
		
	}

	//Read in common houses
	for (int i = 0; i < m; i++){
		int x, y; std::cin >> x >> y;
		lp.set_a(bs, const_nr, y); lp.set_a(cs, const_nr, 1); lp.set_a(c_, const_nr, 0); lp.set_a(d, const_nr, 0); lp.set_b(const_nr, x);
		const_nr++;
		lp.set_a(bs, const_nr, -x); lp.set_a(cs, const_nr, 0); lp.set_a(c_, const_nr, -1); lp.set_a(d, const_nr, -1); lp.set_b(const_nr, y);
		const_nr++;
		lp.set_a(bs, const_nr, x); lp.set_a(cs, const_nr, 0); lp.set_a(c_, const_nr, 1); lp.set_a(d, const_nr, -1); lp.set_b(const_nr, -y);
		const_nr++;

		x_sum += x;
		y_sum -= y;
		
	}

	lp.set_l(d, true, 0);
	lp.set_c(d, 1);

	Solution sol = CGAL::solve_linear_program(lp, ET());
	if (sol.is_infeasible()){ //Sewage pipe can't separate the houses
		std::cout << "Yuck!" << std::endl;
	} else { //We need to check if we are bankrupt
		if (s == -1){ //No cost constraint
			double sol_ = ceil_to_double(sol.objective_value());
			long sol_long = sol_;
			std::cout << sol_long << std::endl;
		} else {
			lp.set_a(bs, const_nr, y_sum); lp.set_a(cs, const_nr, (n-m)); lp.set_a(c_, const_nr, 0); lp.set_a(d, const_nr, 0); lp.set_b(const_nr, s - x_sum);
			const_nr++;
			Solution sol_bankrupt = CGAL::solve_linear_program(lp, ET());
			if (sol_bankrupt.is_infeasible()){
				std::cout << "Bankrupt!" << std::endl;
			}else {
				if (sol_bankrupt.is_optimal()){
					double sol_bank = ceil_to_double(sol_bankrupt.objective_value());
					long sol_long = sol_bank;
					std::cout << sol_long << std::endl;
				}
			}
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

//Math: 
//Separation nobel from common:
//sewage pipe: a_sx+b_sy+c_s = 0
//water pipe : a_wx+b_wy+c_w = 0
//x-coordinate on sewage line: x = frac{-c_s-b_sy}{a_s} --> a_s just scales, thus we define a_s = -1. It follows x = c_s+b_sy
//Constraints: nobel: x \leq c_s+b_sy, common: x \geq c_s+b_sy

//Minimizing distance d to waterpipe:
//we know water pipe and sewage pipe are perpendicular, thus <a_s,b_s>*<a_w,b_w> = 0 it follows a_w = b_s*b_w
//Here again we have a free parameter, we set a_w = 1, it follows b_w = \frac{1}{b_s}
//y-coordinate on water line: y = \frac{-c_w-a_wx}{b_w} it follows y = \underbrace{-b_wb_s}_\text{c_}-b_sx
//distance d from any point to waterpipe is to be minimized, thus we search min d such that distance \leq for every point:
//Point over waterline: d \geq p_y - (c_ - b_sx), Point below waterline: d \geq (c_-b_sx)-p_y 
//Constraints: -p_y \geq -d -c_+b_sy and p_y \geq -d + c_ -b_sx

//Cost constraint:
//We need to build a constraint on the sum of the sewage pipe-lengths
//pipe-length nobel house: dist = c_s+bs_y-p_x, pipe-length for common house: dist = p_x - (c_s+b_sy)
//We just summ up all the lengths and build the constraint out of it: 
//1. sum_x = \sum_\text{nobel} -x_i + \sum_\text{common} +x_i
//2. sum_y = \sum_\text{nobel} +y_i + \sum_\text{common} -y_i
//3. c = (n-m)
//Constraint: sum_y*b_s+c(n-m) = s - x_sum