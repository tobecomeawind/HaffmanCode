#include "haff.h"
#include "tree.h"
#include "decode.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	void binary_view(char* filename);

	char *filename;
	leaf** buf;
	leaf_buf* bp;
	node* top;
	int size;

	if(argc == 2)
		filename = *++argv;
	else{
		printf("\n --Invalid argument!-- \n\n");
		return -1;	
	}
	
	bp   = analyze_file(filename);	
	size = bp->size;
	buf  = bp->buf;

	top = create_tree(buf, size);
	
	create_table(top, 0, 0);
	print_leavies(buf, size);
	encode(filename, top, buf, size);

	decode("example.txt");

	//print_leavies(buf, size);

	//binary_view(filename);
	//binary_view("example.txt");

	return 0;	
}
