#include "pstack.h"

/**

* @breif Initialize the Page Stack

* @Author CheasonY

*/
void pstack_init(pstack* ps){

	ps->top = 0;
	ps->pages[0] = 0;

}

/**

* @breif Push A PAGE ENTRY into the Page Stack

* @Author CheasonY

*/
void pstack_push(pstack* ps, unsigned char page){
	
	ps->top += (ps->top==19)?0:1;
	ps->pages[ps->top] = page;

}

/**

* @breif Pop the top-page from the Page Stack

* @Author CheasonY

*/
void pstack_pop(pstack* ps){

	if(ps->top!=0){
		ps->top -= 1;
	}

}