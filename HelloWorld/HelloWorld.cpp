#include<iostream>
#include<limits>

int main(){
	std::ios_base::sync_with_stdio(false);

	std::cout << "type\tmin()\t\t\tmax()\t\t\t#binary digits\n";
	std::cout << "int\t"
			  << std::numeric_limits<int>::min() << "\t\t"
			  << std::numeric_limits<int>::max() << "\t\t"
			  << std::numeric_limits<int>::digits << "\n";

	return 0;
}
