#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

void testcase(const int n) {
	bool doesIntersect = false;

	//Create ray
	long x; std::cin >> x;
	long y; std::cin >> y;
	long a; std::cin >> a;
	long b; std::cin >> b;
	IK::Point_2 p(x, y), q(a, b);
	IK::Ray_2 ray(p, q);

	//Create segments and check for intersect
	for (int i = 0; i < n; i++){
		long r; std::cin >> r;
		long s; std::cin >> s;
		long t; std::cin >> t;
		long u; std::cin >> u;

		if (!doesIntersect){
			IK::Point_2 segS(r, s), segT(t, u);
			IK::Segment_2 seg(segS, segT);
			if (CGAL::do_intersect(ray, seg)){
				doesIntersect = true;
			}	
		}
	}

	if (doesIntersect){
		std::cout << "yes" << std::endl;
	} else {
		std::cout << "no" << std::endl;
	}
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
