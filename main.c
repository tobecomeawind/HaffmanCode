/********************************************************
 *                                                      *
 *                     Haffman Code                     *
 *                                                      *
 *                                                      *
 *                                                      *
 * Purpose:                                             *
 *                                                      *
 *        Coding and Decoding data from input file      *
 *        			with Haffman Code                   *
 *                                                      *
 *                                                      *
 * Author: tobecomeawind                                *
 *                                                      *
 *                                                      *
 * Copyright 2024 by tobecomeawind.                     *
 *                                                      *
 *                                                      *
 * Warning:                                             *
 * 		1. Not use Windows                              *
 * 		2. Max deep of three code <= 32                 *
 *                                                      *
 * Algorithms:                                          *
 * 			All algorithms you can see                  *
 * 			 in  algs.c file                            *
 *                                                      *
 *                                                      *
 * Input  : ./main <input_file>                         *
 * Output :        <example.txt>                        *
 *                                                      *
 ********************************************************/

#include "analyzer.h"
#include "encoder.h"
#include "decoder.h"
#include <sys/stat.h>

#include <stdio.h>


int main(int argc, char *argv[])
{
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

	if(!top){
		printf("\n--Not enough data to encoding--\n\n");
		return -2;
	}

	create_table(top, 0, 0);
	print_table(buf, size);
	encode(filename, top, buf, size); //encoding info

	decode("example.txt"); //decoding info
	
	destruct_tree(top);	   //free alloced memory


	/********************************
	*********************************
	*****  If u not have linux  *****
	***** programm not  working *****
   	*****    (system calls)     *****
	*********************************
	*********************************/
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
