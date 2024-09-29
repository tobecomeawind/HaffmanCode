#include "analyzer.h"
#include "encoder.h"
#include "decoder.h"

#define ON 0b10000000 // to check the first bit on/off to get the path on - right(1), off - left(0)



void desirialization_tree(node* head, FILE* fp)
{
	 /**************************************\
	|     From binary view to struct tree    |
	|         reverse serialization          |
 	 \**************************************/	 

	/**********************************
	***********************************
	******* See the header file,*******
	*******      encoder.h      *******
	*******  before you reading *******
	*******     a function      *******
	***********************************
	**********************************/

	char c;
	
	//stop point
	if((c = fgetc(fp)) == END){ //
		return;
	}

	//stop point
	if((c & isLEAF) == isLEAF){
		//if c == 0b11111100
		//we write the next value(letter)
		head->lp = leaf_init(fgetc(fp));
		return;
	}	

	//creating left children
	if((c & LEFT) == LEFT)
		head->left  = init_node(NULL, NULL, NULL);	
	//creating right children	
	if((c & RIGHT) == RIGHT)
		head->right = init_node(NULL, NULL, NULL);	

	desirialization_tree(head->left,  fp);
	desirialization_tree(head->right, fp);
	//recursive from left to right
}


void decode(char* filename)
{	
	 /**************************************\
	|       From tree to human view          |
 	 \**************************************/	 	
	FILE* fp;
	node* top;
	node* cur;
	char  c;

	fp = fopen(filename, "rb");	
	
	// creating a haffman tree
	top = init_node(NULL, NULL, NULL);		
	desirialization_tree(top, fp);

	cur = top;
	putchar('\n');
	fgetc(fp);//skip the END value
	while((c = fgetc(fp)) != EOF){
		for(int i = 0; i < 8; i++){		
			if(cur->lp){
				//if searched a leaf
				printf("%c", cur->lp->letter);
				cur = top;
			}
			
			//checking the first bit
			//on  - right(1)
			//off - left (0)
			if((c & ON) == ON)
				cur = cur->right;
			else
				cur = cur->left;
			
			//shifting by change the first bit
			c <<= 1;
		}	
	}
	putchar('\n');

	fclose(fp);
	destruct_tree(top); // free alloced memory
	return;	
}
