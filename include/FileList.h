#ifndef FILE_LIST_H
#define FILE_LIST_H

#include "dependencies.h"
#include "Node.h"

template <class T> class FileList {
public:
	// Конструктор файлового списка по умолчанию и копирования запрещены, т.к.
	// файл должен иметь имя и быть открыт только единожды
	FileList() = delete;
	FileList(const FileList<T>& copy) = delete;

	// Конструктор файлового списка с заданным именем файла. Пробует открыть
	// существующий файл, либо создать нвоый, если файл отсутствует
	FileList(const std::string& _name) {
		name = _name;
		first = 0, last = 0, size = 0;
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
		file.seekg(first);
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
		int64_t pos;

		if (index + 1 >= size) {
			if (!(index == 0 && size == 0) && index + 1 > size) {
				std::cout << "Index is higher than a list size" << std::endl
					<< "Inserting new element at the end" << std::endl;

				insert(_data);
				return;
			}
		}

		file.clear();
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

		file.seekg(first);
		for (uint32_t i = 0; i < index; i++) {
			pos = file.tellg();
			tail.read(file);
			file.seekg(tail.next);
		}

		tail.data = _data; tail.prev = pos;
		file.seekg(0, std::ios::end);
		pos = file.tellg();
		tail.write(file);
		file.seekg(tail.prev);
		overwriteNodePointers(-2, pos);
		file.seekg(tail.next);
		overwriteNodePointers(pos, -2);
		size += 1;
	}

	void insort(const T& data) {
		insert(data);
		sort();
	}

	void remove() {
		Node<T> tail;
		int64_t pos;

		if (size == 0) {
			std::cout << "List's already empty" << std::endl;
			return;
		} else if (first == last) {
			file.close();
			createAndOpenFile(name);
			return;
		}

		// Для удаления элемента мы создаем свап-файл, в который будет записан
		// список без последнего элемента. В процессе записи в свап-файл список
		// будет "выравнен" т.к. это упрощает операцию и придает порядок списку
		std::fstream swapFile("swap.tmp", std::ios::binary | std::ios::out);
		if (!swapFile.is_open()) {
			std::cerr << "ERR: Can't open swap file for deletion" << std::endl;
			return;
		}

		swapFile.write(reinterpret_cast<char*>(&first), sizeof(first));
		swapFile.write(reinterpret_cast<char*>(&last), sizeof(last));

		file.clear();
		file.seekg(first);
		for (uint32_t i = 0; i < size; i++) {
			pos = swapFile.tellg();
			tail.read(file);

			// Если указатель на предыдущий элемент равен -1, то прочитанный
			// элемент должен быть первым, тогда перезаписываем указатель на
			// первый элемент списка. Иначе элемент является очередным и
			// перезаписывается его указатель на предыдущий элемент
			if (tail.prev == -1) {
				first = pos;
			} else {
				tail.prev = pos - tail.getNodeSize();
			}

			// Аналогично, если указатель на следующий элемент равен указателю
			// на последний элемент списка, то этот элемент становится новым
			// концом спиcка. Иначе элемент очередной, его указатель на
			// следующий элемент перезаписывается на позицию следующего элемент
			// и мы продвигаемся по списку
			if (tail.next == -1) {
				break;
			} else if (tail.next == last) {
				file.seekg(tail.next);
				tail.next = -1;
				last = pos;
			} else {
				file.seekg(tail.next);
				tail.next = pos + tail.getNodeSize();
			}

			tail.write(swapFile);
		}

		file.close(); swapFile.close();
		std::remove(name.c_str());
		std::rename("swap.tmp", name.c_str());
		file.open(name, std::ios::binary | std::ios::in | std::ios::out);
		overwriteListPointers();
		size -= 1;
	}

	void remove(uint32_t index) {
		Node<T> tail;
		int64_t pos;

		if (index + 1 >= size) {
			if (!(index == 0 && size == 0) && index + 1 > size) {
				std::cout << "Index is higher than a list size" << std::endl
					<< "Removing last element" << std::endl;
			}
			
			remove();
			return;
		}

		// Аналогично удалению с конца, список будет перезаписан в свап-файл,
		// кроме элемента находящегося на полученном индексе
		std::fstream swapFile("swap.tmp", std::ios::binary | std::ios::out);
		if (!swapFile.is_open()) {
			std::cerr << "ERR: Can't open swap file for deletion" << std::endl;
			return;
		}

		swapFile.write(reinterpret_cast<char*>(&first), sizeof(first));
		swapFile.write(reinterpret_cast<char*>(&last), sizeof(last));

		file.clear();
		file.seekg(first);
		for (uint32_t i = 0; i < size; i++) {
			pos = swapFile.tellg();
			tail.read(file);
			
			if (i == index && index != 0) {
				file.seekg(tail.next);
				size -= 1;
				continue;
			}

			// Если необходимо удалить первый элемент, пропускаем его и делаем
			// следующий элемент новым первым элементом
			if (tail.prev == - 1) {
				if (index == 0) {
					file.seekg(tail.next);
					tail.read(file);
					tail.prev = -1;
				}

				size -= 1;
				first = pos;
			} else {
				tail.prev = pos - tail.getNodeSize();
			}

			if (tail.next == -1) {
				last = pos;
			} else {
				file.seekg(tail.next);
				tail.next = pos + tail.getNodeSize();
			}

			tail.write(swapFile);
		}

		file.close(); swapFile.close();
		std::remove(name.c_str());
		std::rename("swap.tmp", name.c_str());
		file.open(name, std::ios::binary | std::ios::in | std::ios::out);
		overwriteListPointers();
	}

	int64_t find(const T& _data) {
		Node<T> tail;
		int64_t pos;

		file.clear();
		file.seekg(first);
		for (int64_t i = 0; i < size; i++) {
			pos = file.tellg();
			tail.read(file);
			if (tail.data == _data) {
				return pos;
			}
		}

		return -1;
	}

	// Вывод может быть подробным, т.е. с указанием размера списка, адресов
	// первого и последнего элементов списка и адресов предыдущего и следующего
	// элементов списка. По умолчанию вывод не является подробным
	void print(bool detailed = false) {
		file.clear();
		std::cout << "List from " << name << " file:" << std::endl;
		if (size == 0) {
			std::cout << "EMPTY" << std::endl;
		} else {
			Node<T> tail;
			file.seekg(first);		
			if (detailed) {
				std::cout << "List size: " << size << std::endl;
				std::cout << "First: " << first << " Last: " << last
					<< std::endl;
			}

			for (uint32_t i = 0; i < size; i++) {
				int64_t pos = file.tellg();
				tail.read(file);
				std::cout << "\t" << "Element #" << i << ": " << tail.data;

				if (detailed) {
					std::cout << " | " << "Pos: " << pos << " Prev: "
						<< tail.prev << " Next: " << tail.next << std::endl;
				} else {
					std::cout << std::endl;
				}

				file.seekg(tail.next);
			}
		}

		file.clear();
	}

	void sort() {
		Node<T> tail, prevTail;
		int64_t pos, prevPos;

		if (size < 2) {
			std::cout << "Not enough elements to sort list" << std::endl;
			return;
		}
		
		for (uint32_t i = 0; i < size; i++) {
			file.seekg(first);
			pos = file.tellg();
			tail.read(file);
			
			while (tail.next != -1) {
				prevTail = tail;
				prevPos = pos;
				file.seekg(prevTail.next);
				pos = file.tellg();
				tail.read(file);

				if (prevTail > tail) {
					swapNodes(prevTail, tail, prevPos, pos);
					tail.prev = prevTail.prev;
					tail.next = prevPos;
					pos = prevPos;
				}
			}

			file.clear();
		}
	}

	std::string getListName() {
		return name;
	}

	uint32_t getListSize() {
		return size;
	}

private:
	// Используется для (пере)создания файла, если его не существует или доступ
	// к нему не может быть осуществлен. В самом начале записывает указатели
	// списка со значениями -1. Ставит файловый указатель на начало файла
	void createAndOpenFile(const std::string& _name) {
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

		first = -1; last = -1; size = 0;
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
		file.clear();
	}

	// Перезаписывает указатели элемента списка на новые. Подразумевает, что
	// указатель файла на начале элемента. В конце метода возвращается на него.
	// Если полученный аргумент -2, то позиция указателя остаётся прежней
	void overwriteNodePointers(int64_t _prev, int64_t _next) {
		file.clear();
		int64_t pos = file.tellg();

		if (_prev == -2) {
			file.seekg(sizeof(_prev), std::ios::cur);
		} else {
			file.write(reinterpret_cast<char*>(&_prev), sizeof(_prev));
		}

		file.clear();
		if (_next == -2) {
			file.seekg(sizeof(_next), std::ios::cur);
		} else {
			file.write(reinterpret_cast<char*>(&_next), sizeof(_next));
		}

		file.seekg(pos);
		file.clear();
	}

	void swapNodes(Node<T>& left, Node<T>& right, uint64_t leftPos, uint64_t rightPos) {
		if (left.prev != -1) {
			file.seekg(left.prev);
			overwriteNodePointers(-2, rightPos);
		} else {
			first = rightPos;
		}

		file.clear();

		if (right.next != -1) {
			file.seekg(right.next);
			overwriteNodePointers(leftPos, -2);
		} else {
			last = leftPos;
		}

		file.clear();

		file.seekg(leftPos);
		overwriteNodePointers(rightPos, right.next);
		file.clear();

		file.seekg(rightPos);
		overwriteNodePointers(left.prev, leftPos);
		file.clear();

		overwriteListPointers();
		file.clear();
	}

private:
	std::string name;
	std::fstream file;
	int64_t first, last;
	uint32_t size;
};

#endif /* FILE_LIST_H */
