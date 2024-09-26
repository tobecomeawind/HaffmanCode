#include <stdio.h>
#include "haff.h"

//extern leaf* leaf_init(char);

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
	
	lbuf = (leaf**) malloc(sizeof(leaf*));//lbuf указатель на массив указателей leaf	
	
	fp = fopen(name, "r");

	if((c = getc(fp)) != EOF){
		lbuf[0] = leaf_init(c);
		sizebuf = 1;	
		lbuf = bufalloc(lbuf, &sizebuf);
	}

	putchar('\n');
	while((c = getc(fp)) != EOF){
		start_bits += 8;
	
		//printf(" %b ", c);

		lbuf = bininsert(lbuf, c, &sizebuf);				
	}		
	putchar('\n');

	printf("Start Bits: %i", start_bits);

	qsortt(lbuf, 0, sizebuf - 1);

	//free(lbuf);
	fclose(fp);

	leaf_buf* ilbuf = (leaf_buf*) malloc(sizeof(leaf_buf*));//info leaf buf - ilbuf 
	ilbuf->size = sizebuf;
	ilbuf->buf  = lbuf;

	return ilbuf;
}

void print_leavies(leaf** leavies, int sizebuf)
{	
	putchar('\n');
	for(int i = 0; i < sizebuf; i++)
		printf("|  %3i || ASCII: %4i | Letter: %4c | Count: %5i | Code: %32.*b| CodeD: %10d | Lentgh: %5i || %3i  |\n",
				i + 1,	
				leavies[i]->letter,	
				(leavies[i]->letter > 32 && leavies[i]->letter < 120) ? leavies[i]->letter : 78,  
				leavies[i]->count,
				leavies[i]->lcode % 8,
				leavies[i]->code,
				leavies[i]->code,
				leavies[i]->lcode,
				i + 1);
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

	//buf = (leaf**) realloc(buf, (++*size) * sizeof(leaf*));
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
		//free(buf[i]);
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

