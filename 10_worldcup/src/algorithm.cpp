#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <tuple>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Circle_2 C;
typedef K::Segment_2 S;

// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase() {
	long n, m, c; std::cin >> n >> m >> c;
    Program lp (CGAL::SMALLER, false, 0, false, 0);

    int curNrCont = 0;
	//Read in warehouses
	std::vector<std::tuple<K::Point_2, long, long>> warehouses(n);
	for (int warh_nr = 0; warh_nr < n; warh_nr++){
		long x, y, s, a; std::cin >> x >> y >> s >> a;
		warehouses[warh_nr] = std::make_tuple(K::Point_2(x, y), s, a);
        //Sum of suplies to all stadiums must be smallerequal s
        for (int stad_nr = 0; stad_nr < m; stad_nr++){
            int colNr = warh_nr*m+stad_nr;
            lp.set_a(colNr, curNrCont, 1);
        }
        lp.set_b(curNrCont, s);
        curNrCont++;
	}
	//Read in stadiums
	std::vector<std::tuple<K::Point_2, long, long>> stadiums(m);
	for (int stad_nr = 0; stad_nr < m; stad_nr++){
		long x, y, d, u; std::cin >> x >> y >> d >> u;
		stadiums[stad_nr] = std::make_tuple(K::Point_2(x, y), d, u);
        //Sum of supplies mut be smaller d and negative sum must be smaller d
        for (int warh_nr = 0; warh_nr < n; warh_nr++){
            int colNr = warh_nr*m+stad_nr;
            lp.set_a(colNr, curNrCont, 1);
        }
        lp.set_b(curNrCont, d);
        curNrCont++;
        for (int warh_nr = 0; warh_nr < n; warh_nr++){
            int colNr = warh_nr*m+stad_nr;
            lp.set_a(colNr, curNrCont, -1);
        }
        lp.set_b(curNrCont, -d);
        curNrCont++;
	}
    
    //Set alcohol limites
    for (int stad_nr = 0; stad_nr < m; stad_nr++){
        for (int warh_nr = 0; warh_nr < n; warh_nr++){
            int colNr = warh_nr*m+stad_nr;
            long cur_alc =  std::get<2>(warehouses[warh_nr]);
            lp.set_a(colNr, curNrCont, cur_alc);
        }
        long max_alc = std::get<2>(stadiums[stad_nr])*100;
        lp.set_b(curNrCont, max_alc);
        curNrCont++;
    }

	//Read in revenues
	std::vector<std::vector<int>> revenues(n, std::vector<int>(m));
	for (int warh_nr = 0; warh_nr < n; warh_nr++){
		for (int stad_nr = 0; stad_nr < m; stad_nr++){
			long r; std::cin >> r;
			revenues[warh_nr][stad_nr] = r;
            int colNr = warh_nr*m+stad_nr;
            lp.set_c(colNr, -r);
            lp.set_l(colNr, true, 0);
		}
	}
	//Read in contour lines
	std::vector<K::Circle_2> contour_lines(c);
    for (int i = 0; i < c; i++){
        long x, y, r; std::cin >> x >> y >> r;
        K::Circle_2 cur_contour =  K::Circle_2(K::Point_2(x, y), r*r);
        contour_lines[i] = cur_contour;
    }

    //Solve lp
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()){
        std::cout << "RIOT!" << std::endl;
    } else if (s.is_optimal()){
        long val = CGAL::to_double(-s.objective_value());
        std::cout << std::floor(val) << std::endl;
    }
    
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test2.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
