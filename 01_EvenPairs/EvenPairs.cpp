#include<iostream>
#include<fstream>
#include<vector>
#include<numeric>


int factorial(int n){
	int res = 1;

	while (n > 1){
		res = res * n;
		n = n-1;
	}

	return res;
}

int n_sub_k(int n, int k){
	return factorial(n)/(factorial(k)*factorial(n-k));
}

void testcase(){
	int n; std::cin >> n;
	std::vector<int>partSum(n);

	int nrOdd, nrEven;

	for (int i = 0; i < n; i++){
		int nr; std::cin >> nr;
		if (i != 0){
			partSum.at(i) = partSum.at(i-1) + nr;
		} else {
			partSum.at(i) = nr;
		}

		if (partSum.at(i) % 2 == 0){
			nrEven++;
		} else {
			nrOdd++;
		}
	}

	int res = nrEven * (nrEven -1) / 2 + nrOdd * (nrOdd - 1) / 2 + nrEven;

	std::cout << res << std::endl;



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
