#include "../include/Interface.h"

Interface::Interface() {
	listType = ListType::None;
	intList = nullptr;
	doubleList = nullptr;
	stringList = nullptr;

	displayListSelectionMenu();
}

Interface::~Interface() {
	if (intList != nullptr) {
		delete intList;
	}

	if (doubleList != nullptr) {
		delete doubleList;
	}

	if (stringList != nullptr) {
		delete stringList;
	}
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

	std::cout << "Avaliable actions:" << "\n\t"
		<< "[0] - Change working list" << "\n\t"
		<< "[1] - Insert element" << "\n\t"
		<< "[2] - Remove element" << "\n\t"
		<< "[3] - Change element" << "\n\t"
		<< "[4] - Find element" << "\n\t"
		<< "[5] - Output list" << "\n\t"
		<< "[6] - Delete list" << "\n\t"
		<< "[7] - Sort list" << "\n\t"
		<< "[8] - Quit" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);

	// TODO
	}
}

void Interface::displayListSelectionMenu() {
	std::cout << "List selection" << std::endl;
	std::cout << "Avaliable actions:" << "\n\t"
		<< "[1] - Create a new list" << "\n\t"
		<< "[2] - Open an existing list" << "\n\t"
		<< "[3] - Quit" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);

		if (input == "1") {
			createNewListMenu();
			break;
		} else if (input == "2") {
			openExistingListMenu();
			break;
		} else if (input == "3") {
			exit(0);
		}
	}
}

void Interface::createNewListMenu() {
	std::cout << "Select list type:" << "\n\t"
		<< "[1] - Integer number" << "\n\t"
		<< "[2] - Float point number" << "\n\t"
		<< "[3] - String" << std::endl;

	std::string input, name;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);

		if (input == "1") {
			listType = ListType::isInt;
			name = "i-";
			break;
		} else if (input == "2") {
			listType = ListType::isDouble;
			name = "d-";
			break;
		} else if (input == "3") {
			listType = ListType::isString;
			name = "s-";
			exit(0);
		}
	}

	std::cout << "Enter list name" << std::endl;
	
	input.clear();
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);
		if (input.length() > 0) {
			break;
		}
	}

	name += input;
	std::cout << "Result list name is \"" << name << "\"" << std::endl;
	
	switch (listType) {
	case ListType::isInt:
		intList = new FileList<int>(name);
		break;
	case ListType::isDouble:
		doubleList = new FileList<double>(name);
		break;
	case ListType::isString:
		stringList = new FileList<std::string>(name);
		break;
	default:
		break;
	}
}

void Interface::openExistingListMenu() {
	// TODO
}
