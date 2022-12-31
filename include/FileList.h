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

		if (file.peek() != EOF) {
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

private:
	std::fstream file;
	int64_t first, last;
	uint32_t size;
};

#endif /* FILE_LIST_H */
