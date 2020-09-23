#include<iostream>
#include<fstream>

void testcase(){
	int n; std::cin >> n;
	//std::cout << n << std::endl;
	int curBest; std::cin >> curBest;
	//std::cout << curBest << " ";
	for (int i = 1; i < n; i++){
		int curHeight; std::cin >> curHeight;
		//std::cout << curHeight << " ";
		if (curBest >= n){
			curBest = n;
			continue;
		}
		if (i >= curBest){
			continue;
		}
		if (i + curHeight > curBest){
			curBest = i + curHeight;
			//std::cout << curBest << " ";
		}
	}
	//std::cout << std::endl;
	std::cout << curBest << std::endl;

}


int main(){
	std::ios_base::sync_with_stdio(false);
	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;
	for (int i = 0; i < t; i++){
		testcase();
	}
}
