#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <algorithm>
#include <vector>
#include <fstream>
#include <math.h>


struct Node {
	int time, tamortized, nodeNr;

	Node(int t, int ta, int nr) : time(t), tamortized(ta), nodeNr(nr){}
};

bool sortBy(Node &n1, Node &n2){
	return n1.time < n2.time;
	
}


void processSubTreeNodes(const std::vector<Node> &nodes, std::vector<bool> &nodeProcessed, std::vector<Node> &order, int nodeNr, int n){
	if (nodeNr > n-1){
		return;
	}

	if (!nodeProcessed[nodeNr]){
		processSubTreeNodes(nodes, nodeProcessed, order, nodeNr*2+1, n);
		processSubTreeNodes(nodes, nodeProcessed, order, nodeNr*2+2, n);
		order.push_back(nodes[nodeNr]);
		nodeProcessed[nodeNr] = true;
	}
}


void testcase() {
	int n; std::cin >> n;
	int nrLevels = std::log2(n+1);
	int counter = 0;
	std::vector<Node> nodes;
	for (int i = 0; i < nrLevels; i++){
		int curLevel = std::pow(2, i);
		int nrNodesInSubtree = std::pow(2, nrLevels- i) - 2;
		for (int j = 0; j < curLevel; j++){
			int temp; std::cin >> temp;
			nodes.push_back(Node(temp, temp-nrNodesInSubtree, counter));
			counter++;
		}
	}

	std::vector<Node> node_copy;
	for (int i = 0; i < n; i++){
		node_copy.push_back(nodes[i]);
	}
	std::sort(node_copy.begin(), node_copy.end(), sortBy);

	std::vector<Node> order;
	std::vector<bool> nodeProcessed(n, false);
	for (std::vector<Node>::iterator it = node_copy.begin(); it != node_copy.end(); it++){
		if (!nodeProcessed[it->nodeNr]){
			processSubTreeNodes(nodes, nodeProcessed, order, nodes[it->nodeNr].nodeNr, n);
			//order.push_back(nodes[i]);
		}
	}

	//NOw iterate thorugh order array and check if explosion somewhere
	bool explosion = false;
	for (int i = 0; i < n; i++){
		if (order[i].time <= i){
			explosion = true;
			break;
		}
	}
	
	if (explosion){
		std::cout << "no" << std::endl;
	} else {
		std::cout << "yes" << std::endl;
	}

	return;
}

int main() {
	std::ios_base::sync_with_stdio(false);
	
	std::fstream in ("./testsets/sample.in");
	std::cin.rdbuf(in.rdbuf());


	int t;
	std::cin >> t;
	for (int i = 0; i < t; ++i)
		testcase();
	return 0;
}
