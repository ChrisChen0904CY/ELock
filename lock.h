#ifndef _LOCK_H_
#define _LOCK_H_
#include <STC15F2K60S2.H>

// Key Num
extern unsigned char key_num;

// Long Press
extern bit Long_Press;
extern bit Long_Shift;

// Whether the lock has been locked
extern bit locked;

/**

* @breif Lock Struct

* @Author CheasonY

* @member mode: Specify the current mode, more specifically,
*		  0: refers to the default mode ---- Normal Mode
*		  It means that you need and only need to input a series of chars that have a same length to the password

*		  1: refers to Virtual-Bits Mode
*		  You can unlock our product when the password do exist as a subffix of your input

--------------------------------------Instance Here--------------------------------------------------------------
*		  For Mode 1, here was an instance:                                                                     *
*		  Suppose our password was: 1234																		*
*		  Then you can unlock the device through inputs like:													*
*		  00007246871234																						*
*		  			^^^^																						*
*		  0006871234																							*
*		  	    ^^^^																							*
*		  But you can't unlock like this:																		*
*		  00012340																								*
*			 ^^^^#(End up with an additional char)																*
------------------------------------------------------------------------------------------------------------------

*		  2: refers to Descrete-Splinter-Combination Mode
*		  You can unlock our product when the password can be combined up based on your input
*		  !Caution! You can't put two entry of the password too far away from each other
*					More Specifically, the limited distance WAS NOT ALLOWED TO MORE THAN 2
*					It also NEED END UP WITH THE RAIL OF YOUR PASSWORD

--------------------------------------Instance Here--------------------------------------------------------------
*		  For Mode 2, here was an instance:                                                                     *
*		  Suppose our password was: 1234																		*
*		  Then you can unlock the device through inputs like:													*
*		  00007246871265384																						*
*		  			^^	^ ^																					    *
*		  0006871112034																							*
*		  	    ^  ^ ^^																							*
*		  But you can't unlock like this:																		*
*		  0001242340																							*
*			 ^^  ^^#(End up with an additional char)															*
*		  0081555253664											     											*
*			 ^\\\^ ^  ^(Too Far!!!)																				*
------------------------------------------------------------------------------------------------------------------

* @member length: Specify the length of the password

* @member password: Store the password and defaulted by "1234"

* @member input_stream: Your input-characters stream

*/
typedef struct Lock{

	unsigned char mode;		/* Specify the current mode */
	unsigned char length;   /* Specify the length of the password */
	unsigned char allow_length;
	char password[16];		/* Store the password here */
	char input_stream[60];  /* Dynamic Input Stream */
	
}Lock;

void lock_init(Lock*);

#endif
