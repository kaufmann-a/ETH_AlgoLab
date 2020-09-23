#include <iostream>
#include <fstream>


void testcase(){
	int n; std::cin >> n;
	int result = 0;
	for (int i = 0; i < n; ++i){
		int a; std::cin >> a;
		result += a;
	}

	std::cout << result << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false); //Always!

	std::ifstream in("./testsets/sample.in");
	//std::streambuf *cinbuf = std::cin.rdbuf();
	std::cin.rdbuf(in.rdbuf());


	int t; std::cin >> t; //Read the number of test cases
	//std::cout << t << std::endl;
	for (int i = 0; i < t; ++i)
		testcase();
}
