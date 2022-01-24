#pragma once
#include "Analyzer.h"
#include <map>

class WordAnalyzer : public Analyzer {
private:
	string content; // of current file
	// processing setup
	bool ignore_special_characters;
	bool ignore_numbers;
	bool ignore_another_language;
protected:
	map<string, int> words; // map with non-repeating words and statistics for each word

	// clean <words> by processing setup
	void self_clean() {
		map<string, int>::iterator it = words.begin();
		bool I_did_not_delete;
		while (it != words.end())
		{
			I_did_not_delete = true;
			// special characters = not digit & not alpha
			if (ignore_special_characters) {
				if (find_if(it->first.begin(), it->first.end(), [](unsigned char c) { return !isdigit(c) && !isalpha(c); }) != it->first.end()) {
					it = words.erase(it);
					I_did_not_delete = false;
				}
			}
			if (ignore_numbers) {
				if (find_if(it->first.begin(), it->first.end(), ::isdigit) != it->first.end()) {
					it = words.erase(it);
					I_did_not_delete = false;
				}
			}
			// another language = not russian
			if (ignore_another_language) {
				if (find_if(it->first.begin(), it->first.end(), [&](unsigned char c) { return !IsRussianChar1251(c); }) != it->first.end()) {
					it = words.erase(it);
					I_did_not_delete = false;
				}
			}
			if (I_did_not_delete)
				it++;
		}
	};
public:
	// constructor
	WordAnalyzer(path file, bool ignore_case = false,
		bool ignore_special_characters = false, bool ignore_numbers = false, bool ignore_another_language = false,
		map<string, int> words = map<string, int>()) : Analyzer(file), words(words), ignore_special_characters(ignore_special_characters), ignore_numbers(ignore_numbers), ignore_another_language(ignore_another_language) {
		content = get_content();
		if (ignore_case) {
			transform(content.begin(), content.end(), content.begin(),
				[](unsigned char c) { return tolower(c); });
		}
	};

	string* get_clean_content() { return &content; }
	map<string, int> get_words() { return words; }

	// setup <words>
	virtual void setup_words() { };


	// user requests: a ranked "list" of words based on specific statistics
	static map<int, string> get_ranked_map(map<string, int> old_words) {
		// change <string, int> to <int, string>
		map<int, string> new_words;
		for (pair<string, int> el : old_words) {
			if (new_words.find(el.second) == new_words.end()) {
				// not found
				new_words.insert(pair<int, string>(el.second, el.first));
			}
			else {
				// found
				new_words[el.second] += ", " + el.first;
			}
		}
		return new_words;
	}

	// user requests: tip-most-"lists" of words based on specific statistics
	static vector<pair<string, int>> get_top_count(map<string, int> old_words, int count) {
		// sort by value, not by key
		vector<pair<string, int>> A;
		for (auto& it : old_words) { A.push_back(it); }
		sort(A.begin(), A.end(), [](pair<string, int>& a, pair<string, int>& b)
			{
				return a.second > b.second;
			});
		// get first(=most) <count>
		if ((size_t)count >= A.size())
			return A;
		vector<pair<string, int>> B(A.begin(), A.begin() + count);
		return B;
	}

	static bool is_delimiter(char c) {
		return (c == '.' || c == ',' || c == '!' || c == '-' || c == ' ' || c == '\n' || c == '\r');
	}

	constexpr bool IsRussianChar1251(const char c)
	{
		return (c >= 'À' && c <= 'ÿ') || c == '¸' || c == '¨';
	}
};