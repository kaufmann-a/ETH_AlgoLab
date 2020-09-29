#include<iostream>
#include<fstream>
#include<vector>
#include<limits>
#include<set>
//#include<math>

int BEACHSIDE = 1000000;
int BEACHLENGTH = 2*BEACHSIDE + 1;

void testcase(){
	int n; std::cin >> n;

	std::vector<bool> beach(BEACHLENGTH, false) ;
	//Fill beach with parasoles
	for (int i = 0; i < n; i++){
		int pos; std::cin >> pos;
		beach[pos+BEACHSIDE] = true;
	}

	std::set<int> best;

	int curHighestAmount = 0;
	int curShortestDistance = std::numeric_limits<int>::max();
	//Build sum for first 100 parasols
	for (int i = 0; i <= 100; i++){
		if (beach[i] == true){
			curHighestAmount++;
			curShortestDistance = i;
		}
	}
	best.insert(0);

	int curAmount = curHighestAmount;
	int curDistance = curShortestDistance;
	//Iterate through beach
	for (int i = 1; i <= 2*BEACHSIDE; i++){
		//check if at left border of beach
		if (i <= 100){
			//Case new parasol found, Amount must be larger as all previous parasoles kept
			if (beach.at(i+100) == true){
				curAmount++;
				curHighestAmount = curAmount;
				curDistance = 100;
				curShortestDistance = curDistance;
				best.clear();
				best.insert(i);
			} else {
				//Just Update distance
				curDistance--;
				if (std::abs(curDistance) < std::abs(curShortestDistance)){
					curShortestDistance = curDistance;
					best.clear();
					best.insert(i);
				} else if (std::abs(curDistance) == std::abs(curShortestDistance)){
					best.insert(i);
				}
			}
		} else if (i <= 2*BEACHSIDE-100){
			//Add remove potential new parasoles
			if (beach.at(i+100) == true){
				curAmount++;
			}
			if (beach.at(i-101) == true){
				curAmount--;
			}

			if (curAmount > curHighestAmount){
				curHighestAmount = curAmount;
				curDistance = 100;
				curShortestDistance = curDistance;
				best.clear();
				best.insert(i);
			} else if (curAmount == curHighestAmount){
				curDistance--;
				if(std::abs(curDistance) < std::abs(curShortestDistance)){
					curShortestDistance = curDistance;
					best.clear();
					best.insert(i);
				} else if (std::abs(curDistance) == std::abs(curShortestDistance)){
					best.insert(i);
				}
			}
		} else if (i <= 2*BEACHSIDE){
			if (beach.at(i-101) == true){
				break;
			} else {
				curDistance--;
				if(std::abs(curDistance) < std::abs(curShortestDistance)){
					curShortestDistance = curDistance;
					best.clear();
					best.insert(i);
				} else if (std::abs(curDistance) == std::abs(curShortestDistance)){
					best.insert(i);
				}
			}
		}
	}

	std::cout << curHighestAmount << " " << curShortestDistance << std::endl;
	for (std::set<int>::iterator it = best.begin(); it!= best.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;
}


int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t;std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}



	return 0;
}
