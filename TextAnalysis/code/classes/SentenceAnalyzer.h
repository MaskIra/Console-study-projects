#pragma once
#include "Analyzer.h"

class SentenceAnalyzer : public Analyzer {
protected:
    unsigned int qty; // count of sentences
public:
	SentenceAnalyzer(path file) : Analyzer(file) {
        string content = get_content();
        qty = count(content.begin(), content.end(), '.');
    };

    unsigned int get_count() {
        return qty;
	}
};