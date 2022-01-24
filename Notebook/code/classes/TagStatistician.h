#pragma once
#include "Statistician.h"

class TagStatistician : public Statistician {
public:
	TagStatistician(Notebook* nb) : Statistician(nb) { };

	map<string, int> get_statistic() {
		statistic = map<string, int>();
		const vector<Note>* notes = (*nb).get_notes();

		for_each((*notes).begin(), (*notes).end(), [&](Note n) {
			vector<string> tags = n.get_tags();
			for (auto const& el : tags) {
				if (statistic.find(el) == statistic.end()) {
					statistic.insert(pair<string, int>(el, 1));
				}
				else {
					statistic[el] += 1;
				}
			}
			});
		return statistic;
	}
};
