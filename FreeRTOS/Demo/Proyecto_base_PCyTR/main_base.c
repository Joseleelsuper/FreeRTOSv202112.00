/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/******************************************************************************
 * NOTE 1: The FreeRTOS demo threads will not be running continuously, so
 * do not expect to get real time behaviour from the FreeRTOS Linux port, or
 * this demo application.  Also, the timing information in the FreeRTOS+Trace
 * logs have no meaningful units.  See the documentation page for the Linux
 * port for further information:
 * https://freertos.org/FreeRTOS-simulator-for-Linux.html
 *
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, are defined
 * in main.c.
 ******************************************************************************
 *
 * main_base() creates two tasks.  It then starts the scheduler.
 *
 *
 * NOTE:  Console input and output relies on Linux system calls, which can
 * interfere with the execution of the FreeRTOS Linux port. This demo only
 * uses Linux system call occasionally. Heavier use of Linux system calls
 * may crash the port.
 * 
 * Ejecutar con:
 * make PROFILE=1
 * make profile
 * ./build/posix_demo
 */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>  // Para random() y srandom()

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

/* Includes para las prácticas */
#include "Practica1/practica1.h"
#include "Practica2/practica2.h"

/* Priorities at which the tasks are created. */
#define TASK1_PRIORITY    ( tskIDLE_PRIORITY + 1 )
#define TASK2_PRIORITY       ( tskIDLE_PRIORITY + 2 )
#define EDF_SCHEDULER_PRIORITY ( configMAX_PRIORITIES - 1 ) /* Prioridad máxima para el planificador EDF */

/* The rate at which data is sent to the queue.  The times are converted from
 * milliseconds to ticks using the pdMS_TO_TICKS() macro. */
#define FREQUENCY_MS_TASK1         pdMS_TO_TICKS( 2000UL )
#define FREQUENCY_MS_TASK2         pdMS_TO_TICKS( 10000UL )


/*-----------------------------------------------------------*/

/* Variable global para la cola compartida entre tareas */
QueueHandle_t xQueue = NULL;

/* Variable para el IdleTask */
BaseType_t xPrinted = pdFALSE;

/* Semáforos para las tareas de la práctica 2 */
SemaphoreHandle_t xSemaphoreT1T3 = NULL;
SemaphoreHandle_t xSemaphoreT3T2 = NULL;

/* Variables para la práctica 2, ejercicio 3 */
int Modify2_3_1 = 0;
char *Modify2_3_2 = NULL;

/* Declaración de la función hook del IdleTask */
static void vApplicationIdleHook( void );

/*-----------------------------------------------------------*/

/* Hook function para IdleTask */
void vApplicationIdleHook( void )
{
    if (xPrinted == pdFALSE) {
        printf("Ejecutando IdleTask...\n");
        xPrinted = pdTRUE;
    }
}

/*** SEE THE COMMENTS AT THE TOP OF THIS FILE ***/
void main_base( void )
{
    funcs_p1();
    funcs_p2();

    vTaskStartScheduler();
}

void funcs_p1() {
    /* PRACTICA 1 */
    xTaskCreate( prvTask1_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );

    // xTaskCreate( prvTask1_2_1, "Task1", configMINIMAL_STACK_SIZE, NULL, 5, NULL );
    // xTaskCreate( prvTask1_2_2, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, NULL );

    // xTaskCreate( prvTask1_3, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );

    // xTaskCreate( prvTask1_4_1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
    // xTaskCreate( prvTask1_4_2, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
    
    // xQueue = xQueueCreate(10, sizeof(int));
    // if (xQueue == NULL) {
    //     printf("Error: No se pudo crear la cola\n");
    // }
    // srandom(xTaskGetTickCount());
    // xTaskCreate( prvTask1_5_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    // xTaskCreate( prvTask1_5_2, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK2_PRIORITY, NULL );
}

void funcs_p2() {
    /* PRACTICA 2 */
    // Traza 1: https://secure.eu.internxt.com/sh/file/WfA_jUtwQdOFSpS8a3CfWA/x6MvdPPk
    // xTaskCreate( prvOneTimeTask2_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    // xTaskCreate( prvSporadicTask2_1, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    
    // Traza 2 con configUSE_PREEMPTION a 0: https://secure.eu.internxt.com/sh/file/d3BExcl0Qie61JBijaIIiA/4qdPCQYX
    // Traza 2 con configUSE_PREEMPTION a 1: https://secure.eu.internxt.com/sh/file/Ml4b54wlT2mkFXUxxkHADQ/_7RTZ4hh
    // xSemaphoreT1T3 = xSemaphoreCreateBinary();
    // xSemaphoreT3T2 = xSemaphoreCreateBinary();
    // xTaskCreate( prvTask2_2_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    // xTaskCreate( prvTask2_2_2, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    // xTaskCreate( prvTask2_2_3, "Task3", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );

    // xTaskCreate( prvTask2_3_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    // xTaskCreate( prvTask2_3_2, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
    
    // srandom(33); 
    // xTaskCreate(prvShortDeadlineTask2_6, "ShortDL", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
    // xTaskCreate(prvMediumDeadlineTask2_6, "MediumDL", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
    // xTaskCreate(prvLongDeadlineTask2_6, "LongDL", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
    // xTaskCreate(vEDFSchedulerTask2_6, "EDFScheduler", configMINIMAL_STACK_SIZE * 2, NULL, EDF_SCHEDULER_PRIORITY, NULL);
    
    // Traza 7: https://secure.ue.internxt.com/sh/file/1GdbaRtNQh-oA6alHy7v_g/C60Cl72e
    // vStartPriorityInheritanceTasks();
}