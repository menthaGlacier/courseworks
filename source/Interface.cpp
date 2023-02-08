#include "../include/Interface.h"

Interface::Interface() {
	listType = ListType::None;
	intList = nullptr;
	doubleList = nullptr;
	stringList = nullptr;

	displayListSelectionMenu();
}

void Interface::displayMenu() {
	std::cout << "Menu" << std::endl;

	if (listType != ListType::None) {
	std::cout << "Current list name: ";
		switch (listType) {
		case ListType::isInt:
			std::cout << intList->getListName() << std::endl;
			break;
		case ListType::isDouble:
			std::cout << doubleList->getListName() << std::endl;
			break;
		case ListType::isString:
			std::cout << stringList->getListName() << std::endl;
			break;
		default:
			break;
		}
	} else {
		std::cout << "List not selected" << std::endl;
		displayListSelectionMenu();
	}

	std::cout << "Avaliable actions:" << std::endl << "\t"
		<< "[0] - Change working list" << "\t"
		<< "[1] - Insert element" << "\t"
		<< "[2] - Remove element" << "\t"
		<< "[3] - Change element" << "\t"
		<< "[4] - Find element" << "\t"
		<< "[5] - Output list" << "\t"
		<< "[6] - Sort list" << "\t"
		<< "[7] - Quit" << std::endl;
}

void Interface::displayListSelectionMenu() {
	std::cout << "Avaliable actions:" << std::endl << "\t"
		<< "[1] - Create a new list" << "\t"
		<< "[2] - Open an existing list" << "\t"
		<< "[3] - Quit" << std::endl;
}
