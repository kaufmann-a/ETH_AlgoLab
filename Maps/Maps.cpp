#include<iostream>
#include<fstream>
#include<map>
#include<set>

void testcase(){
	int q; std::cin >> q;
	std::multimap<std::string, int> mp;

	for (int i = 0; i < q; i++){
		int a; std::cin >> a;
		std::string b; std::cin >> b;

		if (a == 0){
			std::multimap<std::string, int>::iterator it = mp.find(b);
			mp.erase(it);
		} else {
			mp.insert(std::make_pair(b, a));
		}
	}

	std::string st; std::cin >> st;
	typedef std::multimap<std::string, int>::iterator mmpi;
	std::pair <mmpi, mmpi> ret = mp.equal_range(st);

	if (mp.find(st) == mp.end()){
		std::cout << "Empty" << std::endl;
		return;
	}

	std::set<int> set;
	for (std::multimap<std::string, int>::iterator it = ret.first; it != ret.second; it++){
		set.insert(it->second);
	}

	for (std::set<int>::iterator it = set.begin(); it != set.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}

	return 0;
}
