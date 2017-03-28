/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "aux_sem.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define PRODUCER_DELAY	500
#define CONSUMER_DELAY 	200
#define PRODUCER_START	4
#define CONSUMER_START	0
#define TASK_PRIO (configMAX_PRIORITIES - 1)
#define CONSUMER_LINE_SIZE 3
SemaphoreHandle_t *xSemaphore_producer;
SemaphoreHandle_t *xSemaphore_consumer;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void producer_task(void *pvParameters);
static void consumer_task(void *pvParameters);

extern BufferItem_t BufferItemList[BUFFITEMNUMBER];
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    BufferItemListInit();
    xTaskCreate(producer_task, "PRODUCER_TASK", configMINIMAL_STACK_SIZE, NULL, TASK_PRIO, NULL);
    xTaskCreate(consumer_task, "CONSUMER_TASK", configMINIMAL_STACK_SIZE, NULL, TASK_PRIO, NULL);

    PRINTF("TASKS created.\r\n");
    /* Start scheduling. */
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task producer_task.
 */
static void producer_task(void *pvParameters)
{

	BufferItem_t *producer_item = (BufferItem_t *)&(BufferItemList[PRODUCER_START]);

    while (1)
    {


        if (xSemaphoreTake(producer_item->semphr, portMAX_DELAY) == pdTRUE)
        {
        	if(producer_item->value == 0){
        		producer_item->value = 1;
        		PRINTF("Producer Took item: %d :: Value: %d\r\n", producer_item->id, producer_item->value);
        		int p = 0;
        		while(p < PRODUCER_DELAY )
        			p++;
        	}else{
        		SemaphoreHandle_t *paux_item = &(producer_item->semphr);
        		producer_item = (BufferItem_t *)producer_item->next;
        		PRINTF("Producer RELEASED Item: %d\r\n",producer_item	->id);
        		xSemaphoreGive(*paux_item);

        	}


        }else{
        	PRINTF("Producer Waiting for item: %d\r\n",producer_item->id);
        }

    }
}

/*!
 * @brief Task consumer_task.
 */
static void consumer_task(void *pvParameters)
{
	BufferItem_t *consumer_item = (BufferItem_t *)&(BufferItemList[CONSUMER_START]);

    while (1)
    {
        if (xSemaphoreTake(consumer_item->semphr, portMAX_DELAY) == pdTRUE)
        {
        	if(consumer_item->value == 1){
        		consumer_item->value = 0;
        		PRINTF("Consumer Took item: %d :: Value: %d\r\n", consumer_item->id, consumer_item->value);
        		int c = 0;
        		while(c < CONSUMER_DELAY )
        			c++;
        	}else{
        		SemaphoreHandle_t *caux_item = &(consumer_item->semphr);
        		consumer_item = (BufferItem_t *)consumer_item->next;
        		PRINTF("Consumer RELEASED Item: %d\r\n",consumer_item->id);
        		xSemaphoreGive(*caux_item);

        	}
        }else{
        	PRINTF("Consumer Waiting for Item: %d\r\n",consumer_item->id);
        }
    }
}
