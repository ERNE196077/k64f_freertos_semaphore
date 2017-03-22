#include "aux_sem.h"


BufferItem_t BufferItemList[BUFFITEMNUMBER];

int c;
for ( c = 0; c < BUFFITEMNUMBER - 1; c++)
{
	BufferItemList[c].id = c;
	BufferItemList[c].value = 0;
	BufferItemList[c].next = (struct _BufferItem_t_ *)&BufferItemList[c+1];
}

BufferItemList[BUFFITEMNUMBER-1].id = c;
BufferItemList[BUFFITEMNUMBER-1].value = 0;
BufferItemList[BUFFITEMNUMBER-1].next = &BufferItemList[0];