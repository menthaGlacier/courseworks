#ifndef NODE_H
#define NODE_H

#include "dependencies.h"

template <class TT> class Node {
private:
	bool write(std::fstream& file) {
		if (!file.is_open()) {
			return false;
		}
		
		file.clear();

		file.write(reinterpret_cast<char*>(&prev), sizeof(prev));
		file.write(reinterpret_cast<char*>(&next), sizeof(next));
		file.write(reinterpret_cast<char*>(&data), sizeof(data));
	
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
		file.read(reinterpret_cast<char*>(&data), sizeof(data));
		
		if (file.fail()) {
			return false;
		}

		return true;
	}

	uint32_t getNodeSize() const {
		uint32_t totalSize = 0;
		totalSize += (sizeof(prev) + sizeof(next));
		totalSize += sizeof(data);
		return totalSize;
	}

	bool operator<(const Node<TT>& l, const Node<TT>& r) const {
		return l.data < r.data;
	}

	bool operator>(const Node<TT>& l, const Node<TT>& r) const {
		return l.data > r.data;
	}

private:
	TT data;
	int64_t prev, next;
	template <class T> friend class FileList;
};

#endif /* NODE_H */
