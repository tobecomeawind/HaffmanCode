#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef HAFF_H
#define HAFF_H
typedef struct { 

	char letter; //буква
	unsigned int count; //количество    
	
	int lcode;//len code
	uint32_t code;	

} leaf; //лист дерева

typedef struct {

	leaf** buf;
	int size;

} leaf_buf;


leaf * leaf_init    (char);
leaf** bininsert    (leaf**, char, int*);
leaf** bufalloc     (leaf**, int *);
void   qsortt       (leaf**, int , int);
void   print_table  (leaf**, int);

leaf_buf* analyze_file(char []);

#endif
