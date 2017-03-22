

#include "aux_sem.h"



BufferItem_t BufferItemList[BUFFITEMNUMBER];

void BufferItemListInit(void){
	int j;
	for( j = 0 ; j < BUFFITEMNUMBER - 1 ; j++ ){
		BufferItemList[j].id = j;
		BufferItemList[j].value = 0;
		BufferItemList[j].semphr = xSemaphoreCreateBinary();
		BufferItemList[j].next = &BufferItemList[j+1];
	}
	BufferItemList[BUFFITEMNUMBER - 1].id = BUFFITEMNUMBER - 1;
	BufferItemList[BUFFITEMNUMBER - 1].value = 0;
	BufferItemList[BUFFITEMNUMBER - 1].semphr = xSemaphoreCreateBinary();
	BufferItemList[BUFFITEMNUMBER - 1].next = &BufferItemList[0];
}
