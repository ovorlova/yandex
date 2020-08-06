#include <algorithm>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Learner {
private:
	map<string, int> dict;

public:
	int Learn(const vector<string>& words) {
		int newWords = 0;

		for (auto& word : words) {
			if (dict.count(word) == 0) {
				newWords++;
				dict[word] = 1;
			}
		}
		return newWords;
	}

	vector<string> KnownWords() {
		vector<string> vec;
		for (auto& pairs : dict) {
			vec.push_back(pairs.first);
		}
		return vec;
	}
};