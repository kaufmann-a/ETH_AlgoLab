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
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Delaunay_triangulation_2.h>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;
typedef K::Circle_2 C;
typedef K::Segment_2 S;

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

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
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    
    std::vector<K::Point_2> stad_war;
    stad_war.reserve(n+m);

    int curNrCont = 0;
	//Read in warehouses
	std::vector<std::tuple<K::Point_2, IT, long>> warehouses(n);
	for (int warh_nr = 0; warh_nr < n; warh_nr++){
		long x, y, a; IT s; std::cin >> x >> y >> s >> a;
		warehouses[warh_nr] = std::make_tuple(K::Point_2(x, y), s, a);
        stad_war.push_back(K::Point_2(x,y));
        //Sum of suplies to all stadiums must be smallerequal s
        for (int stad_nr = 0; stad_nr < m; stad_nr++){
            int colNr = warh_nr*m+stad_nr;
            lp.set_a(colNr, curNrCont, 1);
        }
        lp.set_b(curNrCont, s);
        curNrCont++;
	}
	//Read in stadiums
	std::vector<std::tuple<K::Point_2, IT, IT>> stadiums(m);
	for (int stad_nr = 0; stad_nr < m; stad_nr++){
		long x, y; IT d, u; std::cin >> x >> y >> d >> u;
		stadiums[stad_nr] = std::make_tuple(K::Point_2(x, y), d, u);
        stad_war.push_back(K::Point_2(x, y));
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
            IT cur_alc =  std::get<2>(warehouses[warh_nr])/100;
            lp.set_a(colNr, curNrCont, cur_alc);
        }
        IT max_alc = std::get<2>(stadiums[stad_nr]);
        lp.set_b(curNrCont, max_alc);
        curNrCont++;
    }

	//Read in revenues
	std::vector<std::vector<double>> revenues(n, std::vector<double>(m));
	for (int warh_nr = 0; warh_nr < n; warh_nr++){
		for (int stad_nr = 0; stad_nr < m; stad_nr++){
			long r; std::cin >> r;
			revenues[warh_nr][stad_nr] = r;
		}
	}

    Triangulation t;
    t.insert(stad_war.begin(), stad_war.end());

    for (int i = 0; i < c; i++){
        long x, y, r; std::cin >> x >> y >> r;
        K::Point_2 center = K::Point_2(x, y);
        r*= r;
        if (CGAL::squared_distance(t.nearest_vertex(center)->point(), center) <= r){
            for (int j = 0; j < n; j++){
                bool s1 = (CGAL::squared_distance(center, stad_war[i]) <= r);
                for (int k = 0; k < m; k++){
                    bool s2 = (CGAL::squared_distance(center, stad_war[k+n])<=r);
                    if (s2 != s1){
                        revenues[j][k] += 0.01;
                    }
                }
            }
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            lp.set_c(i*m+j, ET(-revenues[i][j]));
        }
    }

    //Solve lp
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()){
        std::cout << "RIOT!" << std::endl;
    } else if (s.is_optimal()){
        double val = CGAL::to_double(-s.objective_value());
        long final_sol = val;
        std::cout << final_sol << std::endl;
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
