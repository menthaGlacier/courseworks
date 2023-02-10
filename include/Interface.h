#ifndef INTERFACE_H
#define INTERFACE_H

#include "dependencies.h"
#include "FileList.h"

class Interface {
public:
	Interface();
	~Interface();
	void displayMenu();
private:
	void displayListSelectionMenu();
	void createNewListMenu();
	void openExistingListMenu();
private:
	enum class ListType {
		None, isInt, isDouble, isString
	} listType;

	FileList<int>* intList;
	FileList<double>* doubleList;
	FileList<std::string>* stringList;
};

#endif /* INTERFACE_H */
