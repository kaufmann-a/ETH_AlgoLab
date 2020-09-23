#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<limits>
#include<set>



void dfs(std::vector<std::set<int>> &graph, std::vector<bool> &visited, std::vector<int> &discovery, std::vector<int> & finished, int &numb, int curNode){
	visited.at(curNode) = true;
	discovery.at(curNode) = numb; numb++;

	for (std::set<int>::iterator it = graph.at(curNode).begin(); it != graph.at(curNode).end(); it++){
		std::cout << *it << " " << std::endl;
		if (visited.at(*it) == false){
			dfs(graph, visited, discovery, finished, numb, *it);
		}
	}
	finished.at(curNode) = numb; numb++;
}

void dfs2(std::vector<std::set<int>> &graph, std::vector<bool> &visited, std::vector<int> &discovery, std::vector<int> & finished, int v){
	std::stack<int> stack;
	std::stack<int> fin;
	stack.push(v);
	fin.push(v);
	int num = 0;
	while(!stack.empty()){
		v = stack.top();
		stack.pop();

		if (!visited[v]){
			discovery.at(v) = num; num++;
			visited.at(v) = true;

		}

		int counter = 0;
		for(std::set<int>::reverse_iterator rit = graph.at(v).rbegin(); rit != graph.at(v).rend(); rit++){
			if (!visited.at(*rit)){
				stack.push(*rit);
				fin.push(*rit);
				counter++;
			}
		}
		if (counter == 0){
			if (stack.empty()){
				while(!fin.empty()){
					finished.at(fin.top()) = num;
					num++;
					fin.pop();
				}
			} else {
				while (fin.top() != stack.top()){
					finished.at(fin.top()) = num;
					num++;
					fin.pop();
				}
			}
		}
	}
}


void testcase(){
	int n, m, v; std::cin >> n; std::cin >> m; std::cin >> v;

	std::vector<std::set<int>> graph(n);
	std::vector<bool> visited(n, false);
	std::vector<int> discovery(n, -1);
	std::vector<int> finished(n, -1);
	int numb = 0;

	for (int i = 0; i < m; i++){
		int a, b; std::cin >> a; std::cin >> b;
		graph.at(a).insert(b);
		graph.at(b).insert(a);
	}

//	std::set<int>::iterator it = graph.at(477).begin();
//	while (it != graph.at(477).end()){
//		std::cout << (*it) << ", " << std::endl;
//		it++;
//	}

	//dfs(graph, visited, discovery, finished, numb, v);
	dfs2(graph, visited, discovery, finished, v);
	for (std::vector<int>::iterator it = discovery.begin(); it != discovery.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	for (std::vector<int>::iterator it = finished.begin(); it != finished.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << std::endl;

}

int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());

	int t; std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}

	return 0;
}
