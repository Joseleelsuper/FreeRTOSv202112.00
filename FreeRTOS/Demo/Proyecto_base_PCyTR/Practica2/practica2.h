/*
 * Práctica 2 - Definiciones de funciones y constantes
 */

#ifndef PRACTICA2_H_
#define PRACTICA2_H_

#include "FreeRTOS.h"
#include "task.h"

/* Ejercicio 1 - Tareas no periódicas */
void prvOneTimeTask2_1(void *pvParameters);
void prvSporadicTask2_1(void *pvParameters);

/* Ejercicio 2 - Sincronización con semáforos */
void prvTask2_2_1(void *pvParameters);
void prvTask2_2_2(void *pvParameters);
void prvTask2_2_3(void *pvParameters);

/* Ejercicio 3 - Memoria compartida */
void prvTask2_3_1(void *pvParameters);
void prvTask2_3_2(void *pvParameters);

/* Ejercicio 6 - Planificación basada en plazos (EDF) */
/* Funciones API para el planificador EDF */
BaseType_t xEDFRegisterTask(TaskHandle_t xTaskHandle, TickType_t xDeadline, TickType_t xPeriod);
void vEDFUpdateTaskDeadline(TaskHandle_t xTaskHandle);
void vEDFUnregisterTask(TaskHandle_t xTaskHandle);
void vEDFSchedulerTask2_6(void *pvParameters);
/* Tareas de ejemplo para el planificador EDF */
void prvShortDeadlineTask2_6(void *pvParameters);
void prvMediumDeadlineTask2_6(void *pvParameters);
void prvLongDeadlineTask2_6(void *pvParameters);

/* Ejercicio 7 - Herencia de prioridad */
void vStartPriorityInheritanceTasks(void);

#endif /* PRACTICA2_H_ */