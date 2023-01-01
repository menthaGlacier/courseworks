#ifndef FILE_LIST_H
#define FILE_LIST_H

#include "dependencies.h"
#include "Node.h"

template <class T> class FileList {
public:
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

	void insert(const T& _data) {
		Node<T> tail;
		int64_t pos;

		if (size == 0) {
			file.seekg(0);
			first = sizeof(first) * 2; last = sizeof(last) * 2;
			tail.data = _data; tail.prev = -1; tail.next = -1;
			file.write(reinterpret_cast<char*>(&first), sizeof(first));
			file.write(reinterpret_cast<char*>(&last), sizeof(last));
			file.write(reinterpret_cast<char*>(&tail.prev), sizeof(tail.prev));
			file.write(reinterpret_cast<char*>(&tail.next), sizeof(tail.next));
			file.write(reinterpret_cast<char*>(&tail.data), sizeof(tail.data));
			size += 1;
		}

		//
		file.seekg(last);
		file.seekg(0, std::ios::end);
		tail.data = _data; tail.prev = last; tail.next = -1;
		file.write(reinterpret_cast<char*>(&tail.prev), sizeof(tail.prev));
		file.write(reinterpret_cast<char*>(&tail.next), sizeof(tail.next));
		file.write(reinterpret_cast<char*>(&tail.data), sizeof(tail.data));
		size += 1;
	}

private:
	//
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
		file.write(reinterpret_cast<char*>(&first), sizeof(first));
		file.write(reinterpret_cast<char*>(&last), sizeof(last));
		file.seekg(0);
	}

private:
	std::fstream file;
	int64_t first, last;
	uint32_t size;
};

#endif /* FILE_LIST_H */
