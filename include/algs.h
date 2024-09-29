void qsortt(leaf** buf, int left, int right, int (*func)(leaf*, leaf*));
void swap  (leaf** buf, int i, int j);


leaf* binsearch (leaf** buf, char target, int size);
leaf** bininsert(leaf** buf, char let   , int* size);
