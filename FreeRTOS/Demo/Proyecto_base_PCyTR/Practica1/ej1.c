/*
 * Práctica 1 - Ejercicio 1
 * Funciones para el ejercicio 1 de la práctica 1
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Función prvTask1_1 - Tarea básica periódica */
void prvTask1_1(void * pvParameters)
{
    const TickType_t xDelay = 1000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    long long int i = 0;

    for( ;; )
    {
        xTaskDelayUntil( &xLastWakeTime, xDelay );
        printf("Iteracion %lld\n", i);
        i++;
    }
}