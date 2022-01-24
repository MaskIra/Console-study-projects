#pragma once
#include "Notebook.h"
#include <vector>
#include <iostream>
#include <algorithm>

class Filter {
protected:
	Notebook* nb;
	vector<Note> filter_notes; // итоговые записи
	vector<Note> old_notes; // начальные данные (возможна замена вместо данных nb)

	Filter(Notebook* nb) : nb(nb), filter_notes(vector<Note>()) { old_notes = *(nb->get_notes()); };
	Filter(Notebook* nb, vector<Note> old_notes) : nb(nb), filter_notes(vector<Note>()), old_notes(old_notes) { };
public:
	Filter() : nb(nullptr) { };

	// переопределяемые методы
	void update_data() { } // обновление необходимых для фильтрации данных, основанных на old_notes
	void check_data() {	} // поверка полученных выборов фильтрации
	void make_filtration() { } // получение актуальной фильтрации
	void set_search(vector<string> search) { } // изменение выборов для фильтрации

	// получение результата фильтрации
	vector<Note> get_filter_notes() { return filter_notes; }

	// вспомогательные функции:
	// проверка, явлется ли дата "за последние 3 дня"
	static bool last_3_days(time_t datetime) {
		time_t t = time(0);
		double dif = difftime(t, datetime) / (double)(60 * 60 * 24);;

		if (dif <= 3) // 3 дня
			return true;
		return false;
	}
};
