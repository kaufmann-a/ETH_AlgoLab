#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

//typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EKS;
typedef CGAL::Min_circle_2_traits_2<EKS> Traits_sq;
typedef CGAL::Min_circle_2<Traits_sq> Min_circle_sq;

typedef CGAL::Exact_predicates_exact_constructions_kernel EK;
typedef CGAL::Min_circle_2_traits_2<EK> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double ceil_to_double(const EK::FT& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -=1;
	return a;
}

double ceil_to_double_sqrt(const EKS::FT& x){
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -=1;
	return a;
}


void testcase(int n) {
	std::vector<EK::Point_2> points(n);
	for (int i = 0; i < n; i++){
		long x, y; std::cin >> x; std::cin >> y;
		EK::Point_2 p(x, y);
		points[i] = p;
	}

	//Calculate min circle with epic kernel
	Min_circle mc(points.begin(), points.end(), true);
	Traits::Circle c = mc.circle();

	//Convert to epic_sqr
	Min_circle_sq mcsq;
	for (auto it = mc.support_points_begin(); it != mc.support_points_end(); it++){
		mcsq.insert(EKS::Point_2(ceil_to_double(it->x()), ceil_to_double(it->y())));
	}
	Traits_sq::Circle csqrt = mcsq.circle();

	EKS::FT d = CGAL::sqrt(csqrt.squared_radius());
	std::cout << (long) ceil_to_double_sqrt(d) << std::endl;
}

int main() {
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;
	while (t != 0){
		testcase(t);
		std::cin >> t;
	}
}
