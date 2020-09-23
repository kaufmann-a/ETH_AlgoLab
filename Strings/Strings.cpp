#include<iostream>
#include<fstream>
#include <string>


std::string reverse(std::string str){
	for (int i = 0; i < str.length() / 2; i++){
		char temp = str[i];
		str[i] = str[str.length() - i - 1];
		str[str.length() - i - 1] = temp;
	}

	return str;
}

void testcase(){
	std::string s1; std::cin >> s1;
	std::string s2; std::cin >> s2;

	std::cout << s1.length() << " " << s2.length() << std::endl;
	std::cout << s1 + s2 << std::endl;

	s1 = reverse(s1);
	s2 = reverse(s2);

	char temp = s1[0];
	s1[0] = s2[0];
	s2[0] = temp;

	std::cout << s1 << " " << s2 << std::endl;
}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::ifstream in("./testsets/test1.in");
	std::streambuf *cinbuf = std::cin.rdbuf();
	std::cin.rdbuf(in.rdbuf());

	int t = 0; std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}

	return 0;
}
