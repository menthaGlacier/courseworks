#ifndef INTERFACE_H
#define INTERFACE_H

#include "dependencies.h"
#include "FileList.h"
#include "FileListString.h"

class Interface {
public:
	Interface();
	~Interface();
	void displayMenu();
private:
	void displayListSelectionMenu();
	void createNewListMenu();
	void openExistingListMenu();

	void insertElementMenu();
	void removeElementMenu();
	void findElementMenu();
	void outputListMenu();
	void deleteListMenu();
	void sortListMenu();
	void closeActiveList();
private:
	enum class ListType {
		None, isInt, isDouble, isString
	} listType;

	FileList<long>* intList;
	FileList<double>* doubleList;
	FileList<std::string>* stringList;
};

#endif /* INTERFACE_H */
