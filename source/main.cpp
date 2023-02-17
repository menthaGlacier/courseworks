/*  */

#include "../include/dependencies.h"
#include "../include/FileList.h"
#include "../include/Interface.h"

int main() {
/*	FileList<int64_t> list("cw.bin");
	list.insert(13);
	list.remove();

	list.insert(220);
	list.insert(110, 0);
	list.insert(-22, 1);
	list.insert(14, 1);
	list.insert(18);
	list.print(true);
	list.remove();
	list.print();

	list.remove(0);
	list.print(true);
	std::cout << "Found at: " << list.find(-22) << std::endl;
	list.insert(100);
	list.insert(12);
	list.print(true);
	//list.sort(); // We don't talk about it rn 
*/
	Interface mainInterface;
	while (true) {
		mainInterface.displayMenu();
	}

	return 0;
}
