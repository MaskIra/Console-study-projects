#include <Windows.h>
#include <limits>
#include "NbHandler.h"
#include "TagStatistician.h"
#include "AuthorStatistician.h"
#include "Tags_Filter.h"
#include "Author_Filter.h"
#include "ChangeDate_Filter.h"
#include "CreationDate_Filter.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	Notebook nb = Notebook();
	NbHandler nbh = NbHandler(&nb);

	string input; // переменная для пользовательского ввода
	string author; // текующий пользователь (автор)
	char choice; // переменная пользовательского выбора
	bool trig = true;

	cout << "--- Записная книжка ---" << endl;
	cout << "Здравствуй, автор! Пожалуйста, представься: ";
	getline(cin, author);

	while (trig) {
		cout << endl << "--- Меню: ---" << endl
			<< "1. Добавить запись" << endl
			<< "2. Изменить/удалить запись" << endl
			<< "3. Прочитать запись" << endl
			<< "4. Вывести список записей" << endl
			<< "5. Сделать поиск по записям" << endl
			<< "6. Вывести список записей по фильтрам" << endl
			<< "7. Загрузить записи из файла" << endl
			<< "8. Загрузить записи в файл" << endl
			<< "9. Ввывести статистику по записям" << endl
			<< "Иное - выход" << endl;

		getline(cin, input);
		choice = input[0];

		switch (choice) {
		case '1': {
			// добавление записи
			string note;
			cout << author << ", введите вашу запись в одну строку и нажмите enter:" << endl;
			getline(cin, note);

			vector<string> tags = vector<string>();
			cout << "Введите теги через enter. Для окончания ввода введите строку, начинающюся с 0" << endl;
			while (true) {
				getline(cin, input);
				if (input[0] == '0') break;
				tags.push_back(input);
			}

			nb.add_note(note, author, tags);
			break;
		}
		case '2': {
			// измнение/удаление записи
			cout << endl << "С какой записью вы хотите провести действие? (введите её номер из основного (не отфильтрованного) списка) : ";
			getline(cin, input);
			unsigned int index = stoi(input) - 1;
			if (index < 0 || index >= nb.size()) {
				cout << "Записи с таким номером нет, попробуйте снова" << endl;
				break;
			}

			cout << endl << "Вы желаете удалить запись или изменить её? (у-удалить, иное-изменить)";
			getline(cin, input);
			if (input[0] == 'у') {
				// удаление записи
				nb.delete_note(index);
				cout << "Запись успешно удалена" << endl;
			}
			else {
				// изменение записи
				string note;
				cout << author << ", введите новую запись в одну строку и нажмите enter:" << endl;
				getline(cin, note);

				vector<string> tags = vector<string>();
				cout << "Введите теги (они будут перезаписаны) через enter. Для окончания ввода введите строку, начинающюся с 0" << endl;
				while (true) {
					getline(cin, input);
					if (input[0] == '0') break;
					tags.push_back(input);
				}

				nb.modify_note(index, note, author, tags);
			}
			break;
		}
		case '3': {
			// чтение содержания записи
			cout << endl << "Какую запись вы хотите прочитать? (введите её номер из основного (не отфильтрованного) списка) : ";
			getline(cin, input);
			unsigned int index = stoi(input) - 1;
			if (index < 0 || index >= nb.size()) {
				cout << "Записи с таким номеом нет, попробуйте снова" << endl;
				break;
			}
			nbh.display_note(index);
			break;
		}
		case '4': {
			// вывод списка записей
			nbh.display_notebook();
			break;
		}
		case '5': {
			// поиск
			string filter_by_author = ""; // искомый автор
			vector<string> filter_by_tags = vector<string>(); // искомые теги

			// поиск по автору
			cout << "Желаете сделать поиск по автору? (д-да, иное-нет)" << endl;
			getline(cin, input);
			if (input[0] == 'д') {
				cout << "Введите имя автора, которого хотите найти (регистр важен): ";
				getline(cin, filter_by_author);
			}

			// поиск по тегам
			cout << "Желаете сделать поиск по тегам? (д-да, иное-нет)" << endl;
			getline(cin, input);
			if (input[0] == 'д') {
				cout << "Введите теги через enter. Для окончания ввода введите строку, начинающюся с 0" << endl;
				while (true) {
					getline(cin, input);
					if (input[0] == '0') break;
					filter_by_tags.push_back(input);
				}
			}
			nbh.display_notebook_with_search(filter_by_author, filter_by_tags);
			break;
		}
		case '6': {
			// фильтрация
			vector<Note> filter_notes = vector<Note>(); // сохранение промежуточного результата фильтрации. Итоговый результат

			// по авторам
			cout << "Желаете сделать фильтрацию по авторам? (д-да, иное-нет)" << endl;
			getline(cin, input);
			if (input[0] == 'д') {
				Author_Filter af = Author_Filter(&nb); // класс для фильтрации, список для фильтрации - весь
				vector<string> filter_by_authors = vector<string>(); // выбранные пользователем авторы

				// получение и вывод доступного списка
				const vector<string>* all_authors = af.get_all_authors();
				int i = 1;
				for_each((*all_authors).begin(), (*all_authors).end(), [&](string author) {
					cout << i << "\t" << author << endl; i++;
					});
				cout << "Введите номера авторов, которых хотите видеть в списке через enter. Для окончания ввода введите строку, начинающюся с 0" << endl;
				while (true) {
					// выбор авторов
					getline(cin, input);
					if (input[0] == '0') break;
					unsigned int index = stoi(input) - 1;
					if (index < 0 || index >= all_authors->size())
					{
						cout << "Слишком много/мало" << endl;
						break;
					}
					else
						filter_by_authors.push_back((*all_authors)[index]);
				}

				// установка авторов для фильтрации и сохранение промежуточных результатов
				af.set_search(filter_by_authors);
				af.make_filtration();
				filter_notes = af.get_filter_notes();
			}

			// по дате создания
			cout << "Желаете вывести только записи, созданные за последние 3 дня? (д-да, иное-нет)" << endl;
			getline(cin, input);
			if (input[0] == 'д') {
				CreationDate_Filter cdf = CreationDate_Filter(&nb, filter_notes);
				cdf.make_filtration();
				filter_notes = cdf.get_filter_notes();
			}

			// по дате изменения
			cout << "Желаете вывести только записи, изменённые за последние 3 дня? (д-да, иное-нет)" << endl;
			getline(cin, input);
			if (input[0] == 'д') {
				ChangeDate_Filter cdf = ChangeDate_Filter(&nb, filter_notes);
				cdf.make_filtration();
				filter_notes = cdf.get_filter_notes();
			}

			// по тегам
			cout << "Желаете сделать поиск по тегам? (д-да, иное-нет)" << endl;
			getline(cin, input);
			if (input[0] == 'д') {
				Tags_Filter tf = Tags_Filter(&nb, filter_notes);
				vector<string> filter_by_tags = vector<string>(); // выбранные пользователем тэги

				// вывод всех доступных тегов и выбор из них
				const vector<string>* all_tags = tf.get_all_tags();
				int i = 1;
				for_each((*all_tags).begin(), (*all_tags).end(), [&](string tag) {
					cout << i << "\t" << tag << endl; i++;
					});
				cout << "Введите номера тегов, которых хотите видеть в списке через enter. Для окончания ввода введите строку, начинающюся с 0" << endl;
				while (true) {
					getline(cin, input);
					if (input[0] == '0') break;
					unsigned int index = stoi(input) - 1;

					if (index < 0 || index >= all_tags->size())
					{
						cout << "Слишком много/мало" << endl;
						break;
					}
					else
					filter_by_tags.push_back((*all_tags)[index]);
				}

				// установка тегов для фильтрации и сохранение промежуточных результатов
				tf.set_search(filter_by_tags);
				tf.make_filtration();
				filter_notes = tf.get_filter_notes();
			}

			// отображение отфильтрованного списка
			NbHandler::display_notes(filter_notes);
			break;
		}
		case '7': {
			// чтение данных из файла (подгрузка)
			cout << "Введите полный путь к файлу (вместе с файлом и расширением .notes): ";
			getline(cin, input);

			// обработка ошибок
			ifstream iff(input);
			if (iff.bad()) {
				cout << "Неверно указан путь" << endl;
				break;
			}
			if (input.substr(input.find_last_of(".") + 1) != "notes") {
				cout << "Неверное расширение файла (необходимо .notes)" << endl;
				break;
			}

			nbh.read_notes(input);
			cout << "Готово!" << endl;
			cin.ignore();
			break;
		}
		case '8': {
			// сохранение данных (выгрузка)
			string path;
			cout << "Введите путь, куда сохранить файл (внимание! файл будет перезаписан): ";
			getline(cin, path);
			cout << "Введите имя файла или 0 - значение по умолчанию: ";
			getline(cin, input);
			if (input[0] == '0') {
				nbh.write_notes(path);
				break;
			}
			nbh.write_notes(path, input);
			cout << "Готово!" << endl;
			break;
		}
		case '9': {
			// статистика
			cout << "Статистика по авторам (количество записей для каждого автора):" << endl;
			AuthorStatistician as = AuthorStatistician(&nb);
			as.get_statistic();
			as.display_statistic();
			cout << "Статистика по тегам (количество записей для каждого тега):" << endl;
			TagStatistician ts = TagStatistician(&nb);
			ts.get_statistic();
			ts.display_statistic();
			break;
		}
		default: {
			// безопасный выход
			cout << "Вы уверены, что хотите выйти? Несохранённые в файл записи будут утеряны (д - выйти, иное - остаться)" << endl;
			getline(cin, input);
			choice = input[0];
			if (choice == 'д') {
				trig = false;
			}
			break;
		}
		}
	}
	return 0;
}