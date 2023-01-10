/*  */

#include "../include/dependencies.h"
#include "../include/FileList.h"

int main() {
	FileList<int64_t> list("cw.bin");
	list.insert(13);
	list.remove();

	list.insert(220);
	list.insert(110, 0);
	list.insert(-22, 1);
	list.insert(14, 1);
	list.debugPrint();
	list.remove();
	list.debugPrint();

	list.insert(95, 0);
	list.debugPrint();

	list.insert(3, 3);
	list.debugPrint();
	list.remove();
	list.debugPrint();
	return 0;
}
