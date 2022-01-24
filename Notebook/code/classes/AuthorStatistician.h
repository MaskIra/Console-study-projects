#pragma once
#include "Statistician.h"

class AuthorStatistician : public Statistician {
public:
	AuthorStatistician(Notebook* nb) : Statistician(nb) { };

	map<string, int> get_statistic() {
		statistic = map<string, int>();
		const vector<Note>* notes = (*nb).get_notes();

		for_each((*notes).begin(), (*notes).end(), [&](Note n) {
			string author = n.get_author();
			if (statistic.find(author) == statistic.end()) {
				statistic.insert(pair<string, int>(author, 1));
			}
			else {
				statistic[author] += 1;
			}
			});
		return statistic;
	}
};
