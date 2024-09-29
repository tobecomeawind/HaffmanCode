#include "analyzer.h"
#include "algs.h"

leaf* leaf_init(char let)
{
	 /**********************\
	|Constructor of structure|
	|         leaf           |
	 \**********************/	
	
	leaf *p;

	p = malloc(sizeof(leaf));
	p->letter = let;	
	p->count  = 1;
	p->lcode  = 0;
	p->code   = 0;

	return p;
}

leaf_buf* analyze_file(char name[])
{		
	 /**********************\
	|    Analyzer of file    |
	 \**********************/	
	
	FILE *fp;
	char c;
	leaf **lbuf; //leavies buf
	int sizebuf = 1;

	//sorting by count letter in text
	int (*func)(leaf* , leaf*) = &compare_count;

	fp = fopen(name, "r");

	if(!fp) // input file not opening
		return NULL;

	lbuf = (leaf**) malloc(sizeof(leaf*));
	
	lbuf = bufalloc(lbuf, &sizebuf);	
	lbuf[0] = leaf_init(c);
	
	while((c = getc(fp)) != EOF){
		lbuf = bininsert(lbuf, c, &sizebuf);// searching by alphabet
											// and insert
	}		

	qsortt(lbuf, 0, sizebuf - 1, func); // sort by count

	fclose(fp);

	leaf_buf* ilbuf = (leaf_buf*) malloc(sizeof(leaf_buf*));
	ilbuf->size = sizebuf;
	ilbuf->buf  = lbuf;

	return ilbuf;
}

void print_table(leaf** leavies, int sizebuf)
{		
	leaf* var;	
	
	putchar('\n');
	printf("\n______________________________________________________________________________\n");
	printf("| N || ASCII | Count | Letter |             Code               | Lenght || N |\n");
	printf("|---||-------|-------|--------|--------------------------------|--------||---|\n");
	for(int i = 0; i < sizebuf; i++){
		var = leavies[i];			
		printf("|%3i||  %4i | %5i |   %4c |%32.*b|  %5i ||%3i|\n",
				i + 1,	
				var->letter,
				var->count,	
				(var->letter > 32 && var->letter < 120) ? var->letter : 78,  
				var->lcode % 8,
				var->code,
				var->lcode,
				i + 1);
	}	
	printf("------------------------------------------------------------------------------\n");
	putchar('\n');
}


leaf** bufalloc(leaf** buf, int *size)
{	
	 /**********************\
	|    Reallocator bufs    |
	 \**********************/	
	
	leaf **pt;	
	
	pt = (leaf**) malloc((*size + 1) * sizeof(leaf*));

	for(int i = 0; i < *size; i++){
		pt[i] = buf[i];
	}	
	
	free(buf);	
	
	return pt;
}

int compare_count(leaf* a, leaf* b)
{	
	 /**********************\
	|    Compare by count    |
	 \**********************/	
	if(a->count < b->count)	
		return 1;
	else
		return 0;
}
