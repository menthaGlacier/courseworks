#ifndef FILE_LIST_H
#define FILE_LIST_H

#include "dependencies.h"
#include "Node.h"

template <class T> class FileList {
public:
	// Конструктор файлового списка по умолчанию запрещен
	FileList() = delete;

	// Конструктор файлового списка с заданным именем файла. Пробует открыть
	// существующий файл, либо создать нвоый, если файл отсутствует
	FileList(std::string _name) {
		name = _name;
		file.open(name, std::ios::binary | std::ios::in | std::ios::out);
		if (!file.is_open()) {
			createAndOpenFile(name);
		}

		if (file.peek() == EOF) {
			std::cerr << "ERR: File is empty even though it shouldn't be"
				<< "\n" << "Consider to delete file \"" << name << "\""
				<< std::endl;
			exit(2);
		}

		file.read(reinterpret_cast<char*>(&first), sizeof(first));
		file.read(reinterpret_cast<char*>(&last), sizeof(last));
		if (file.fail()) {
			std::cerr << "ERR: Can't read first and/or last file pointer(s)"
				<< "\n" << "Consider to delete file \"" << name << "\""
				<< std::endl;
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

		// Если список пуст, первый элемент будет после указателей списка.
		// Иначе, перезаписываем указатель на следующий элемент
		if (size == 0) {
			first = sizeof(first) + sizeof(last);
		} else {
			file.seekg(last);
			overwriteNodePointers(-2, pos);
			file.seekg(pos);
		}

		tail.data = _data; tail.prev = last; tail.next = -1;
		last = file.tellg();
		overwriteListPointers();
		file.seekg(0, std::ios::end);
		tail.write(file);
		size += 1;
	}

	void insert(const T& _data, uint32_t index) {
		Node<T> tail;
		int64_t pos, _first;

		file.clear();
		if (index + 1 > size) {
			std::cout << "Index is higher than a list size" << std::endl
				<< "Inserting new element at the end" << std::endl;
			insert(_data);
			return;
		}

		if (index == 0) {
			tail.data = _data; tail.prev = -1; tail.next = first;
			file.seekg(0, std::ios::end);
			pos = file.tellg();
			tail.write(file);
			file.seekg(first);
			overwriteNodePointers(pos, -2);
			first = pos;
			overwriteListPointers();
			size += 1;
			return;
		}
	}

	// Удаление элемента с конца списка
	void remove() {
		Node<T> tail;
		int64_t pos, _first, _last;

		if (size == 0) {
			std::cout << "List's already empty" << std::endl;
			return;
		}

		// Для удаления элемента мы создаем свап-файл, в который будет записан
		// список без последнего элемента. Т.к. этот элемент может быть в любом
		// месте файла, мы проходим по всему списку
		std::fstream swapFile("swap.tmp", std::ios::binary | std::ios::out);
		if (!swapFile.is_open()) {
			std::cerr << "ERR: Can't open swap file for deletion" << std::endl;
			return;
		}

		file.clear();
		file.seekg(0);
		file.read(reinterpret_cast<char*>(&_first), sizeof(_first));
		file.read(reinterpret_cast<char*>(&_last), sizeof(_last));

		// Если указатели списка совпадают, то список либо пуст, либо содержит
		// только один элемент. Новосозданный файл подходит этому
		if (_first == _last) {
			file.close(); swapFile.close();
			std::remove("swap.tmp");
			createAndOpenFile(name);
			size -= 1;
			return;
		} else {
			swapFile.write(reinterpret_cast<char*>(&_first), sizeof(_first));
			swapFile.write(reinterpret_cast<char*>(&_last), sizeof(_last));
		}

		for (uint32_t i = 0; i < size; i++) {
			int64_t tempPos = file.tellg();
			if (tempPos == last) {
				continue;
			}

			tail.read(file);
			if (tail.next == last) {
				tail.next = -1;
				pos = tempPos; 
			}

			tail.write(swapFile);
		}

		file.close(); swapFile.close();
		std::remove(name.c_str());
		std::rename("swap.tmp", name.c_str());
		size -= 1; first = _first; last = pos;
		file.open(name, std::ios::binary | std::ios::in | std::ios::out);
		overwriteListPointers();
	}

	void print() {
		std::cout << "List from " << name << " file:" << std::endl;
		if (size == 0) {
			std::cout << "EMPTY" << std::endl;
		} else {
			Node<T> tail;
			file.seekg(first);
			for (uint32_t i; i < size; i++) {
				tail.read(file);
				std::cout << "\t" << "Element #" << i << " "
					<< tail.data << std::endl;
				file.seekg(tail.next);
			}
		}
	}

private:
	// Используется для (пере)создания файла, если его не существует или доступ
	// к нему не может быть осуществлен. В самом начале записывает указатели
	// списка со значениями -1. Ставит файловый указатель на начало файла
	void createAndOpenFile(std::string _name) {
		file.open(_name, std::ios::out);
		if (!file.is_open()) {
			std::cerr << "ERR: Can't create a file" << std::endl;
			exit(1);
		}

		file.close();
		file.open(_name, std::ios::binary | std::ios::in | std::ios::out);
		if (!file.is_open()) {
			std::cerr << "ERR: Can't open new file" << std::endl;
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

	// Перезаписывает указатели элемента списка на новые. Подразумевает, что
	// указатель файла на начале элемента. В конце метода возвращается на него.
	// Если полученный аргумент -2, то позиция указателя остаётся прежней
	void overwriteNodePointers(int64_t _prev, int64_t _next) {
		file.clear();
		int64_t nodePos = file.tellg();

		if (_prev == -2) {
			file.seekg(sizeof(_prev), std::ios::cur);
		} else {
			file.write(reinterpret_cast<char*>(&_prev), sizeof(_prev));
		}

		if (_next == -2) {
			file.seekg(sizeof(_next), std::ios::cur);
		} else {
			file.write(reinterpret_cast<char*>(&_next), sizeof(_next));
		}

		file.seekg(nodePos);
	}

private:
	std::string name;
	std::fstream file;
	int64_t first, last;
	uint32_t size;
};

#endif /* FILE_LIST_H */
