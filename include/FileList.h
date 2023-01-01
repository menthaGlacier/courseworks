#ifndef FILE_LIST_H
#define FILE_LIST_H

#include "dependencies.h"
#include "Node.h"

template <class T> class FileList {
public:
	// Конструктор файлового списка по умолчанию. Вначале пробует открыть файл
	// либо создать его, если открытие не осуществилось.
	FileList() {
		file.open("CWBin", std::ios::binary | std::ios::in | std::ios::out);
		if (!file.is_open()) {
			createAndOpenFile();
		}

		if (file.peek() == EOF) {
			std::cerr << "ERR: File is empty even though it shouldn't be"
				<< "\n" << "Consider to delete file \"CWBin\"" << std::endl;
			exit(2);
		}

		file.read(reinterpret_cast<char*>(&first), sizeof(first));
		file.read(reinterpret_cast<char*>(&last), sizeof(last));
		if (file.fail()) {
			std::cerr << "ERR: Can't read first and/or last file pointer(s)"
				<< "\n" << "Consider to delete file \"CWBin\"" << std::endl;
			exit(2);
		}

		Node<T> tail;
		while (tail.read(file)) {
			size += 1;
			if (tail.next != -1) {
				file.seekg(tail.next);
			} else {
				break;
			}
		}
	}

	~FileList() {
		if (file.is_open()) {
			file.close();
		}
	}

	// Вставка элемента в конец списка
	void insert(const T& _data) {
		Node<T> tail;
		int64_t pos;

		file.clear();
		file.seekg(0, std::ios::end);
		pos = file.tellg();

		// Если список пуст, первый элемент будет после указателей списка
		if (size == 0) {
			first = sizeof(first) + sizeof(last);
		} else {
			file.seekg(last);
			// TODO
		}

		tail.data = _data; tail.prev = last; tail.next = -1;
		last = file.tellg();
		overwriteListPointers();
		file.seekg(0, std::ios::end);
		tail.write(file);
		size += 1;
	}

private:
	// Используется для (пере)создания файла, если его не существует или доступ
	// к нему не может быть осуществлен. В самом начале записывает указатели
	// списка со значениями -1. Возвращает файловый указатель на начало файла
	void createAndOpenFile() {
		file.open("CWBin", std::ios::out);
		if (!file.is_open()) {
			std::cerr << "ERR: Can't create a file" << std::endl;
			exit(1);
		}

		file.close();
		file.open("CWBin", std::ios::binary | std::ios::in | std::ios::out);
		if (!file.is_open()) {
			std::cerr << "ERR: Can't open the file" << std::endl;
			exit(1);
		}

		first = -1, last = -1;
		overwriteListPointers();
		file.seekg(0);
	}

	// Перезаписывает указатели списка first и last на актуальные версии,
	// оставляет файловый указатель на первом записанном элементе в файле
	void overwriteListPointers() {
		file.clear();
		file.seekg(0);
		file.write(reinterpret_cast<char*>(&first), sizeof(first));
		file.write(reinterpret_cast<char*>(&last), sizeof(last));
	}
private:
	std::fstream file;
	int64_t first, last;
	uint32_t size;
};

#endif /* FILE_LIST_H */
