#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>

void testcase(){
	int n; std::cin >> n;
	std::vector<int> vec(n);

	for (int i = 0; i < n; i++){
		std::cin >> vec.at(i);
	}

	int x; std::cin >> x;
	if (x == 1){
		std::sort(vec.begin(), vec.end(), std::greater<int>());
	} else {
		std::sort(vec.begin(), vec.end());
	}
	for (const int &nr : vec){
		std::cout << nr << " ";
	}
	std::cout << std::endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}


	return 0;
}
