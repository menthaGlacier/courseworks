/*  */

#include "../include/dependencies.h"
#include "../include/FileList.h"

int main() {
	FileList<int64_t> list("cw.bin");
	list.insert(13);
	list.remove();
	list.insert(220);
	list.insert(110);
	list.insert(-22);
	list.insert(14);
	list.remove();
	return 0;
}
