#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

long D = 0; long T = 0;

struct Movement{
	long nr_moves = 0;
	long time = 0;
	long dist = 0;

	bool operator <(const Movement &otherMovement) const {
		if (dist == otherMovement.dist){
			return time < otherMovement.time;
		}
		return dist < otherMovement.dist;
	}

	Movement(long p1, long p2, long p3):nr_moves(p1), time(p2), dist(p3) {}
};

std::vector<Movement> generate_powerset(std::vector<std::pair<long, long>> &moves, int nr_moves, int index_offset){
	std::vector<Movement> movements;

	for (int s = 0; s < 1<<nr_moves; ++s){
		long nr_moves_included = 0;
		long cur_dist = 0;
		long cur_time = 0;
		for (int i = 0; i < nr_moves; ++i){
			if (s & 1<<i){ //Case i-th move is in subset
				nr_moves_included++;
				cur_dist += moves[i+index_offset].first;
				cur_time += moves[i+index_offset].second;
			}
		}
		Movement m(nr_moves_included, cur_time, cur_dist);
		movements.push_back(m); 
	}
	return movements;
}

std::vector<long> min_times_propagated(std::vector<Movement> &subset){
	std::vector<long> min_times(subset.size());
	min_times[subset.size()-1] = subset[subset.size()-1].time;
	for (int i = subset.size()-2; i >= 0; i--){
		min_times[i] = std::min(subset[i].time, min_times[i+1]);
	}
	return min_times;
}

bool find_valid_move_sequence(std::vector<Movement> subset1, std::vector<Movement> subset2, long addDist){
	//First update all movements mit nr. of gulps
	for (size_t i = 0; i < subset1.size(); i++){
		long newDist = subset1[i].nr_moves * addDist + subset1[i].dist;
		subset1[i].dist = newDist;
	}
	for (size_t i = 0; i < subset2.size(); i++){
		long newDist = subset2[i].nr_moves * addDist + subset2[i].dist;
		subset2[i].dist = newDist;
	}

	//Sort movement arrays
	std::sort(subset2.begin(), subset2.end());

	//Now we have to propagate the minimum time for every distance, i.e. if a distance is possible within a certain time
	//A smaller or equal distance is possible within the same time always as well (We can run past panoramix)
	std::vector<long> min_times = min_times_propagated(subset2);

	//Now for every element in first subset we look if we can find an element in second subset, such that time is
	//enough to reach panoramix
	for (size_t i = 0; i < subset1.size(); i++){
		long time_left = T - subset1[i].time;
		long dist_left = D - subset1[i].dist;
		Movement mov_to_comp(0, 0, dist_left); //Important just initialize with dist, not with time
		std::vector<Movement>::iterator mve = std::lower_bound(subset2.begin(), subset2.end(), mov_to_comp);
		if (mve != subset2.end()){
			long time_needed = min_times[mve - subset2.begin()];
			if (time_left > time_needed){
				return true;
			}
		}
	}
	return false;
}

void testcase() {
	long n, m; std::cin >> n >> m >> D >> T;

	std::vector<std::pair<long, long>> moves(n); //first = dist, second = time
	for (int i = 0; i < n; i++){
		long d, t; std::cin >> d >> t;
		moves[i] = std::make_pair(d, t);
	}

	std::vector<long> add_dist(m+1);
	for (int i = 1; i <= m; i++){
		std::cin >> add_dist[i];
	}

	//Generate two powersets first half/second half of available moves
	std::vector<Movement> subset1 = generate_powerset(moves, n/2, 0);
	std::vector<Movement> subset2 = generate_powerset(moves, n-(n/2), n/2);

	bool sol_found = false;
	//First check with zero gulps
	bool zero_gulps = find_valid_move_sequence(subset1, subset2, 0);
	if (zero_gulps) sol_found = true;

	//Do binsearch on nr. gulps needed
	long l = 0; long r = m; long cur_min = 0;
	if (!zero_gulps){
		while (l <= r){
			long mid = l + (r-l)/2;
			long add_dist_cur_nr_gulps = add_dist[mid];
			if (find_valid_move_sequence(subset1, subset2, add_dist_cur_nr_gulps)){
				cur_min = mid;
				r = mid-1;
				sol_found = true;
			} else {
				l = mid+1;
			}
		}
	}

	if (sol_found){
		std::cout << cur_min << std::endl;
	} else {
		std::cout << "Panoramix captured" << std::endl;
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
