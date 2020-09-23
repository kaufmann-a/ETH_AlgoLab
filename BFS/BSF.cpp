#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<limits>
#include<set>
#include<queue>





void bfs2(std::vector<std::set<int>> &graph, std::vector<int> &visited, std::vector<int> &distance, int v){
	std::queue<int> queue;
	queue.push(v);
	distance.at(v) = 0;
	visited.at(v) = 0;

	while(!queue.empty()){
		v = queue.front();
		queue.pop();
		for(std::set<int>::iterator it = graph.at(v).begin(); it != graph.at(v).end(); it++){
			if (visited.at(*it) == -1){
				visited.at(*it) = v;
				queue.push(*it);
				distance.at(*it) = distance.at(v) + 1;
			}
		}
	}
}


void testcase(){
	int n, m, v; std::cin >> n; std::cin >> m; std::cin >> v;

	std::vector<std::set<int>> graph(n);
	std::vector<int> visited(n, -1);
	std::vector<int> distance(n, -1);

	for (int i = 0; i < m; i++){
		int a, b; std::cin >> a; std::cin >> b;
		graph.at(a).insert(b);
		graph.at(b).insert(a);
	}

	//dfs(graph, visited, discovery, finished, numb, v);
	bfs2(graph, visited, distance, v);
	for (std::vector<int>::iterator it = distance.begin(); it != distance.end(); it++){
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
