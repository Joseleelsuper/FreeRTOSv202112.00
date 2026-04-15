/*
 * Práctica 1 - Ejercicio 4
 * Funciones para el ejercicio 4 de la práctica 1 (Cambio de prioridades)
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Tarea 1 que cambia su prioridad en la iteración 5 */
void prvTask1_4_1(void * pvParameters)
{
    long long int i = 0;
    TaskHandle_t xTask1Handle = xTaskGetCurrentTaskHandle();
    TickType_t xLastWakeTime;

    for( ;; )
    {
        xLastWakeTime = xTaskGetTickCount();
        
        /* Bucle que consume tiempo de CPU */
        printf("Task1: iniciando iteración %lld\n", i);
        vTaskDelay(pdMS_TO_TICKS(8000));
        
        printf("Task1 acaba de finalizar la iteración %lld\n", i);
        
        /* Cambiar la prioridad a 4 a partir de la iteración 5 */
        if(i == 4) {
            printf("Task1: cambiando prioridad a 4\n");
            vTaskPrioritySet(xTask1Handle, 4);
        }
        
        i++;
        
        /* Bloqueamos la tarea durante 200ms */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(200));
    }
}

/* Tarea 2 periódica que se ejecuta cada 2 segundos */
void prvTask1_4_2(void * pvParameters)
{
    long long int i = 0;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for( ;; )
    {
        printf("Task2: iteración %lld\n", i);
        i++;
        
        /* Bloqueamos la tarea durante 2 segundos */
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
    }
}