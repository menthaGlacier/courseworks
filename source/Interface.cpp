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
		<< "[3] - Find element" << "\n\t"
		<< "[4] - Output list" << "\n\t"
		<< "[5] - Delete list" << "\n\t"
		<< "[6] - Sort list" << "\n\t"
		<< "[7] - Quit" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);

		if (input == "0") {
			closeActiveList();
			displayListSelectionMenu();
			break;
		} else if (input == "1") {
			insertElementMenu();
			break;
		} else if (input == "2") {
			removeElementMenu();
			break;
		} else if (input == "3") {
			findElementMenu();
			break;
		} else if (input == "4") {
			outputListMenu();
			break;
		} else if (input == "5") {
			deleteListMenu();
			break;
		} else if (input == "6") {
			sortListMenu();
			break;
		} else if (input == "7") {
			closeActiveList();
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
			break;
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
		intList = new FileList<long>(name);
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
		intList = new FileList<long>(input);
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
		<< "[2] - At specific position" << "\t\n"
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
			std::cout << "Enter an index" << std::endl;
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
	std::cout << "Choose how you want to remove element" << "\t\n"
		<< "[1] - At the end of list" << "\t\n"
		<< "[2] - At specific position" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);
		if (input == "1") {
			switch (listType) {
			case ListType::isInt:
				intList->remove();
				break;
			case ListType::isDouble:
				doubleList->remove();
				break;
			case ListType::isString:
				stringList->remove();
				break;
			default:
				break;
			}

			break;
		} else if (input == "2") {
			std::cout << "Enter an index" << std::endl;
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
				intList->remove(std::stoi(input));
				break;
			case ListType::isDouble:
				doubleList->remove(std::stoi(input));
				break;
			case ListType::isString:
				stringList->remove(std::stoi(input));
				break;
			default:
				break;
			}

			break;
		}
	}
}

void Interface::findElementMenu() {
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

	int64_t pos = -1;
	switch (listType) {
	case ListType::isInt:
		pos = intList->find(std::stoi(data));
		break;
	case ListType::isDouble:
		pos = doubleList->find(std::stod(data));
		break;
	case ListType::isString:
		pos = stringList->find(data);
		break;
	default:
		break;
	}

	std::cout << "Search results" << "\n\t";
	if (pos != -1) {
		std::cout << "Found " << data << " in position: " << pos << std::endl;
	} else {
		std::cout << "Nothing was found" << std::endl;
	}
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
	std::cout << "You sure you want to delete this list?" << "\t\n"
		<< "[1] - Yes" << "\t\n"
		<< "[2] - No" << std::endl;

	std::string input;
	while (true) {
		std::cout << ">";
		std::cin.clear();
		std::getline(std::cin, input);
		if (input == "1") {
			switch (listType) {
			case ListType::isInt:
				remove(intList->getListName().c_str());
				delete intList;
				break;
			case ListType::isDouble:
				remove(doubleList->getListName().c_str());
				delete doubleList;
				break;
			case ListType::isString:
				remove(stringList->getListName().c_str());
				delete stringList;
				break;
			default:
				break;
			}

			listType = ListType::None;
			break;
		} else if (input == "2") {
			return;
		}
	}
}

void Interface::sortListMenu() {
	switch (listType) {
	case ListType::isInt:
		intList->sort();
		break;
	case ListType::isDouble:
		doubleList->sort();
		break;
	case ListType::isString:
		stringList->sort();
		break;
	default:
		break;
	}
}

void Interface::closeActiveList() {
	if (intList) {
		delete intList;
		intList = nullptr;
	}

	if (doubleList) {
		delete doubleList;
		doubleList = nullptr;
	}

	if (stringList) {
		delete stringList;
		stringList = nullptr;
	}

	listType = ListType::None;
}
