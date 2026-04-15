/*
* Práctica 2 - Ejercicio 6
* Planificador basado en plazos (Earliest Deadline First)
*/

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Máximo número de tareas a gestionar por el planificador EDF */
#define MAX_EDF_TASKS 10

/* Estructura para almacenar información sobre tareas y sus deadlines */
typedef struct {
    TaskHandle_t taskHandle;     /* Handle de la tarea */
    TickType_t deadline;         /* Deadline absoluto en ticks */
    TickType_t period;           /* Periodo de la tarea (0 si no es periódica) */
    UBaseType_t normalPriority;  /* Prioridad normal/base de la tarea */
    char taskName[20];           /* Nombre de la tarea */
} EDFTaskInfo_t;

/* Array para almacenar la información de las tareas EDF */
static EDFTaskInfo_t xEDFTasks[MAX_EDF_TASKS];

/* Mutex para proteger el acceso al array de tareas */
static SemaphoreHandle_t xEDFMutex = NULL;

/* Contador de tareas registradas */
static UBaseType_t uxNumEDFTasks = 0;

/* Función para añadir una tarea al planificador EDF */
BaseType_t xEDFRegisterTask(TaskHandle_t xTaskHandle, TickType_t xDeadline, TickType_t xPeriod) {
    BaseType_t xResult = pdFAIL;
    
    if(xEDFMutex == NULL) {
        /* Inicializar el mutex si es la primera vez */
        xEDFMutex = xSemaphoreCreateMutex();
    }
    
    if(xEDFMutex != NULL && xSemaphoreTake(xEDFMutex, portMAX_DELAY) == pdTRUE) {
        if(uxNumEDFTasks < MAX_EDF_TASKS) {
            /* Obtener la prioridad actual como prioridad base */
            UBaseType_t uxPriority = uxTaskPriorityGet(xTaskHandle);
            
            /* Almacenar información de la tarea */
            xEDFTasks[uxNumEDFTasks].taskHandle = xTaskHandle;
            xEDFTasks[uxNumEDFTasks].deadline = xTaskGetTickCount() + xDeadline;
            xEDFTasks[uxNumEDFTasks].period = xPeriod;
            xEDFTasks[uxNumEDFTasks].normalPriority = uxPriority;
            
            /* Asignar un nombre genérico a la tarea */
            snprintf(xEDFTasks[uxNumEDFTasks].taskName, 20, "Task_%lu", (unsigned long)uxNumEDFTasks);
            
            uxNumEDFTasks++;
            xResult = pdPASS;
            
            printf("Tarea registrada en EDF. ID: %lu, Deadline: %lu, Periodo: %lu\n", 
                (unsigned long)uxNumEDFTasks-1, 
                (unsigned long)xDeadline, 
                (unsigned long)xPeriod);
        } else {
            printf("Error: Número máximo de tareas EDF alcanzado\n");
        }
        
        xSemaphoreGive(xEDFMutex);
    }
    
    return xResult;
}

/* Función para actualizar el deadline de una tarea periódica */
void vEDFUpdateTaskDeadline(TaskHandle_t xTaskHandle) {
    BaseType_t xFound = pdFALSE;
    UBaseType_t uxIndex = 0;
    
    if(xEDFMutex != NULL && xSemaphoreTake(xEDFMutex, portMAX_DELAY) == pdTRUE) {
        /* Buscar la tarea en el array */
        for(uxIndex = 0; uxIndex < uxNumEDFTasks; uxIndex++) {
            if(xEDFTasks[uxIndex].taskHandle == xTaskHandle) {
                /* Actualizar deadline basado en el periodo */
                if(xEDFTasks[uxIndex].period > 0) {
                    xEDFTasks[uxIndex].deadline = xTaskGetTickCount() + xEDFTasks[uxIndex].period;
                    xFound = pdTRUE;
                    printf("Deadline actualizado para tarea %s: %lu\n", 
                        xEDFTasks[uxIndex].taskName, 
                        (unsigned long)xEDFTasks[uxIndex].deadline);
                }
                break;
            }
        }
        
        xSemaphoreGive(xEDFMutex);
    }
    
    if(!xFound) {
        printf("Error: No se pudo actualizar el deadline. Tarea no encontrada.\n");
    }
}

/* Función para eliminar una tarea del planificador EDF */
void vEDFUnregisterTask(TaskHandle_t xTaskHandle) {
    UBaseType_t uxIndex, uxNewIndex;
    BaseType_t xFound = pdFALSE;
    
    if(xEDFMutex != NULL && xSemaphoreTake(xEDFMutex, portMAX_DELAY) == pdTRUE) {
        /* Buscar la tarea y compactar el array */
        for(uxIndex = 0, uxNewIndex = 0; uxIndex < uxNumEDFTasks; uxIndex++) {
            if(xEDFTasks[uxIndex].taskHandle == xTaskHandle) {
                /* Encontrada la tarea a eliminar */
                xFound = pdTRUE;
                printf("Tarea %s eliminada del planificador EDF\n", xEDFTasks[uxIndex].taskName);
                
                /* Restaurar la prioridad original de la tarea */
                vTaskPrioritySet(xTaskHandle, xEDFTasks[uxIndex].normalPriority);
            } else {
                /* Compactar array moviendo elementos si es necesario */
                if(uxIndex != uxNewIndex) {
                    xEDFTasks[uxNewIndex] = xEDFTasks[uxIndex];
                }
                uxNewIndex++;
            }
        }
        
        /* Si se encontró y eliminó una tarea, decrementar el contador */
        if(xFound) {
            uxNumEDFTasks--;
        }
        
        xSemaphoreGive(xEDFMutex);
    }
    
    if(!xFound) {
        printf("Error: No se pudo eliminar la tarea. No encontrada en EDF.\n");
    }
}

/* Función para ejecutar el planificador EDF */
void vEDFSchedulerTask2_6(void *pvParameters) {
    UBaseType_t uxI, uxJ;
    EDFTaskInfo_t xTemp;
    TickType_t xCurrentTime;
    
    /* Inicializar mutex si no existe */
    if(xEDFMutex == NULL) {
        xEDFMutex = xSemaphoreCreateMutex();
    }
    
    for(;;) {
        if(xEDFMutex != NULL && xSemaphoreTake(xEDFMutex, portMAX_DELAY) == pdTRUE) {
            xCurrentTime = xTaskGetTickCount();
            
            /* Ordenar tareas por deadline (algoritmo de burbuja) */
            for(uxI = 0; uxI < uxNumEDFTasks; uxI++) {
                for(uxJ = 0; uxJ < uxNumEDFTasks - uxI - 1; uxJ++) {
                    if(xEDFTasks[uxJ].deadline > xEDFTasks[uxJ + 1].deadline) {
                        /* Intercambiar elementos */
                        xTemp = xEDFTasks[uxJ];
                        xEDFTasks[uxJ] = xEDFTasks[uxJ + 1];
                        xEDFTasks[uxJ + 1] = xTemp;
                    }
                }
            }
            
            /* Asignar prioridades en función del orden de deadline */
            for(uxI = 0; uxI < uxNumEDFTasks; uxI++) {
                /* La tarea con deadline más cercano recibe la mayor prioridad */
                UBaseType_t uxNewPriority = configMAX_PRIORITIES - 2 - uxI;
                
                /* Evitar prioridades por debajo de la normal para la tarea */
                if(uxNewPriority < xEDFTasks[uxI].normalPriority) {
                    uxNewPriority = xEDFTasks[uxI].normalPriority;
                }
                
                /* Asignar nueva prioridad */
                vTaskPrioritySet(xEDFTasks[uxI].taskHandle, uxNewPriority);
                
                /* Depuración: mostrar estado de las tareas */
                printf("EDF: Tarea %s, Deadline: %lu, Nueva prioridad: %lu, Tiempo actual: %lu\n", 
                    xEDFTasks[uxI].taskName,
                    (unsigned long)xEDFTasks[uxI].deadline, 
                    (unsigned long)uxNewPriority,
                    (unsigned long)xCurrentTime);
            }
            
            xSemaphoreGive(xEDFMutex);
        }
        
        /* Ejecutar planificador cada 100ms */
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/* Tareas de ejemplo para probar el planificador EDF */

/* Tarea con deadline corto */
void prvShortDeadlineTask2_6(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(500); /* Periodo de 500ms */
    int i = 0;
    
    /* Registrar la tarea en el planificador EDF */
    xEDFRegisterTask(xTaskGetCurrentTaskHandle(), pdMS_TO_TICKS(100), xPeriod);
    
    xLastWakeTime = xTaskGetTickCount();
    
    for(;;) {
        printf("Tarea de plazo corto: iteración %d\n", i++);
        
        /* Simular trabajo */
        vTaskDelay(pdMS_TO_TICKS(50));
        
        /* Actualizar deadline y esperar hasta siguiente periodo */
        vEDFUpdateTaskDeadline(xTaskGetCurrentTaskHandle());
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/* Tarea con deadline medio */
void prvMediumDeadlineTask2_6(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(1000); /* Periodo de 1 segundo */
    int i = 0;
    
    /* Registrar la tarea en el planificador EDF */
    xEDFRegisterTask(xTaskGetCurrentTaskHandle(), pdMS_TO_TICKS(500), xPeriod);
    
    xLastWakeTime = xTaskGetTickCount();
    
    for(;;) {
        printf("Tarea de plazo medio: iteración %d\n", i++);
        
        /* Simular trabajo */
        vTaskDelay(pdMS_TO_TICKS(100));
        
        /* Actualizar deadline y esperar hasta siguiente periodo */
        vEDFUpdateTaskDeadline(xTaskGetCurrentTaskHandle());
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

/* Tarea con deadline largo */
void prvLongDeadlineTask2_6(void *pvParameters) {
    TickType_t xLastWakeTime;
    const TickType_t xPeriod = pdMS_TO_TICKS(2000); /* Periodo de 2 segundos */
    int i = 0;
    
    /* Registrar la tarea en el planificador EDF */
    xEDFRegisterTask(xTaskGetCurrentTaskHandle(), pdMS_TO_TICKS(1500), xPeriod);
    
    xLastWakeTime = xTaskGetTickCount();
    
    for(;;) {
        printf("Tarea de plazo largo: iteración %d\n", i++);
        
        /* Simular trabajo */
        vTaskDelay(pdMS_TO_TICKS(200));
        
        /* Actualizar deadline y esperar hasta siguiente periodo */
        vEDFUpdateTaskDeadline(xTaskGetCurrentTaskHandle());
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}