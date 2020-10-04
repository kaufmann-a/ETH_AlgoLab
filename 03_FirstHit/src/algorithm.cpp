
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel EK;

double floor_to_double(const EK::FT& x){
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a +=1;
	return a;
}

void testcase(const int n) {
	//Create ray
	long x; std::cin >> x;
	long y; std::cin >> y;
	long a; std::cin >> a;
	long b; std::cin >> b;
	EK::Point_2 ep(x, y), eq(a, b);
	EK::Ray_2 ray(ep, eq);

	std::vector<EK::Segment_2> segments(n);
	for (int i = 0; i < n; i++){
		long r; std::cin >> r;
		long s; std::cin >> s;
		long t; std::cin >> t;
		long u; std::cin >> u;
		EK::Point_2 segS(r, s), segT(t, u);
		EK::Segment_2 seg(segS, segT);
		segments[i] = seg;
	}
	std::random_shuffle(segments.begin(), segments.end());

	EK::FT bestdist = std::numeric_limits<long>::max();
	EK::Segment_2 bestseg;
	EK::Point_2 bestPoint(0, 0);
	bestdist = bestdist * bestdist;

	bool intersectionAnywhere = false;
	//Create segments and check for intersect
	for (EK::Segment_2 curseg : segments){
		
		bool doIntersect = false;
		if (!intersectionAnywhere){
			doIntersect = do_intersect(ray, curseg);
		} else {
			doIntersect = do_intersect(bestseg, curseg);
		}

		if (doIntersect){
			intersectionAnywhere = true;
			auto o = CGAL::intersection(curseg, ray);
			if (const EK::Point_2* op = boost::get<EK::Point_2>(&*o)){
				bestseg = EK::Segment_2(ep, *op);
			} else if (const EK::Segment_2* op = boost::get<EK::Segment_2>(&*o)) {
				if (EK::Segment_2(ep, op->source()).has_on(bestseg.target())){
					bestseg = EK::Segment_2(ep, op->target()); 
				} else {
					bestseg = EK::Segment_2(ep, op->source());
				}
			} else {
				std::cout << "What the heck is happening here" << std::endl;
			}
		}
	}

	if (intersectionAnywhere){
		std::cout << (long) floor_to_double(bestseg.target().x()) << " " << (long) floor_to_double(bestseg.target().y()) << std::endl;
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