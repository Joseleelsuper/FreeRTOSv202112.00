/*
 * Práctica 2 - Ejercicio 2
 * Dependencias entre tareas mediante semáforos
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Referencia externa a los semáforos compartidos */
extern SemaphoreHandle_t xSemaphoreT1T3;
extern SemaphoreHandle_t xSemaphoreT3T2;

/*
 * Tarea 1 - Tarea periódica que cede el semáforo a T3
 */
void prvTask2_2_1(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(6000);
    int iteracion = 0;
    
    for (;;)
    {
        // T1 realiza su trabajo
        printf("T1: Iniciando iteración %d\n", iteracion);
        
        // Simulamos trabajo de la tarea
        vTaskDelay(pdMS_TO_TICKS(100));
        
        printf("\nT1: Finalizando iteración %d\n", iteracion);
        
        // Cedemos el semáforo para que T3 pueda ejecutarse
        printf("T1: Liberando semáforo para T3\n");
        xSemaphoreGive(xSemaphoreT1T3);
        
        iteracion++;
        
        // Esperamos hasta el próximo periodo
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/*
 * Tarea 2 - Espera el semáforo de T3
 */
void prvTask2_2_2(void *pvParameters)
{
    int iteracion = 0;
    
    for (;;)
    {
        // T2 espera a que T3 le ceda el semáforo
        printf("T2: Esperando semáforo de T3...\n");
        
        if (xSemaphoreTake(xSemaphoreT3T2, portMAX_DELAY) == pdTRUE)
        {
            // T2 realiza su trabajo
            printf("\nT2: Recibido semáforo de T3, iniciando trabajo en iteración %d\n", iteracion);
            
            // Simulamos trabajo de la tarea
            vTaskDelay(pdMS_TO_TICKS(100));
            
            printf("T2: Trabajo completado en iteración %d\n", iteracion);
            iteracion++;
        }
    }
}

/*
 * Tarea 3 - Espera el semáforo de T1 y luego cede el semáforo a T2
 */
void prvTask2_2_3(void *pvParameters)
{
    int iteracion = 0;
    
    for (;;)
    {
        // T3 espera a que T1 le ceda el semáforo
        printf("T3: Esperando semáforo de T1...\n");
        
        if (xSemaphoreTake(xSemaphoreT1T3, portMAX_DELAY) == pdTRUE)
        {
            // T3 realiza su trabajo
            printf("\nT3: Recibido semáforo de T1, iniciando trabajo en iteración %d\n", iteracion);
            
            // Simulamos trabajo de la tarea
            vTaskDelay(pdMS_TO_TICKS(100));
            
            printf("T3: Trabajo completado en iteración %d\n", iteracion);
            
            // Cedemos el semáforo para que T2 pueda ejecutarse
            printf("T3: Liberando semáforo para T2\n");
            xSemaphoreGive(xSemaphoreT3T2);
            
            iteracion++;
        }
    }
}