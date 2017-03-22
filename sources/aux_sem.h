#ifndef _AUX_SEM_H_
#define _AUX_SEM_H_

#include "semphr.h"


#define BUFFITEMNUMBER 10

typedef struct _BufferItem_t_{
	uint8_t id;
	uint8_t value;
	SemaphoreHandle_t semphr;
	struct _BufferItem_t_ *next;
}BufferItem_t;


#endif