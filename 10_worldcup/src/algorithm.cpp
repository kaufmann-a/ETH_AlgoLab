#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <fstream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef K::Point_2 P;

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double ceil_to_double(const CGAL::Quotient<ET> &x)
{
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -= 1;
  return a;
}

void testcase(){
    int n, m, c;
    std::cin >> n >> m >> c;
    std::vector<P> s_w(n+m);
    std::vector<IT> warehouses(n);

    Program lp (CGAL::SMALLER, true, 0, false, 0);
    for(int i = 0; i < n; i++) {
        IT s;
        std::cin >> s_w[i] >> s >> warehouses[i];
        lp.set_b(i, s);
    }

    for(int i = 0; i < m; i++) {
        IT d, u;
        std::cin >> s_w[i+n] >> d >> u;
        u *= 100;
        lp.set_b(i+n, d);
        lp.set_b(i+n+m, -d);
        lp.set_b(i+n+2*m, u);
    }

    int counter = -1;
    std::vector<std::vector<std::pair<int, double>>> revenues(n, std::vector<std::pair<int, double>>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            counter++;
            double revenue;
            std::cin >> revenue;

            revenues[i][j] = std::make_pair(counter, -revenue);

            lp.set_a(counter, i, 1);
            lp.set_a(counter, j+n, 1);
            lp.set_a(counter, j+n+m, -1);
            lp.set_a(counter, j+n+2*m, warehouses[i]);
        }
    }

    Triangulation tri;
    tri.insert(s_w.begin(), s_w.end());
    for(int i = 0; i < c; i++) {
        P contour;
        long radius;
        std::cin >> contour >> radius;
        radius *= radius;
        if(CGAL::squared_distance(tri.nearest_vertex(contour)->point(), contour) <= radius) {
            for(int i = 0; i < n; i++) {
                bool sign1 = (CGAL::squared_distance(contour, s_w[i]) <= radius);
                for(int j = 0; j < m; j++) {
                    bool sign2 = (CGAL::squared_distance(contour, s_w[j+n]) <= radius);
                    if(sign1 != sign2)
                        revenues[i][j].second += 0.01;
                }
            }
        }
    }

    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            lp.set_c(revenues[i][j].first, ET(revenues[i][j].second));

    Solution s = CGAL::solve_linear_program(lp, ET());
    if(s.is_infeasible()){
        std::cout << "RIOT!\n";
    } 
    else {
        CGAL::Quotient<ET> res = s.objective_value();
        long revenue = ceil_to_double(res);
        std::cout <<  -revenue << "\n";
    } 
    return;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::fstream in("./testsets/test1.in");
    std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}