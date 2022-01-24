#pragma once
#include "WordAnalyzer.h"

class FrequencyWordAnalyzer : public WordAnalyzer {
public:
	// constructor
	FrequencyWordAnalyzer(path file, 
		bool ignore_case = false, bool ignore_special_characters = false, bool ignore_numbers = false, bool ignore_another_language = false,
		map<string, int> words = map<string, int>())
		: WordAnalyzer(file, ignore_case, ignore_special_characters, ignore_numbers, ignore_another_language, words)
	{
		setup_words();
	};

	// frequency for each unique word
	void setup_words() {
		string* content = get_clean_content();
		string word = "";
		for (char c : *content) {
			if (is_delimiter(c) && word != "") {
				if (words.find(word) == words.end()) {
					// not found in map
					words.insert(pair<string, int>(word, 1));
				}
				else {
					// found in map
					words[word] += 1;
				}
				// next
				word = "";
			}
			else {
				// make word
				word.push_back(c);
			}
		}
		self_clean();
	}
};