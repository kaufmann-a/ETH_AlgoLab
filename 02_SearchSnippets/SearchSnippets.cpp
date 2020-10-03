#include<iostream>
#include<fstream>
#include<vector>
#include<limits>
#include<set>
#include<map>
//#include<math>


void testcase(){
    uint n; std::cin >> n;

    std::vector<int> m(n); // nr of times word i occures in the document
    for (uint i = 0; i < n; i++)
    {
        std::cin >> m[i];
    }

    // create map where the key is the index of the words and the value is the word number
    std::map<int, int> index;
    for (uint i_word = 0; i_word < n; i_word++)
    {
        for (int j_position = 0; j_position < m[i_word]; j_position++)
        {
            int position;
            std::cin >> position;
            index.insert(std::make_pair(position, i_word));
        }
    }

    // sliding window approach
    int min_distance = std::numeric_limits<int>::max();
    std::map<int, int>::iterator left = index.begin();
    std::map<int, int>::iterator right = index.begin();
    std::map<int, int>::iterator oldRight = right;
    std::set<int> wordList;
    std::vector<int> wordCountInList(n, 0);
    
    do {
        // increase right until all words are in the list
        while (wordList.size() < n && right != index.end()) {
            // add right word to list
            wordCountInList[right->second]++;
            wordList.insert(right->second);
            // save distance if list contains all words
            if (wordList.size() == n)
            {
                min_distance = std::min(min_distance, right->first - left->first + 1);
            }
            oldRight = right;
            right++;
        }

        // increase left until the list is not full anymore
        while (wordList.size() == n && left != right)
        {
            // save distance if list contains all words
            if (wordList.size() == n)
            {
                min_distance = std::min(min_distance, oldRight->first - left->first + 1);
            }

            wordCountInList[left->second]--;
            if (wordCountInList[left->second] == 0)
            {
                // remove the word form the list if the count is zero
                wordList.erase(wordList.find(left->second));
            }


            left++;
        }
    } while (right != index.cend());

    std::cout << min_distance << std::endl;



}


int main(){
	std::ios_base::sync_with_stdio(false);

	std::fstream in("./testsets/test1.in");
	std::cin.rdbuf(in.rdbuf());

	int t;std::cin >> t;

	for (int i = 0; i < t; i++){
		testcase();
	}



	return 0;
}