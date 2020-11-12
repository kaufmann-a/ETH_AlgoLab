#include<iostream>
#include<vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void buildLP(Program &lp, int nrPoints, std::vector<std::vector<double>> &points, int lpStartLine, int d, int multiplicator){
	for (int i = 0; i < nrPoints; i++){
    	int lpColNr = 1;
		for (int d1 = 0; d1 <= d; d1++){
			for (int d2 = 0; d2 <= d - d1; d2++){
				for (int d3 = 0; d3 <= d - d2 - d1; d3++){
					lp.set_a(lpColNr, i+lpStartLine, multiplicator*std::pow(points[i][0], d1)*std::pow(points[i][1], d2)*std::pow(points[i][2], d3));
					std::cout << multiplicator*std::pow(points[i][0], d1)*std::pow(points[i][1], d2)*std::pow(points[i][2], d3) << std::endl;
					lpColNr++;
				}
			}
		}
    	lp.set_a(0, i+lpStartLine, 1);
    	lp.set_b(i+lpStartLine, 0);
  	}
}


void testcase(){
	int h, t; 
	std::cin >> h >> t; 
	std::vector<std::vector<double>> healthy(h, std::vector<double>(3)); 
	std::vector<std::vector<double>> not_healthy(t, std::vector<double>(3)); 
	for(int i = 0; i < h; i++){
		double x, y, z; 
		std::cin >> x >> y >> z; 
		healthy[i][0] = x;
		healthy[i][1] = y;
		healthy[i][2] = z;
  	}
  	for(int i = 0; i < t; i++){
		double x, y, z; 
		std::cin >> x >> y >> z; 
		not_healthy[i][0] = x;
		not_healthy[i][1] = y;
		not_healthy[i][2] = z;
  	}
	
	bool flag = false; 
	for(int i = 0; i < 30; i++){
		Program lp(CGAL::SMALLER, false, 0, false, 0);
		buildLP(lp, h, healthy, 0, i, 1);
		buildLP(lp, t, not_healthy, h, i, -1);
		lp.set_l(0, true, 1); 
		CGAL::Quadratic_program_options options;
		options.set_pricing_strategy(CGAL::QP_BLAND);
		Solution res = CGAL::solve_linear_program(lp, ET(), options);
		if(!res.is_infeasible()){
			flag = true; 
			std::cout << i << std::endl; 
			break;
		}
 	}
  	if(!flag) std::cout << "Impossible!" << std::endl; 
}

int main(){
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}