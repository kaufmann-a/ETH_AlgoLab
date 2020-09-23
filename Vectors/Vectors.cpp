#include<iostream>
#include<fstream>
#include<vector>

void testcase(){
	int n; std::cin >> n;
	std::vector<int> vec(n);

	for (int i = 0; i < n; i++){
		std::cin >> vec.at(i);
	}

	//Remove element ad pos. d
	int d; std::cin >> d;
	vec.erase(vec.begin() + d);

	//Remove range of elements
	int a, b; std::cin >> a; std::cin >> b;
	vec.erase(vec.begin() + a, vec.begin() + b + 1);

	//Print result
	if (vec.size() == 0){
		std::cout << "Empty";
		return;
	}
	for (int i : vec){
		std::cout << i << " ";
	}
	std::cout << std::endl;
}


int main() {
	std::ios_base::sync_with_stdio(false);

	std::ifstream in("./testsets/test1.in");
	//std::streambuf *cinbuf = std::cin.rdbuf();
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;
	for (int i = 0; i < t; i++){
		testcase();
	}

	return 0;
}
