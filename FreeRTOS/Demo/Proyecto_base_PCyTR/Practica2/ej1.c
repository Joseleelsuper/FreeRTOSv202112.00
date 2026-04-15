/*
 * Práctica 2 - Ejercicio 1
 * Tareas no periódicas: tarea de ejecución única y tarea esporádica
 */

#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"

/*
 * Tarea que se ejecuta una única vez y luego se elimina
 */
void prvOneTimeTask2_1(void *pvParameters)
{
    printf("OneTimeTask: Iniciando ejecución única\n");
    
    // Simular algún trabajo
    printf("OneTimeTask: Realizando trabajo...\n");
    vTaskDelay(pdMS_TO_TICKS(3000));  // Retardo de 3 segundos
    
    printf("OneTimeTask: Trabajo completado. Eliminando la tarea.\n");
    
    // Eliminar esta tarea
    vTaskDelete(NULL);
    
    // Este punto nunca se alcanzará
    printf("OneTimeTask: Este mensaje nunca se imprimirá\n");
}

/*
 * Tarea esporádica que se ejecuta en intervalos aleatorios
 */
void prvSporadicTask2_1(void *pvParameters)
{
    TickType_t xDelay;
    int iteration = 0;
    
    for (;;)
    {
        // Generar un tiempo de espera aleatorio entre 1 y 10 segundos
        xDelay = pdMS_TO_TICKS((random() % 9000) + 1000);
        
        printf("SporadicTask: Iteración %d, esperando %lu ms\n", 
               iteration, (unsigned long)(xDelay / portTICK_PERIOD_MS));
        
        // Esperar durante un tiempo aleatorio
        vTaskDelay(xDelay);
        
        // Realizar algún trabajo
        printf("SporadicTask: Ejecutando trabajo en iteración %d\n", iteration);
        
        iteration++;
    }
}