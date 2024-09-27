#include "haff.h"


leaf * leaf_init(char let)
{
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
	FILE *fp;
	char c;
	leaf **lbuf;
	int sizebuf, start_bits = 0;
	
	fp = fopen(name, "r");

	lbuf = (leaf**) malloc(sizeof(leaf*));

	if((c = getc(fp)) == EOF){
		printf("\n ERROR: !Not enough data! \n");	
	}

	sizebuf = 1;	
	lbuf = bufalloc(lbuf, &sizebuf);	
	lbuf[0] = leaf_init(c);
	
	putchar('\n');
	while((c = getc(fp)) != EOF){
		start_bits += 8;
		lbuf = bininsert(lbuf, c, &sizebuf);				
	}		
	putchar('\n');

	printf("Start Bits: %i, Bytes: %i", start_bits, start_bits / 8);

	qsortt(lbuf, 0, sizebuf - 1);

	//free(lbuf);
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

leaf** bininsert(leaf** buf, char let, int* size)
{
	int low, high, mid, midres;
	leaf *lp;

	low  = 0;
	high = *size - 1;

	while(low <= high){
		mid  = (low + high)/2; 
		midres = buf[mid]->letter - let;	
		
		if(midres > 0){//go to left
			high = mid - 1;		
		}else if(midres < 0){
			low = mid + 1;//go to right or equal
		}else{
			buf[mid]->count++;
			return buf;		
		}
	}

	lp = leaf_init(let);

	++*size;	
	buf = bufalloc(buf, size);

	for(int i = *size; i > low; i--){
		buf[i] = buf[i - 1];	
	}	
		
	buf[low] = lp;
		
	return buf;	
}

leaf** bufalloc(leaf** buf, int *size)
{
	leaf **pt;	
	
	pt = (leaf**) malloc((*size + 1) * sizeof(leaf*));

	for(int i = 0; i < *size; i++){
		pt[i] = buf[i];
	}	
	
	free(buf);	
	
	return pt;
}


void swapp(leaf** buf, int i, int j)
{
	leaf* var = buf[i];
	buf[i] = buf[j];
	buf[j] = var;
}


void qsortt(leaf** buf, int low, int high)
{
	int last;

	if(low >= high)
		return;

	swapp(buf, (low + high) / 2, low);
	last = low;

	for(int i = low + 1; i <= high; i++){
		if(buf[i]->count < buf[low]->count){
	   		swapp(buf, ++last, i);	
		}
	}
	swapp(buf, low, last);
	qsortt(buf, low, last - 1);
	qsortt(buf, last + 1, high);
}

