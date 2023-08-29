#ifndef NODE_STRING_H
#define NODE_STRING_H

#include "dependencies.h"
#include "Node.h"

template <> class Node<std::string> {
public:
	bool write(std::fstream& file) {
		if (!file.is_open()) {
			return false;
		}
		
		file.clear();

		file.write(reinterpret_cast<const char*>(&prev), sizeof(prev));
		file.write(reinterpret_cast<const char*>(&next), sizeof(next));

        size_t dataSize = data.size();
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
		file.write(reinterpret_cast<const char *>(data.c_str()), dataSize);
	
		if (file.fail()) {
			return false;
		}

		return true;
	}

	bool read(std::fstream& file) {
		if (!file.is_open()) {
			return false;
		}

		file.clear();
		file.flush();

		file.read(reinterpret_cast<char*>(&prev), sizeof(prev));
		file.read(reinterpret_cast<char*>(&next), sizeof(next));

        size_t dataSize = 0;
		char* buffer = nullptr;

        file.read(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
		buffer = new char[dataSize];
        file.read(buffer, dataSize);
        data.assign(buffer, dataSize);
        delete[] buffer;

		if (file.fail()) {
			return false;
		}

		return true;
	}

	uint32_t getNodeSize() const {
		uint32_t totalSize = 0;
		totalSize += (sizeof(prev) + sizeof(next));
		totalSize += sizeof(size_t) + data.size();
		return totalSize;
	}

	// bool operator<(const Node<TT>& l, const Node<TT>& r) const {
	// 	return l.data < r.data;
	// }

	// bool operator>(const Node<std::string>& l, const Node<std::string>& r) const {
	// 	return l.data > r.data;
	// }

public:
	std::string data;
	int64_t prev, next;
};

#endif /* NODE_STRING_H */
