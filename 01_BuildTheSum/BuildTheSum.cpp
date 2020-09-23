#include<iostream>
#include<fstream>

void testcase(){
	int n; std::cin >> n;

	int sum = 0;
	for (int i = 0; i < n; i++){
		int temp; std::cin >> temp;
		sum += temp;
	}

	std::cout << sum << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());
	int t; std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}
}
