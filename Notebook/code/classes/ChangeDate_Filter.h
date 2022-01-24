#pragma once
#include "Filter.h"

class ChangeDate_Filter : public Filter {
public:
	ChangeDate_Filter(Notebook* nb) : Filter(nb) { };
	ChangeDate_Filter(Notebook* nb, vector<Note> old_notes) : Filter(nb, old_notes) { };

	void make_filtration() {
		for_each(old_notes.begin(), old_notes.end(), [&](Note n) {
			if (last_3_days(n.get_change_date())) {
				filter_notes.push_back(n);
			}
			});
	}
};
