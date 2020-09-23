#include<iostream>
#include<fstream>
#include<set>

void testcase(){
	int q; std::cin >> q;
	std::set<int> s;
	//std::cout << q << std::endl;
	//Work through inputs
	for (int i = 0; i < q; i++){
		int a, b; std::cin >> a; std::cin >> b;

		if (a == 0){
			s.insert(b);

		} else {
			std::set<int>::iterator it = s.find(b);
			if (it != s.end()){
				s.erase(it);
			}

		}
		//std::cout << i << " ";
		//std::cout << a << " ";
		//std::cout << b << " ";
		//std::cout << std::endl;
	}

	//Print remaining set
	if (s.size() == 0){
		std::cout << "Empty" << std::endl;
		return;
	}

	for (std::set<int>::iterator it = s.begin(); it != s.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::ifstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int n; std::cin >> n;

	for (int i = 0; i < n; i++){
		testcase();
	}
	return 0;
}
