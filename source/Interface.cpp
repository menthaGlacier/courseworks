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
	std::cout << "Current list: ";
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

		if (input == "0") {
			// TODO
		} else if (input == "1") {
			insertElementMenu();
			break;
		} else if (input == "2") {
			removeElementMenu();
			break;
		} else if (input == "3") {
			changeElementMenu();
			break;
		} else if (input == "4") {
			findElementMenu();
			break;
		} else if (input == "5") {
			outputListMenu();
			break;
		} else if (input == "6") {
			deleteListMenu();
			break;
		} else if (input == "7") {
			sortListMenu();
			break;
		} else if (input == "8") {
			exit(0);
		}
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
			if (input == "q" || input == "Q") {
				std::cout << "The list cannot be named \"q\"" << std::endl;
			} else {
				break;
			}
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
	std::ifstream tryFile;

	std::cout << "Enter existing list name" << std::endl;
	std::cout << "(if you want to quit, enter \"q\")" << std::endl;
	
	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);
		if (input.length() > 0) {
			if (input == "q" || input == "Q") {
				return;
			} else {
				tryFile.open(input);
				if (!tryFile.is_open()) {
					std::cout << "File was not founded" << std::endl;
				} else {
					tryFile.close();
					break;
				}
			}
		}
	}

	switch (input[0]) {
	case 'i':
		listType = ListType::isInt;
		intList = new FileList<int>(input);
		break;
	case 'd':
		listType = ListType::isDouble;
		doubleList = new FileList<double>(input);
		break;
	case 's':
		listType = ListType::isString;
		stringList = new FileList<std::string>(input);
		break;
	default:
		break;
	}
}


void Interface::insertElementMenu() {
	std::cout << "Enter data" << std::endl;

	std::string data;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, data);
		if (data.length() > 0) {
			break;
		}
	}

	std::cout << "Choose how you want to insert element" << "\t\n"
		<< "[1] - At the end of list" << "\t\n"
		<< "[2] - At specifit position" << "\t\n"
		<< "[3] - Using sorting" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);
		if (input == "1") {
			switch (listType) {
			case ListType::isInt:
				intList->insert(std::stoi(data));
				break;
			case ListType::isDouble:
				doubleList->insert(std::stod(data));
				break;
			case ListType::isString:
				stringList->insert(data);
				break;
			default:
				break;
			}

			break;
		} else if (input == "2") {
			std::cout << "Enter index" << std::endl;
			while (true) {
				std::cout << ">";
				std::cin.clear();
				std::getline(std::cin, input);
				if (input.length() > 0) {
					break;
				}
			}

			switch (listType) {
			case ListType::isInt:
				intList->insert(std::stoi(data), std::stoi(input));
				break;
			case ListType::isDouble:
				doubleList->insert(std::stod(data), std::stoi(input));
				break;
			case ListType::isString:
				stringList->insert(data, std::stoi(input));
				break;
			default:
				break;
			}

			break;
		} else if (input == "3") {
			switch (listType) {
			case ListType::isInt:
				intList->insort(std::stoi(data));
				break;
			case ListType::isDouble:
				doubleList->insort(std::stod(data));
				break;
			case ListType::isString:
				stringList->insort(data);
				break;
			default:
				break;
			}

			break;
		}
	}
}

void Interface::removeElementMenu() {
	
}

void Interface::findElementMenu() {

}

void Interface::outputListMenu() {
	std::cout << "Choose an output type" << "\t\n"
		<< "[1] - Regular" << "\t\n"
		<< "[2] - Detailed" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);
		if (input == "1") {
			switch (listType) {
			case ListType::isInt:
				intList->print();
				break;
			case ListType::isDouble:
				doubleList->print();
				break;
			case ListType::isString:
				stringList->print();
				break;
			default:
				break;
			}

			break;
		} else if (input == "2") {
			switch (listType) {
			case ListType::isInt:
				intList->print(true);
				break;
			case ListType::isDouble:
				doubleList->print(true);
				break;
			case ListType::isString:
				stringList->print(true);
				break;
			default:
				break;
			}

			break;
		}
	}
}

void Interface::deleteListMenu() {

}

void Interface::sortListMenu() {

}
