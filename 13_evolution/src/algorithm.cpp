#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <string>

struct Species {
	int age;
	int index_predecessor;
	std::string name;

	Species(int s, int e, std::string v) : age(s), index_predecessor(e), name(v) {}
};

int index_of_spec(std::vector<Species> &species_list, std::string &name_spec){
	int l, r; l = 0; r = species_list.size()-1;
	while (l < r){
		int mid = l + (r-l)/2;
		if (species_list[mid].name == name_spec){
			return mid;
		}
		if (species_list[mid].name > name_spec){
			r = mid;
		} else if (species_list[mid].name < name_spec){
			l = mid+1;
		}
	}
	return r;
}

bool specSort(Species sp1, Species sp2){
	return sp1.name < sp2.name;
}

void testcase() {
	int n, q; std::cin >> n >> q;

	std::vector<Species> species_list(n, Species(0, 0, ""));

	for (int i = 0; i < n; i++){
		std::string s; int a; std::cin >> s >> a;
		Species sp(a, -1, s);
		species_list[i] = sp;
	}

	std::sort(species_list.begin(), species_list.end(), specSort);

	for (int i = 0; i < n-1; i++){
		std::string s, p; std::cin >> s >> p;
		int index_s = index_of_spec(species_list, s);
		int index_p = index_of_spec(species_list, p);
		species_list[index_s].index_predecessor = index_p;
	}

	for (int i = 0; i < q; i++){
		std::string s; int a; std::cin >> s >> a;
		int index_s = index_of_spec(species_list, s);
		int index_save = index_s;
		while (species_list[index_s].age <= a){
			if (species_list[index_s].index_predecessor == -1){
				index_save = index_s;
				break;
			}
			index_save = index_s;
			index_s = species_list[index_s].index_predecessor;
		}
		std::cout << species_list[index_save].name << " ";
	}
	std::cout << std::endl;
	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/user_test.in");
	std::cin.rdbuf(in.rdbuf());

	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}