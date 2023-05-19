#include "lock.h"

int xdata j = 0;

void lock_init(Lock* lock){

	lock->mode = 0;
	lock->length = 4;
	lock->allow_length = 60;
	for(j = 0; j < 4; j++){
		lock->password[j] = '0' + j+1;
	}

}
