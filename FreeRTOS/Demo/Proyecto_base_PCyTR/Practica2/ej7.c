/*
 * Ejercicio 7 - Herencia de prioridad
 *
 * Este ejercicio demuestra cómo funciona la herencia de prioridad en FreeRTOS para evitar
 * la inversión de prioridad cuando una tarea de menor prioridad bloquea un recurso
 * necesario para una tarea de mayor prioridad.
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Definición de prioridades para las tareas */
#define TASK_PRIORITY_T1    4
#define TASK_PRIORITY_T2    3
#define TASK_PRIORITY_T3    5

/* Períodos de las tareas */
#define PERIOD_T1_MS        pdMS_TO_TICKS(5000) /* 5 segundos */
#define PERIOD_T2_MS        pdMS_TO_TICKS(10000) /* 10 segundos */
#define PERIOD_T3_MS        pdMS_TO_TICKS(5000) /* 5 segundos */

/* Semáforo mutex compartido entre T2 y T3 */
static SemaphoreHandle_t xMutexS1 = NULL;

/*
 * Tarea T1: Prioridad 4, Periodo 5s, Consumo de CPU 3s
 */
void prvTask2_7_1(void *pvParameters)
{
    TickType_t xLastWakeTime;

    /* Inicialización del tiempo de despertar de la tarea */
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        printf("T1 (P4): Iniciando ejecución en %lu ms\n", xTaskGetTickCount());
        
        /* Consume CPU durante 3 segundos con un bucle */
        TickType_t xStartTime = xTaskGetTickCount();
        while ((xTaskGetTickCount() - xStartTime) < pdMS_TO_TICKS(3000))
        {
            /* Consumo de CPU */
        }
        
        printf("T1 (P4): Finalizada ejecución en %lu ms\n", xTaskGetTickCount());
        
        /* Espera hasta el siguiente periodo */
        vTaskDelayUntil(&xLastWakeTime, PERIOD_T1_MS);
    }
}

/*
 * Tarea T2: Prioridad 3, Periodo 10s, Consumo de CPU 3s
 * Utiliza el semáforo S1 para proteger su región crítica
 */
void prvTask2_7_2(void *pvParameters)
{
    TickType_t xLastWakeTime;

    /* Inicialización del tiempo de despertar de la tarea */
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        printf("T2 (P3): Intentando adquirir mutex S1 en %lu ms\n", xTaskGetTickCount());
        
        /* Intenta adquirir el semáforo mutex S1 */
        xSemaphoreTake(xMutexS1, portMAX_DELAY);
        
        /* Región crítica protegida por el mutex S1 */
        printf("T2 (P3): Mutex S1 adquirido, iniciando ejecución en %lu ms, prioridad actual: %lu\n", 
               xTaskGetTickCount(), uxTaskPriorityGet(NULL));
        
        /* Consume CPU durante 3 segundos con un bucle */
        TickType_t xStartTime = xTaskGetTickCount();
        while ((xTaskGetTickCount() - xStartTime) < pdMS_TO_TICKS(3000))
        {
            /* Consumo de CPU */
        }
        
        printf("T2 (P3): Finalizando ejecución, liberando mutex S1 en %lu ms\n", xTaskGetTickCount());
        
        /* Libera el semáforo mutex S1 */
        xSemaphoreGive(xMutexS1);
        
        /* Espera hasta el siguiente periodo */
        vTaskDelayUntil(&xLastWakeTime, PERIOD_T2_MS);
    }
}

/*
 * Tarea T3: Prioridad 5, Periodo 5s, Consumo de CPU 1s
 * También utiliza el semáforo S1 para proteger su región crítica
 */
void prvTask2_7_3(void *pvParameters)
{
    TickType_t xLastWakeTime;

    /* Inicialización del tiempo de despertar de la tarea */
    xLastWakeTime = xTaskGetTickCount();

    for (;;)
    {
        printf("T3 (P5): Intentando adquirir mutex S1 en %lu ms\n", xTaskGetTickCount());
        
        /* Intenta adquirir el semáforo mutex S1 */
        xSemaphoreTake(xMutexS1, portMAX_DELAY);
        
        /* Región crítica protegida por el mutex S1 */
        printf("T3 (P5): Mutex S1 adquirido, iniciando ejecución en %lu ms\n", xTaskGetTickCount());
        
        /* Consume CPU durante 1 segundo con un bucle */
        TickType_t xStartTime = xTaskGetTickCount();
        while ((xTaskGetTickCount() - xStartTime) < pdMS_TO_TICKS(1000))
        {
            /* Consumo de CPU */
        }
        
        printf("T3 (P5): Finalizando ejecución, liberando mutex S1 en %lu ms\n", xTaskGetTickCount());
        
        /* Libera el semáforo mutex S1 */
        xSemaphoreGive(xMutexS1);
        
        /* Espera hasta el siguiente periodo */
        vTaskDelayUntil(&xLastWakeTime, PERIOD_T3_MS);
    }
}

/*
 * Función para iniciar las tareas del ejercicio 7
 */
void vStartPriorityInheritanceTasks(void)
{
    /* Crea el semáforo mutex S1 */
    xMutexS1 = xSemaphoreCreateMutex();
    
    if (xMutexS1 == NULL)
    {
        printf("Error: No se pudo crear el semáforo mutex S1\n");
        return;
    }
    
    /* Crea las tres tareas periódicas */
    xTaskCreate(prvTask2_7_1, "T1", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_T1, NULL);
    xTaskCreate(prvTask2_7_2, "T2", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_T2, NULL);
    xTaskCreate(prvTask2_7_3, "T3", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY_T3, NULL);
    
    printf("Tareas de herencia de prioridad creadas\n");
}