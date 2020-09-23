#include<iostream>
#include<fstream>
#include<vector>

void testcase(){
	//Read input

	int n; std::cin >> n;

	std::vector<std::vector<int>> matrix(n);
	for (int i = 0; i < n; i++){
		matrix.at(i).resize(n);
	}

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			std::cin >> matrix.at(i).at(j);
			//std::cout << matrix.at(i).at(j) << " ";
		}
	}

	std::vector<std::vector<int>> matrixsum(n + 1);
	for (int i = 0; i <= n; i++){
		matrixsum.at(i).resize(n + 1);
	}

	for (int i = 0; i <= n; i++){
		for (int j = 0; j <= n; j++){
			if (i == 0 || j == 0){
				matrixsum.at(i).at(j) = 0;
			} else {
				matrixsum.at(i).at(j) = matrix.at(i-1).at(j-1) + matrixsum.at(i-1).at(j) + matrixsum.at(i).at(j-1) - matrixsum.at(i-1).at(j-1);
			}
		}
	}

//	//Print Matrixsum
//	for (int i = 0; i <= n; i++){
//		for (int j = 0; j <= n; j++){
//			std::cout << matrixsum.at(i).at(j) << " ";
//		}
//		std::cout << std::endl;
//	}
	int curSum = 0;
	//Solve by sliding windows
	for (int i = 2; i <= (n+1); i++){
		for (int j = 2; j <= (n+1); j++){
			//Vertical slide
			for (int m = 0; m <= (n+1-i); m++){
				//Horizontal slide
				for (int k = 0; k <= (n+1-j); k++){
					int sum = matrixsum.at(m+i-1).at(k+j-1) + matrixsum.at(m+i-1).at(k) + matrixsum.at(m).at(k+j-1) - matrixsum.at(m).at(k);
					if (sum % 2 == 0){
						curSum++;
					}
				}
			}
		}
	}


	std::cout << curSum << std::endl;

}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;
	//std::cout << t << std::endl;
	for (int i = 0; i < t; i++){
		testcase();
	}

	return 0;
}
