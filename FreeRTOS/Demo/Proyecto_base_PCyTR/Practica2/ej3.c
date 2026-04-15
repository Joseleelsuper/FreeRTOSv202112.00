/*
 * Práctica 2 - Ejercicio 3
 * Funciones para el ejercicio 3 de la práctica 2
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Referencias externas a variables compartidas */
extern int Modify2_3_1;
extern char *Modify2_3_2;

/*
 * Tarea 1 del ejercicio 3
 */
void prvTask2_3_1(void *pvParameters)
{
    const TickType_t xDelay = pdMS_TO_TICKS(2000); // 2 segundos
    int iteracion = 0;
    
    for (;;)
    {
        // Simulación de trabajo para la tarea 1
        printf("Tarea 2_3_1: Iniciando iteración %d\n", iteracion);
        
        // Modificamos la variable compartida Modify2_3_1
        Modify2_3_1 = iteracion;
        printf("Tarea 2_3_1: Estableciendo Modify2_3_1 = %d\n", Modify2_3_1);
        
        // Esperamos un poco antes de terminar esta iteración
        vTaskDelay(xDelay);
        
        iteracion++;
    }
}

/*
 * Tarea 2 del ejercicio 3
 */
void prvTask2_3_2(void *pvParameters)
{
    const TickType_t xDelay = pdMS_TO_TICKS(3000); // 3 segundos
    int iteracion = 0;
    const char *mensajes[] = {
        "Mensaje 1",
        "Mensaje más largo 2",
        "Este es el tercer mensaje",
        "Último mensaje de la serie"
    };
    const int num_mensajes = 4;
    
    for (;;)
    {
        // Simulación de trabajo para la tarea 2
        printf("Tarea 2_3_2: Iniciando iteración %d\n", iteracion);
        
        // Leemos el valor actual de Modify2_3_1
        printf("Tarea 2_3_2: Valor actual de Modify2_3_1 = %d\n", Modify2_3_1);
        
        // Establecemos un mensaje en la variable Modify2_3_2
        Modify2_3_2 = (char *)mensajes[iteracion % num_mensajes];
        printf("Tarea 2_3_2: Estableciendo mensaje: %s\n", Modify2_3_2);
        
        // Esperamos antes de la siguiente iteración
        vTaskDelay(xDelay);
        
        iteracion++;
    }
}