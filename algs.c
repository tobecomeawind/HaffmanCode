#include "haff.h"
#include "tree.h"
#include "algs.h"

void qsortt(leaf** buf, int left, int right, int (*func)(leaf* a, leaf* b))
{
	void swap(leaf** buf, int i, int j);	
	int pivot;

	if(left >= right)
		return;

	swap(buf, left, (left + right) / 2);
	pivot = left;	

	for(int i = left + 1; i <= right; i++){
		if(func(buf[i], buf[left])){
			swap(buf, ++pivot, i);
		}	
	}

	swap(buf, left, pivot);
	qsortt(buf, left, pivot - 1,  func);
	qsortt(buf, pivot + 1, right, func);
}

void swap(leaf** buf, int i, int j)
{
	leaf* var = buf[i];
	buf[i]    = buf[j];
    buf[j]    = var;	
}


leaf* binsearch(leaf** buf, char target, int size)
{
	int low, high, mid;

	low  = 0;
	high = size - 1;

	while(low <= high){
		mid    = (low + high) / 2; 

		if (target > buf[mid]->letter)
			low  = mid + 1;
		else if (target < buf[mid]->letter)
			high = mid - 1;	
		else
			return buf[mid];
	}
	
	return 0;
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
		
		if(midres > 0){
			high = mid - 1;		
		}else if(midres < 0){
			low = mid + 1;
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
