/*
 * Práctica 1 - Ejercicio 2
 * Funciones para el ejercicio 2 de la práctica 1
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Tarea 1 del ejercicio 2 - Tarea periódica de 10s */
void prvTask1_2_1(void * pvParameters)
{
    const TickType_t xDelay = 10000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    long long int i = 0;

    for( ;; )
    {
        xTaskDelayUntil( &xLastWakeTime, xDelay );
        printf("Task1: Iteracion %lld\n", i);
        i++;
    }
}

/* Tarea 2 del ejercicio 2 - Tarea periódica de 1s */
void prvTask1_2_2(void * pvParameters)
{
    const TickType_t xDelay = 1000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    long long int i = 0;

    for( ;; )
    {
        xTaskDelayUntil( &xLastWakeTime, xDelay );
        printf("Task2: Iteracion %lld\n", i);
        i++;
    }
}