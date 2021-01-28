#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>

int n_sub_2(int n){
	return n * (n-1)/2;
}

void testcase() {
	int n; std::cin >> n;
	std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			std::cin >> matrix[i][j];
		}
	}

	//Build prefix matrix
	std::vector<std::vector<int>> prefixm(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			if (i == 0 && j == 0){ //First field, take value from matrix
				prefixm[i][j] = matrix[i][j];
			} else if (i == 0){ //First line
				prefixm[i][j] = prefixm[i][j-1] + matrix[i][j];
			} else if (j == 0){ //First column
				prefixm[i][j] = prefixm[i-1][j] + matrix[i][j];
			} else { //Regular fields
				prefixm[i][j] = prefixm[i-1][j] + prefixm[i][j-1] - prefixm[i-1][j-1] + matrix[i][j];
			}
		}
	}

	//Do magic
	int counter = 0;
	for (int i = 0; i < n; i++){ //Interval end
		for (int j = 0; j <= i; j++){ //interval start
			std::vector<int> curprefixSum(n);
			int nrEven = 0; int nrOdd = 0;
			for (int k = 0; k < n; k++) { //Iterate over the columnds and build the sum for every column
				if (j == 0){ //First line
					curprefixSum[k] = prefixm[i][k];
				} else { //Interval from j to i in column k
					curprefixSum[k] = prefixm[i][k] - prefixm[j-1][k] ; 
				}
				if (curprefixSum[k] % 2 == 0){
					nrEven++;
				} else {
					nrOdd++;
				}
			}
			int curNr = n_sub_2(nrEven) + n_sub_2(nrOdd) + nrEven;
			counter += curNr;
		}
	}
	std::cout << counter << std::endl;
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
