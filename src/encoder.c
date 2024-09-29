#include "analyzer.h"
#include "encoder.h"
#include "algs.h"


node* init_node(leaf* lp, node* left, node* right)
{	
 	/****************************\
   |   Constructor node of tree   |
 	\****************************/
	
	node* nptr;

	nptr = (node*) malloc(sizeof(node));
	
	nptr->lp    = lp; //leaf pointer
		
	//if leaf pointer null(if we in node), we check left and right nodes(args) and adding them
	//if leaf pointer     (if we in leaf), we get count of leaf from buf
	nptr->count = (lp) ? lp->count : ((left) ? left->count : 0) + ((right) ? right->count : 0);	
	
	nptr->left  = left;
	nptr->right = right;

	return nptr;	
}

void destruct_tree(node* head)
{
			
 	/****************************\
   |  Freing Tree alloced memory  |
 	\****************************/
	if(head->lp){
		free(head->lp);	// free a leaf alloced memory
		free(head);     // free a leaf node
		return;
	}
		
	//typical recursive run by tree
	//from bottom to top   
	//from left   to right	
	destruct_tree(head->left);
	destruct_tree(head->right);

	free(head);  // if current head == node
}


node* create_tree(leaf** buf, int size)
{

 	/****************************\
   |   Creating a Haffman tree    |
   |        without codes         |
   | from bottom(leavies) to top  |
 	\****************************/

	node* rhead;// right head
	node* lhead;// left  head
	FILE *fp;

	lhead = init_node(buf[0], NULL, NULL);

	if (size > 2)
		rhead = init_node(buf[1], NULL, NULL);
	else
		return NULL;// error not enough values
					// i am lazy and if letters < 2
					// encoding not working	
					// 
					// or if letters < 2 
					// no point cuz start size < finish size
					// check the coefs(tree needs min 7 bytes)
	
	// start by 2 cuz lhead and rhead already exists
	//
	//How function works
	//letter already sorted by count in text
	//
	//l - lhead
	//r - rhead
	//i - current letter
	//
	//we check if (l + r) <= (r + i) 
	//then l = l + r
	//	   r = i
	//and we adding count, because how it haffman encoding work
	//
	//l(1)   r(1)  |i(1)   2     3     5   6 | leavies 
	//	 \    /    *----*                    |  
	//    l(2)     r(1) |i(2)    3     5   6 | 
	//       \      /   *-----*              | 
	//         l(2)      r(2) |i(3)    5   6 | 
	//            \      /    *-----*        |            
	//              l(4)       r(3) |i(5)  6 | 
	//                 \      /     *--------*  else working (l(7) + r(5) = 12) !< (r(5) + i(6) = 11) => r = r + i
	//                   l(7)      r(5)  i(6)  
	//              	    \        \    /
	//              	     \        r(11)    
	//              	      \        /
	//              	       \	  /
	//              	     	 l(18)         
	//                            root 
	//                        
	

	for(int i = 2; i < size; i++){
		if((lhead->count) <= (rhead->count)){
			lhead = init_node(NULL, lhead, rhead);
			rhead = init_node(buf[i], NULL, NULL);
		}else{
			rhead = init_node(NULL, rhead, init_node(buf[i], NULL, NULL));	
		}	
	}		


	// the top, the first node(root) 
	// not difference between lhead and rhead if they become a top
	lhead = init_node(NULL, lhead, rhead);

	return lhead;
}

void create_table(node* head, int len, uint32_t code)
{			
 	/****************************\
   |   Creating a Haffman table   |
   |              ||              |
   |    creating haffman codes    |
   |    from root to every leaf   |
   |           in tree            |
 	\****************************/
	
	//leaf = struct node where attr lp(leaf pointer) not null
	//
	//00000  00001
	//leaf   leaf
	//	0\    / 1  0001               
	//    node     leaf  
	//      0\      / 1  001
	//         node      leaf  
	//           0\      / 1    01         
	//              node       leaf 
	//                0\      / 1   10    11
	//                   node      leaf  leaf 
	//              	    \      0 \    / 1
	//              	     \        node   
	//              	      \        /
	//              	     0 \	  / 1
	//              	     	 root       
	//
	//all cods are unique and not prefix 
	if(head->lp){
		//stop point if node is leaf
		head->lp->code  = code;
		head->lp->lcode = len;
		return;
	}
	
	//we need to write codes in format from root 
	//not from leavies
	//
	//thats why in anything step right or left we shif the byte to left
	//and if 
	//we go to right and add a 1
	//code is correct and if we go left->left->right->left->right 
	//we get                         0    0      1      0     1
	code <<= 1;

	create_table(head->left, ++len, code);	
	code |= 1;
	
	create_table(head->right, len, code);
}


void encode(char* filename, node* head, leaf** buf, int size)
{	
 	/****************************\
   |        Encoding file         |
   |         with using           |
   |        haffman tree          |
 	\****************************/

	//file encode	
	void fencode(FILE* fp, FILE* nfp,         
				 leaf** buf, int size);
	//tree encode	
	void tencode(char * filename, node* head);
	FILE* fp;  //    file pointer(input  file)
	FILE* nfp; //new file pointer(output file)

	//pointer to compare func need to qsort(sorting by alphabet(ascii codes))
	int (*func)(leaf* ,leaf*) = &compare_alph;

	qsortt(buf, 0 , size - 1, func);	
	
	//writing a haffman tree in file
	tencode("result.txt", head);

	fp  = fopen(filename,      "rb");
	nfp = fopen("result.txt", "awb");	//go to end, because tree already encoded in file
										//and if just open in "wb" tree will broken
	//writing a main encoded data in file	
	fencode(fp, nfp, buf, size);

	fclose(fp);
	fclose(nfp);

	return;
}

//tree encode
void tencode(char* filename, node* head)
{
					
 	/****************************\
   |        Encoding tree         |
 	\****************************/	
	
	FILE* fp;	
	void serialization_tree(node* head, FILE* fp);

	fp = fopen(filename, "wb"); // output file opening

	// writing tree structure in binary view
	serialization_tree(head, fp);
	
	fputc(END, fp); // putting a stop char(see in tree.h)
					// END = 0b00000000
					// 
					// when desiarilazioning tree 
					// we need a stop point to recursive alg
	fclose(fp);

	return;
}

void serialization_tree(node* head, FILE* fp)
{
			
 	/*****************************\
   |      Serialization Tree       |
   |              ||               |
   |    writing a tree structure   |
   |        in binary view         |
   |            to file            |
 	\*****************************/	

	/**********************************
	***********************************
	******* See the header file,******* 
	*******  before you reading *******
	*******     a function      *******
	***********************************
	**********************************/
	
	uint8_t data = NODEWP; // node in binary view without a params
	
	if(head->lp){
		//stop point 
		//if we in node with leaf
		//we write to file two bytes
		// 1. Byte says about the next byte is letter
		// 2. Byte keep a letter of leaf
		//
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

	// How it actually work
	//
	// in every recursive iteration we put a byte
	// byte include info 3 types
	//  1. If node->left  != NULL
	//  2. If node->right != NULL
	//  3. If node->leaf  != NULL
	//
	//  Basic byte is NODEWP, without any data
	//  if current node have a left or right children 
	//  we writing a byte like this
	//  	
	//  1. Have a left           - 0b11111010
	//  2. Have a right          - 0b11111001
	//  3. Have a left and right - 0b11111011
	// 
	//  If we in stop point
	//  			 ||
	//  		  head->lp
	//  we just adding a byte isLeaf
	//  like 0b1111100
	//  and after writing a letter data
	//
	//
	//  ***********************************
	//	***********************************
	//  **** Byte with all 'ON' params **** 
	//  ****     like 0b11111111       ****
	//  ****  not exist in every case  ****
	//  ***********************************
	//  ***********************************
	//
	//  Basic binary view of tree
	//   
	//    root    node(1)     a
	//  11111011 11111011 11111100 ascii(a)
	//      b                 c
	//  11111100 ascii(b) 11111100 ascii(c)
	//	
	//	a      b       c
	//    \  /        /
	//   node(1)     /
	//   	 \      /
	//   	   root
	//
	//
	//  codding by go to left
	//
	//
}



//file encode
void fencode(FILE*    fp,      FILE* nfp,  //file pointers to get(fp) and put byte(nfp) 
			 leaf**   buf ,    int   size) //leavies buffer , size leavies buffer
{
			
 	/*****************************\
   |        File encoding          |
 	\*****************************/	

	/******************************************************
	 ********!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*******
	 *******!! This function only working with     !!****** 
	 *******!!  leavies where lentgh of code <= 32 !!******
	 *******!!    in a negative case all file      !!******
	 *******!!              broke                  !!******
	 ********!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!******* 
	 ******************************************************/

	uint8_t  ibit  =  0; // insert byte
	int      iblen =  8; // Free space len insert byte
	
	uint32_t rbyte =  0; // remainder 4 bytes
	int      rblen = 32; // Free space len remainder bytes

	leaf* binsearch(leaf** buf, char target, int size);

	char c;

	leaf*    var;   // variable
	int      lcode; // lentgh code
	uint32_t code;  // code
		
	uint32_t mbit = 0; //multiply bit
	
	while(True){	
		
		//stop point
		if(rblen == 32 && (c = fgetc(fp)) == EOF){ // if remainder bytes is 0 
												   // and if we in the end	
			fputc(c, nfp); //put the EOF in file

			return;
		}		
		
		// rblen cheking
		// if remainder bytes not 0
		// current bytes = remainder bytes
		// else 
		// current bytes = code from current leavies
		var   = (rblen != 32) ?  NULL : binsearch(buf, c, size);			
		lcode = (rblen != 32) ? rblen : var->lcode; 
		code  = (rblen != 32) ? rbyte : var->code;	
		
		if(lcode < iblen){
			// if free space in byte
			// we just adding a value
	
			ibit |= (code) << (iblen -= lcode);

			rbyte =  0;
			rblen = 32;	
						
		} else if (lcode > iblen){	

			ibit |= (code) >> (lcode - iblen);
										
			lcode -= iblen;		
			for (int i = 0; i < lcode; i++) // creating a multiply bit
				mbit |= 1 << i;

			rbyte = code & mbit; // resetting values that already inserted 
								 //  101 01010101
								 // we insert 5 bits	
								 // mbit = 000 00111111
								 // 101 01010101 & 000 0011111
								 //              ||
								 //         000 00010101
								 //
			rblen = lcode;	

			fputc(ibit, nfp);
			ibit  = 0;	
			iblen = 8;

		} else { // if lcode == 0 , no free space == byte is full
				 // and we need put him
				 // and refresh values
		
			ibit |= code; 	

			fputc(ibit, nfp);	

			ibit  = 0;
			iblen = 8;

			rbyte =  0;
			rblen = 32;	
		}
	}


	// How it actually works
	//
	// imagine a haffman table
	//
	// a  |  00
	// b  |  01
	// g  |  111 01010101
	// _________________________
	// ||||the message: abg||||
	//
	// ibit  =  0;
	// iblen =  8;
	// rbyte =  0; // we may call this buffer 
	// rblen = 32; 
	// 
	// lcode = current_letter->lcode
	// code  = current_letter->code
	//
	// we have 3 variants:
	// 	
	// 	1. lcode  <  iblen 
	//  2. lcode  >  iblen
	//  3. lcode  == iblen 
	//__________________________________
	// The first letter in our message is 'a'
	//
	// code  = 00;
	// lcode =  2;
	// we in the first variant
	// lcode < iblen
	// 
	// ibit |= code << (iblen -= lcode) 
	// 
	// iblen - lcode = 6
	// we shift our 00 to six right
	//        a  free space
	// ibit = 00 000000
	//_________________________
	// Second later "b" variant
	// code  = 01
	// lcode = 2
	//
	// we in the first variant
	//
	//shift 
	//iblen - lcode = 4 
	//
	//        a  b  free
	// ibit = 00 01 0000
	//
	//_______________________
	// Third later "g"
	// code  = 111 01010101
	// lcode = 11
	//
	// we in the second variant
	//
	// we need to push a 4 bits of this code
	// because iblen = 4 = free space in byte
	// 
	// we need shift a such number that 4 bits last
	// lcode - iblen = 7
	// shift by 7 
	// 000 0000 1110
	// and we putting this in ibit
	//
	//        a  b  g(4)
	// ibit = 00 01 1110
	//
	// we write ibit in file
	//
	//than we creating our buffer(rbyte)
	//with mbit = multiply bit
	//we need delete the remainder
	//
	//
	//mbit  =  000 0  1111111
	//rbyte &= mbit 
	//
	//        trash  buffer(remainder)
	//rbyte = 000 0  1010101
	//rblen = 7(remainder size)
	//___________________________
	//
	// In this iterarion we need to put
	// a remainder in file
	//
	// the first variant working
	//
	// __________________________
	//
	// Our file
	//
	// ---tree bytes---0000000
	// a  b  g(4) g(7)        EOF
	// 00 01 1110-1010101 0 11111111
}


int compare_alph(leaf* a, leaf* b)
{
 	/*****************************\
   |     Basic compare function    |
   |         by alphabet           |
 	\*****************************/	
	if(a->letter < b->letter)	
		return 1;//true
	else
		return 0;//false
}
