#include "haff.h"
#include "tree.h"
#include "decode.h"
#include <sys/stat.h>

#include <stdio.h>


int main(int argc, char *argv[])
{
	void binary_view(char* filename);

	char *filename;
	leaf** buf;
	leaf_buf* bp;
	node* top;
	int size; 
	long int fisize, fosize;//input file size, output file size
	struct stat fi, fo;

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
	print_table(buf, size);
	encode(filename, top, buf, size);

	decode("example.txt");
	
	destruct_tree(top);	

	stat(filename,      &fi);	
	stat("example.txt", &fo);	
	
	fisize = fi.st_size;
	fosize = fo.st_size;

	printf("\nInput file size bytes: %li\n",  fisize);
	printf("\nOutput file size bytes: %li\n", fosize);
	printf("\nCompression coefficent: %f\n", (float)fisize / fosize);

	free(buf);
	free(bp);
	
	return 0;	
}
