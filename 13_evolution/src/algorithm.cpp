#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>


struct Species {
	std::string name = "";
	int index = -1;
	int age = 0;
	std::vector<int> children;
	int parent = -1;
	std::vector<int> querries;

	bool operator <(const Species &other) const {
		return name < other.name;
	}

	Species(std::string n, int a) : name(n), age(a){};
};

int find_species(std::vector<Species> &species, std::string search){
	int l = 0; int r = species.size() -1;
	while (l < r){
		int mid = l + (r-l)/2;
		Species spec = species[mid];
		if (spec.name >= search){
			r = mid;
		} else {
			l = mid+1;
		}
	}
	return r;
}

std::string find_ancestor(std::vector<Species> &cur_line, int age_search){
	int l = 0; int r = cur_line.size()-1;
	std::string best = "";
	while (l < r){
		int mid = l + (r-l)/2;
		int age_mid = cur_line[mid].age;
		if (age_mid <= age_search){
			best = cur_line[mid].name;
			r = mid;
		} else {
			l = mid + 1;
		}
	}
	return cur_line[r].name;
}

void solve_recursive(std::vector<Species> &species, std::vector<std::pair<std::string, int>> &querries, std::vector<std::string> &solutions, std::vector<Species> &cur_line, int cur_node){
	//Push curnode
	cur_line.push_back(species[cur_node]);
	
	//process querries of cur node
	for (int i = 0; i < species[cur_node].querries.size(); i++){
		int cur_querry_age = querries[species[cur_node].querries[i]].second;
		std::string sol = find_ancestor(cur_line, cur_querry_age);
		solutions[species[cur_node].querries[i]] = sol;
	}

	//Process children
	for (int i = 0; i < species[cur_node].children.size(); i++){
		solve_recursive(species, querries, solutions, cur_line, species[cur_node].children[i]);
	}

	//Remove last child from cur line array
	cur_line.pop_back();
}


void testcase() {
	int n, q; std::cin >> n >> q;
	
	std::vector<Species> species;
	
	//Read in all species
	for (int i = 0; i < n; i++){
		std::string name; int age; std::cin >> name >> age;
		species.push_back(Species(name, age));
	}

	std::sort(species.begin(), species.end());

	//Update indices of species
	for (int i = 0; i < n; i++){
		species[i].index = i;
	}

	//Read relationships
	for (int i = 0; i < n-1; i++){
		std::string s; std::string p; std::cin >> s >> p;
		int spec_s = find_species(species, s);
		int spec_p = find_species(species, p);
		species[spec_s].parent = spec_p;
		species[spec_p].children.push_back(spec_s);
	}

	//Read querries
	std::vector<std::pair<std::string, int>> querries(q);
	for (int i = 0; i < q; i++){
		std::string q_name; int q_age; std::cin >> q_name >> q_age;
		querries[i] = std::make_pair(q_name, q_age);
		int spec_index = find_species(species, q_name);
		species[spec_index].querries.push_back(i);
	}

	//Find root
	int start = 0; int store = 0;
	while(start != -1){
		store = start;
		start = species[start].parent;
	}

	//Solve querries recursively
	std::vector<std::string> solutions(q);
	std::vector<Species> cur_line;
	solve_recursive(species, querries, solutions, cur_line, store);

	//Print solutions
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
