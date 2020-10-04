
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
	IK::Point_2 p(x, y), q(a, b);
	IK::Ray_2 ray(p, q);
	EK::Point_2 ep(p.x(), p.y()), eq(q.x(), q.y());
	EK::Ray_2 eray(ep, eq);


	EK::FT bestdist = std::numeric_limits<long>::max();
	EK::Point_2 bestPoint(0, 0);
	bestdist = bestdist * bestdist;
	bool doIntersect = false;
	//Create segments and check for intersect
	for (int i = 0; i < n; i++){

		long r; std::cin >> r;
		long s; std::cin >> s;
		long t; std::cin >> t;
		long u; std::cin >> u;
		IK::Point_2 segS(r, s), segT(t, u);
		IK::Segment_2 seg(segS, segT);

		if (CGAL::do_intersect(ray, seg)){
			EK::Point_2 esegS(segS.x(), segS.y()), esegT(segT.x(), segT.y());
			EK::Segment_2 eseg(esegS, esegT);
			EK::FT dist = 0;
			auto o = CGAL::intersection(eseg, eray);
			if (const EK::Point_2* op = boost::get<EK::Point_2>(&*o)){
				EK::FT dist = CGAL::squared_distance(*op, ep);
				if (dist < bestdist){
					bestdist = dist;
					bestPoint = *op;
				}
			} else if (const EK::Segment_2* op = boost::get<EK::Segment_2>(&*o)) {
				EK::FT dist_source = CGAL::squared_distance(op->source(), ep);
				EK::FT dist_target = CGAL::squared_distance(op->target(), ep);
				if (dist_source < bestdist){
					bestPoint = op->source();
					bestdist = dist_source;
				}
				if (dist_target < bestdist){
					bestPoint = op->target();
					bestdist = dist_target;
				}
			} else {
				std::cout << "What the heck is happening here" << std::endl;
			}
			doIntersect = true;
		}

	}

	if (doIntersect){
		std::cout << (long) floor_to_double(bestPoint.x()) << " " << (long) floor_to_double(bestPoint.y()) << std::endl;
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