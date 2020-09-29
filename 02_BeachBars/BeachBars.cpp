#include<iostream>
#include<fstream>
#include<vector>
#include<limits>
#include<set>
//#include<math>


void testcase(){
	int n; std::cin >> n;

	std::set<int> coordinates;
	for (int i = 0; i < n; ++i){
		int val; std::cin >> val;
		coordinates.insert(val);
	}

	std::set<int> optimalPos;
	int maxNrOfParasols = 0;
	int minDistance = std::numeric_limits<int>::max();

	int nrOfParasols = 0;
	int intervalSize = 0;

	std::set<int>::iterator left = coordinates.begin();
	std::set<int>::iterator right = coordinates.begin();
	std::set<int>::iterator rightOld = coordinates.begin();

	do {
		do {
			rightOld = right;
			nrOfParasols++;
			right++;
			intervalSize = *right - *left;

		} while (intervalSize <= 200 && right != coordinates.end());

		//Calculate
		int midPosition = (*rightOld-*left)/2 + *left;
		int curMinDistance = std::max(*rightOld - midPosition, midPosition - *left);

		//Clear optimalPos set if new best location found
		if (nrOfParasols > maxNrOfParasols || (nrOfParasols == maxNrOfParasols && curMinDistance < minDistance)){
			maxNrOfParasols = nrOfParasols;
			minDistance = curMinDistance;
			optimalPos.clear();
		}
		//Add to set if equal or better position found
		if (nrOfParasols >= maxNrOfParasols && curMinDistance <= minDistance){
			optimalPos.insert(midPosition);

			//Check if interval was odd number
			if ((*rightOld-*left) % 2 == 1){
				optimalPos.insert(midPosition+1);
			}
		}

		//Iterate left marker until interval size is below 200 again
		while (intervalSize > 200 && left != right){
			left++;
			nrOfParasols--;
			intervalSize = *right - *left;
		} 
	} while (right != coordinates.end());
	
	std::cout << maxNrOfParasols << " " << minDistance << std::endl;
	for (std::set<int>::iterator it = optimalPos.begin(); it != optimalPos.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;


}


int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t;std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}



	return 0;
}
