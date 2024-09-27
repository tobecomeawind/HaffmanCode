#include "haff.h"
#include "tree.h"
#include "algs.h"

node* init_node(leaf* lp, node* left, node* right)
{
	node* nptr;

	nptr = (node*) malloc(sizeof(node));
	
	nptr->lp    = lp;
	nptr->count = (lp) ? lp->count : ((left) ? left->count : 0) + ((right) ? right->count : 0);	
	nptr->left  = left;
	nptr->right = right;

	return nptr;	
}

void destruct_tree(node* head)
{
		
	if(head->lp){
		free(head->lp);	
		free(head);
		return;
	}
			
	destruct_tree(head->left);
	destruct_tree(head->right);

	free(head);
}


node* create_tree(leaf** buf, int size)
{
	node* rhead;
	node* lhead;
	FILE *fp;

	node* top;

	if(size > 2){
		lhead = init_node(buf[0], NULL, NULL);
		rhead = init_node(buf[1], NULL, NULL);	
	}

	for(int i = 2; i < size; i++){
	
		if((lhead->count) <= (rhead->count)){
			lhead = init_node(NULL, lhead, rhead);
			rhead = init_node(buf[i], NULL, NULL);
		}else{
			rhead = init_node(NULL, rhead, init_node(buf[i], NULL, NULL));	
		}	
	}		

	lhead = init_node(NULL, lhead, rhead);

	return lhead;
}

void create_table(node* head, int len, uint32_t code)
{
	if(head->lp){
		head->lp->code  = code;
		head->lp->lcode = len;
		return;
	}

	code <<= 1;

	create_table(head->left, ++len, code);	
	code |= 1;
	
	create_table(head->right, len, code);
}


void encode(char* filename, node* head, leaf** buf, int size)
{
	void fencode(FILE* fp, FILE* nfp,
		   	     uint8_t ibit, int iblen,
				 uint32_t rbyte, int rblen,
				 leaf** buf, int size);
	void tencode(char * filename, node* head);
	FILE* fp;
	FILE* nfp;

	int (*func)(leaf* ,leaf*) = &compare_alph;

	qsortt(buf, 0 , size - 1, func);	
	
	tencode("example.txt", head);

	fp  = fopen(filename,      "rb");
	nfp = fopen("example.txt", "awb");	//go to end

	fencode(fp, nfp, 0, 8, 0, 32, buf, size);

	fclose(fp);
	fclose(nfp);

	return;
}

//tree encode
void tencode(char* filename, node* head)
{
	FILE* fp;

	void serialization_tree(node* head, FILE* fp);

	fp = fopen(filename, "wb");

	serialization_tree(head, fp);
	
	fputc(END, fp);
	fclose(fp);

	return;
}

//file encode
void fencode(FILE*    fp,      FILE* nfp,  //file pointers to get(fp) and put byte(nfp) 
			 uint8_t  ibit,    int   iblen,//insert (byte) 8 bit, insert bit free space len 
			 uint32_t rbyte,   int   rblen,//remainder(остаток) 4 byte, remainder byte len 
			 leaf**   buf ,    int   size) //leavies buffer , size leavies buffer
{
	leaf* binsearch(leaf** buf, char target, int size);

	char c;

	leaf*    var;
	int      lcode;
	uint32_t code;
		
	uint32_t mbit = 0;
	
	while(True){	

		if(rblen == 32 && (c = fgetc(fp)) == EOF){
			fputc(c, nfp);

			return;
		}		

		var   = (rblen != 32) ?  NULL : binsearch(buf, c, size);			
		lcode = (rblen != 32) ? rblen : var->lcode; 
		code  = (rblen != 32) ? rbyte : var->code;	
		
		if(lcode < iblen){
		
			ibit |= (code) << (iblen -= lcode);

			rbyte =  0;
			rblen = 32;	
						
		} else if (lcode > iblen){	

			ibit |= (code) >> (lcode - iblen);
										
			lcode -= iblen;		
			for (int i = 0; i < lcode; i++)
				mbit |= 1 << i;

			rbyte = code & mbit;	
			rblen = lcode;	

			fputc(ibit, nfp);
			ibit  = 0;	
			iblen = 8;

		} else {
		
			ibit |= code; 	

			fputc(ibit, nfp);	

			ibit  = 0;
			iblen = 8;

			rbyte =  0;
			rblen = 32;	
		}
	}
}

void serialization_tree(node* head, FILE* fp)
{
	uint8_t data = NODEWP;	
	
	if(head->lp){
		fputc(data | isLEAF, fp);
		fputc(head->lp->letter, fp);		
		return;
	}

	if(head->left)
		data |= LEFT;
	if (head->right)
		data |= RIGHT;	

	fputc(data, fp);

	serialization_tree(head->left,  fp);	
	serialization_tree(head->right, fp);	
}


int compare_alph(leaf* a, leaf* b)
{
	if(a->letter < b->letter)	
		return 1;
	else
		return 0;
}
