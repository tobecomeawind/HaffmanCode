#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "haff.h"// так как tree.h зависит от haff.h, они имеют такой орядок включения
#include "tree.h"

#include "decode.h"

#define ON 0b10000000 // to check the first bit on/off to get the path on - right(1), off - left(0)



void desirialization_tree(node* head, FILE* fp)//можно сэкономить место если в один байт впихивать 2 прохождения дерева
											   //вместо 11111001
											   //       1011 1100
{
	char c;

	if((c = fgetc(fp)) == END){
		return;
	}

	if((c & isLEAF) == isLEAF){
		head->lp = leaf_init(fgetc(fp));
		return;
	}	

	if((c & LEFT) == LEFT)
		head->left  = init_node(NULL, NULL, NULL);	
	if((c & RIGHT) == RIGHT)
		head->right = init_node(NULL, NULL, NULL);	

	desirialization_tree(head->left,  fp);
	desirialization_tree(head->right, fp);
}


void decode(char* filename)
{
	FILE* fp;
	node* top;
	node* cur;
	char  c;

	fp = fopen(filename, "rb");	

	top = init_node(NULL, NULL, NULL);		
	desirialization_tree(top, fp);

	cur = top;
	putchar('\n');
	fgetc(fp);
	while((c = fgetc(fp)) != EOF){
		for(int i = 0; i < 8; i++){	
			if(cur->lp){
				printf("%c", cur->lp->letter);
				cur = top;
			}

			if((c & ON) == ON)
				cur = cur->right;
			else
				cur = cur->left;

			c <<= 1;
		}	
	}
	putchar('\n');

	fclose(fp);
	destruct_tree(top);
	return;	
}
