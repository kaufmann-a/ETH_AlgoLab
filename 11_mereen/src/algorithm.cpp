#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>
#include <map>

//dp_entry is used to save a dp_entry after n rounds, which fighters are on the left, which fighters are on the right
struct dp_entry{
	int left_m; //encoding of last m fighters. example: m=3, if left_m = 34 this means that last figher was of type 4 and second to last of type 3
	int right_m;
	long sum_excitement;
};


int calculate_nr_diff_fighters(int nr){
	std::vector<int> fighter_dist(5);
	while (nr > 0){
		int fighter = nr % 10;
		fighter_dist[fighter] ++;
		nr /= 10;
	}
	int nr_fighters = 0;
	for (int i = 1; i < 5; i++){
		if (fighter_dist[i] > 0){
			nr_fighters++;
		}
	}
	return nr_fighters;
}

void testcase() {
	int n, k, m; std::cin >> n >> k >> m;

	std::vector<int> fighters(n, 0);
	//we need fighters to be of type 1-4
	for (int i = 0; i < n; i++){
		int fighter; std::cin >> fighter;
		fighters[i] = fighter+1;
	}

	//DP-Table: 
	//dim0 = nr. of round
	//dim1: represents ratio total left to toal right moves. for dim1=12 this means equal amount of left and right, for dim1<12, more left, for dim1>12 more right
	//dim2 set: here we store different possible dp_entry, per dp_entry always the one with highest excitement is kept
	//tuple: <0>=left_queue, <1>=right_queue <2>=total excitement
	std::vector<std::vector<std::map<std::pair<int, int>, long>>> dp(n+1, std::vector<std::map<std::pair<int, int>, long>>(25));

	//init dp
	
	std::pair<int, int> entry = std::make_pair(0, 0);
	dp[0][12].insert(std::make_pair(entry, 0));
	int equal_left_right = 12;

	//now we iterate over the queue
	for (int i = 0; i< n; i++){
		int next_fighter = fighters[i];
		for (int j = 0; j < 25; j++){
			
			//check if dp[i][j] contains a dp_entry
			if (dp[i][j].empty()){
				continue;
			}

			//we now check for every dp_entry contained in dp[i][j] the excitement for assigning current fighter left or right
			for (std::map<std::pair<int, int>, long>::iterator it = dp[i][j].begin(); it != dp[i][j].end(); it++){
				std::pair<std::pair<int, int>, long> cur_entry = *it;
				
				//possibility 1: we move to the left
				int left_queue = (cur_entry.first.first*10+next_fighter) % (int) (std::pow(10, m));
				long excitement_l = 1000 * calculate_nr_diff_fighters(left_queue);
				long penalty_l = std::pow(2, std::abs(equal_left_right - j + 1));
				long final_excitement_l = excitement_l - penalty_l;
				//check if in dp[i+1][j-1] queue with set already there
				if (final_excitement_l > 0){
					long new_total_exitement_l = cur_entry.second + final_excitement_l;
					if (new_total_exitement_l > dp[i+1][j-1][std::make_pair(left_queue, cur_entry.first.second)]){
						dp[i+1][j-1][std::make_pair(left_queue, cur_entry.first.second)] = new_total_exitement_l;
					}
				}

				//possibility 2: we move to the right
				int right_queue = (cur_entry.first.second*10+next_fighter) % (int) (std::pow(10, m));
				long excitement_r = 1000 * calculate_nr_diff_fighters(right_queue);
				long penalty_r = std::pow(2, std::abs(equal_left_right - j - 1));
				long final_excitement_r = excitement_r - penalty_r;
				if (final_excitement_r > 0){
					long new_total_excitement_r = cur_entry.second + final_excitement_r;
					if (new_total_excitement_r > dp[i+1][j+1][std::make_pair(cur_entry.first.first, right_queue)]){
						dp[i+1][j+1][std::make_pair(cur_entry.first.first, right_queue)] = new_total_excitement_r;
					}
				}
			}
		}
	}

	long maxExcitement = 0;
	for (int i = 0; i < 25; i++){
		for (std::map<std::pair<int, int>, long>::iterator it = dp[n][i].begin(); it != dp[n][i].end(); it++){
			if (it->second > maxExcitement){
				maxExcitement = it->second;
			}
		}
	}


	std::cout << maxExcitement << std::endl; 
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
