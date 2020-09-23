#include<iostream>
#include<fstream>
#include<limits>
#include<iomanip>

void testcase(){
	int a; std::cin >> a;
	long b; std::cin >> b;
	std::string c; std::cin >> c;
	double d; std::cin >> d;

	std::cout << std::setprecision(2);

	std::cout << a << " " << b << " " << c << " " << std::fixed << d << std::endl;

}

int main() {
	std::ios_base::sync_with_stdio(false);
	std::ifstream in("./testsets/test1.in");
	std::streambuf *cinbuf = std::cin.rdbuf();
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;


	for (int i = 0; i < t; i++) {
		testcase();
	}

	return 0;

}
