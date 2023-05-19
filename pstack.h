# ifndef __PSTACK_H__
# define __PSTACK_H__

/**
---------------------------------
|    MODE PAGE INDEX TABEL      |
--------------------------------
|UNLOCKED              |      0 |
--------------------------------
|LOCKED                |      1 |
--------------------------------
|SETTINGS              |      2 |
--------------------------------
|PASSWORD CHANGE       |      3 |
--------------------------------
|PASSWORD MATCH MODE   |      4 |
--------------------------------
|PASSWORD CONTEXT MODE |      5 |
--------------------------------
|TIME CHECK            |      6 |
--------------------------------
*/

/**

* @breif Page Stack

* @Author CheasonY

* @member top: An Un-Negtive Integer which specify current top index

* @member pages[20]: The Storation Array of Pages

*/
typedef struct pstack{

	unsigned char top;
	unsigned char pages[20];
	
}pstack;

void pstack_init(pstack*);
void pstack_push(pstack*, unsigned char);
void pstack_pop(pstack*);

#endif
