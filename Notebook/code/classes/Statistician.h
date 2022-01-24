#pragma once
#include "Notebook.h"
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

class Statistician {
protected:
	Notebook* nb;
	map<string, int> statistic; // результат

	Statistician(Notebook* nb) : nb(nb), statistic(map<string, int>()) { };
public:
	void display_statistic() {
		for (const auto& elem : statistic)
		{
			cout << elem.second << " - " << elem.first << endl;
		}
	}
};