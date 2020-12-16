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
	int myIndex;
	std::vector<int> index_childs;
	std::vector<std::pair<int, int>> search_list; //Here we save all queries with querynr and age to search for

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

void recursiveSearch(std::vector<Species> &speciesList, std::vector<Species> &speciesPath, std::vector<std::string> &sol, int curSpecies){
	for (int i = 0; i < speciesList[curSpecies].search_list.size(); i++){
		int querynr = speciesList[curSpecies].search_list[i].first;
		int ageToSearch = speciesList[curSpecies].search_list[i].second;
		int l, r; l = 0; r = speciesPath.size()-1;
		while (l < r){
			int mid = l + (r-l)/2;
			if (speciesPath[mid].age == ageToSearch){
				r = mid;
				break;
			}
			if (speciesPath[mid].age < ageToSearch){
				r = mid;
			} else if (speciesPath[mid].age > ageToSearch){
				l = mid+1;
			}
		}
		sol[querynr] = speciesPath[r].name;
	}
	for (int i = 0; i < speciesList[curSpecies].index_childs.size(); i++){
		int nextSpec = speciesList[curSpecies].index_childs[i];
		speciesPath.push_back(speciesList[nextSpec]);
		recursiveSearch(speciesList, speciesPath, sol, nextSpec);
	}
	speciesPath.pop_back();
}

void testcase() {
	int n, q; std::cin >> n >> q;

	std::vector<Species> species_list(n, Species(0, 0, ""));
	std::vector<Species> species_path;
	std::vector<std::string> solutions(q, "");

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
		species_list[index_s].myIndex = index_s;
		species_list[index_p].index_childs.push_back(index_s);
		species_list[index_p].myIndex = index_p;
	}

	for (int i = 0; i < q; i++){
		std::string s; int a; std::cin >> s >> a;
		int index_s = index_of_spec(species_list, s);
		species_list[index_s].search_list.push_back(std::make_pair(i, a));
	}

	//Find root
	Species curSpecies = species_list[0];
	while (curSpecies.index_predecessor != -1){
		curSpecies = species_list[curSpecies.index_predecessor];
	}
	species_path.push_back(curSpecies);

	recursiveSearch(species_list, species_path, solutions, curSpecies.myIndex);

	for (int i = 0; i < solutions.size(); i++){
		std::cout << solutions[i] << " ";
	}
	std::cout << std::endl;
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