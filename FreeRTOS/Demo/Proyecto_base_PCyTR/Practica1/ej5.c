/*
 * Práctica 1 - Ejercicio 5
 * Funciones para el ejercicio 5 de la práctica 1 (Productor-Consumidor)
 */

#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Referencia externa a la cola compartida */
extern QueueHandle_t xQueue;

/* Tarea productora */
void prvTask1_5_1(void * pvParameters)
{
    int valor;
    const TickType_t xDelay = pdMS_TO_TICKS(5000); // 5 segundos
    
    for( ;; )
    {
        /* Generar número aleatorio */
        valor = (int)random();
        
        /* Imprimir el valor generado */
        printf("Task1 (enviado): %d\n", valor);
        
        /* Enviar el valor a la cola */
        xQueueSend(xQueue, &valor, portMAX_DELAY);
        
        /* Suspender la tarea durante 5 segundos */
        vTaskDelay(xDelay);
    }
}

/* Tarea consumidora */
void prvTask1_5_2(void * pvParameters)
{
    int valor_recibido;
    
    for( ;; )
    {
        /* Recibir valor de la cola */
        if (xQueueReceive(xQueue, &valor_recibido, portMAX_DELAY) == pdPASS)
        {
            /* Imprimir el valor recibido */
            printf("Task2 (recibido): %d\n", valor_recibido);
        }
    }
}