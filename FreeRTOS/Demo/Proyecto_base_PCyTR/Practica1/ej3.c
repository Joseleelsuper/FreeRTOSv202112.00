/*
 * Práctica 1 - Ejercicio 3
 * Funciones para el ejercicio 3 de la práctica 1 (IdleTask)
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Variable externa para controlar impresiones del IdleTask */
extern BaseType_t xPrinted;

/* Función de tarea que permite la ejecución del IdleTask */
void prvTask1_3(void * pvParameters)
{
    const TickType_t xDelay = 2000;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    long long int i = 0;

    for( ;; )
    {
        // Primero nos bloqueamos para dar oportunidad a la IdleTask
        vTaskDelayUntil( &xLastWakeTime, xDelay );
        
        // Realizamos el trabajo de nuestra tarea
        printf("Iteracion %lld\n", i);
        i++;
        
        // Reseteamos la bandera para permitir que la IdleTask pueda imprimir 
        xPrinted = pdFALSE;
        
        // Pequeña pausa para permitir que la IdleTask se ejecute
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}