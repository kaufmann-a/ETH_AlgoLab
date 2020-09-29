#include<iostream>
#include<fstream>
#include<vector>
#include<limits>
#include<set>
#include<math>

void testcase(){
	int n; std::cin >> n;

	std::vector<bool> beach(1000000*2+1, false) ;
	for (int i = 0; i < n; i++){
		int pos; std::cin >> pos;
		beach[pos+100000] = true;
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
	for (int i = 1; i <= 2000000; i++){
		//check if at left border of beach
		if (i < 100){
			if (beach[i+100] == true){
				curAmount++;
				curDistance = 100;
				if(curAmount > curHighestAmount){
					best.clear();
					best.insert(i);
					curShortestDistance = 100;
				}
			} else {
				curDistance--;
				best.clear();
				best.insert(i);
			}

		} else if (i+100 > 2000000){
			if (beach[i-101] == false){
				if (curDistance < 0){
					curDistance--;
				} else {
					curDistance++;
				}
				if (std::abs(curDistance))

			}
		}
	}

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
