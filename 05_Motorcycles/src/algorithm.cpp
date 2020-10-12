#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <map>
#include <set>
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

void intersectCheck(std::vector<EK::Segment_2> &segments, std::map<int, EK::Ray_2> &rays, EK::Ray_2 &curRay, int bicker_nr){

	bool collidedWithSeg = false;
	EK::Segment_2 ray_seg;
	for (ulong i = 0; i < segments.size(); i++){ //Loop over every already read biker and check if ray somewhere intersects
		bool doIntersect = false;
		if (collidedWithSeg){
			doIntersect = CGAL::do_intersect(segments[i], ray_seg);
			std::cout << segments[ï].source().x() << " " << segments[i].source().y() << std::endl;
		} else {
			doIntersect = CGAL::do_intersect(segments[i], curRay);
		}
		
		
		if (doIntersect){ //Intersection check
			auto o = CGAL::intersection(segments[i], curRay);
			if (const EK::Point_2* op = boost::get<EK::Point_2>(&*o)){
				//Segment and ray intersect, check which distance to origin is larger
				EK::FT dist_ray = CGAL::squared_distance(*op, curRay.source());
				EK::FT dist_seg = CGAL::squared_distance(*op, segments[i].source());
				if (dist_ray > dist_seg){ //new rays distance is longer, thus journey ends at this point op
					collidedWithSeg = true;
					ray_seg.source() = curRay.source();
					ray_seg.target() = *op;
				} else if (dist_ray == dist_seg){ //Case where both distances are the same, check which bicker came from the right
					if (curRay.source().y() < segments[i].source().y()){ //new Ray wins, old seg is shortened
						EK::Segment_2 seg_new(segments[i].source(), *op);
						segments[i] = seg_new;
					} else { //Old seg wins, new ray is shortened
						collidedWithSeg = true;
						ray_seg.source() = curRay.source();
						ray_seg.target() = *op;
					}
				} else { //Case where old seg is longer, therefore ray stays
					EK::Segment_2 seg_new(segments[i].source(), *op);
					segments[i] = seg_new;
				}
			} else {
				std::cout << "What is happening here" << std::endl;
			}
		}
	}

	if (collidedWithSeg){ //No potential driver to sunset, therefore add segment of driver to segments-array
		segments.push_back(ray_seg);
	} else { //No coolision, could be potential new driver to sunset, check if collision with other pot. sunsetdrivers
		bool newPotDriver = true;
		bool colided = false;
		EK::Segment_2 ray_seg_new;
		for (auto it = rays.cbegin(), next_it = it; it != rays.cend(); it = next_it){
			++next_it;
			bool doIntersect = false;
			if (colided){
				doIntersect = CGAL::do_intersect(it->second, ray_seg_new);
			} else {
				doIntersect = CGAL::do_intersect(it->second, curRay);
			}
				
			if (doIntersect){
				auto o = CGAL::intersection(it->second, curRay);
				if (const EK::Point_2* op = boost::get<EK::Point_2>(&*o)){
					//Rays intersect, check which distance to origin is larger
					EK::FT dist_new_ray = CGAL::squared_distance(*op, curRay.source());
					EK::FT dist_old_ray = CGAL::squared_distance(*op, it->second.source());

					if (dist_new_ray > dist_old_ray){ //New ray looses and is added as segment to segments array
						EK::Segment_2 seg(curRay.source(), *op);
						ray_seg_new = seg;
						newPotDriver = false;
						colided = true;
					} else { //Old pot driver to sunset is beaten, remove from rays and insert to sesgments
						EK::Segment_2 new_seg(it->second.source(), *op);
						segments.push_back(new_seg);
						rays.erase(it);
					}
				} else {
					std::cout << "Again strange stuff here" << std::endl;
				}
			}
		}
		if (newPotDriver){
			rays.insert(std::make_pair(bicker_nr, curRay));
		} else {
			segments.push_back(ray_seg_new);
		}

	}


}


void testcase() {
	int n; std::cin >> n;

	std::vector<EK::Segment_2> segments;
	std::map<int, EK::Ray_2> rays;

	for (int i = 0; i < n; i++){
		long x0, y0, x1, y1; std::cin >> y0; std::cin >> x1; std::cin >> y1; x0 = 0;
		EK::Point_2 start(x0, y0), sup(x1, y1);
		EK::Ray_2 ray(start, sup);
		intersectCheck(segments, rays, ray, i);
	}
	 
	for (std::map<int, EK::Ray_2>::iterator it = rays.begin(); it != rays.end(); it++){
		std::cout << it->first << " ";
	}
	std::cout << std::endl;





	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in ("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());


	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
