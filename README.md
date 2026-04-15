# Guia rapida de uso: Proyecto_base_PCyTR

Este repositorio es muy grande, pero para trabajar con tus practicas solo necesitas esta carpeta:

- `FreeRTOS/Demo/Proyecto_base_PCyTR`

La ejecucion de las funciones de practicas se controla desde:

- `FreeRTOS/Demo/Proyecto_base_PCyTR/main_base.c`

El script que automatiza compilacion/ejecucion es:

- `FreeRTOS/Demo/Proyecto_base_PCyTR/build.sh`

## Índice

- [Requisitos minimos (Linux)](#requisitos-minimos-linux)
- [Configuracion inicial](#configuracion-inicial)
- [Flujo general para ejecutar cualquier ejercicio](#flujo-general-para-ejecutar-cualquier-ejercicio)
- [Donde activar cada funcion](#donde-activar-cada-funcion)
  - [Practica 1](#practica-1)
    - [Ejercicio 1 (`prvTask1_1`)](#ejercicio-1-prvtask1_1)
    - [Ejercicio 2 (`prvTask1_2_1`, `prvTask1_2_2`)](#ejercicio-2-prvtask1_2_1-prvtask1_2_2)
    - [Ejercicio 3 (`prvTask1_3` + Idle hook)](#ejercicio-3-prvtask1_3--idle-hook)
    - [Ejercicio 4 (`prvTask1_4_1`, `prvTask1_4_2`)](#ejercicio-4-prvtask1_4_1-prvtask1_4_2)
    - [Ejercicio 5 (`prvTask1_5_1`, `prvTask1_5_2` + cola)](#ejercicio-5-prvtask1_5_1-prvtask1_5_2--cola)
  - [Practica 2](#practica-2)
    - [Ejercicio 1 (`prvOneTimeTask2_1`, `prvSporadicTask2_1`)](#ejercicio-1-prvonetimetask2_1-prvsporadictask2_1)
    - [Ejercicio 2 (`prvTask2_2_1`, `prvTask2_2_2`, `prvTask2_2_3` + semaforos)](#ejercicio-2-prvtask2_2_1-prvtask2_2_2-prvtask2_2_3--semaforos)
    - [Ejercicio 3 (`prvTask2_3_1`, `prvTask2_3_2`)](#ejercicio-3-prvtask2_3_1-prvtask2_3_2)
    - [Ejercicio 6 (EDF)](#ejercicio-6-edf)
    - [Ejercicio 7 (herencia de prioridad)](#ejercicio-7-herencia-de-prioridad)
- [Ejecucion recomendada por ejercicio](#ejecucion-recomendada-por-ejercicio)
- [Resolucion de problemas rapida](#resolucion-de-problemas-rapida)
- [Autor](#autor)

## Requisitos minimos (Linux)

- Instala la máquina virtual compartida por el profesor haciendo clic en [este enlace](https://universidaddeburgos-my.sharepoint.com/:u:/r/personal/ruben_ruiz_ubu_es/Documents/PCyTR.7z?csf=1&web=1&e=R1Eqac)

## Configuracion inicial

Simplemente haz un git clone de este proyecto a cualquier carpeta de tu sistema. No es necesario copiar nada ni instalar dependencias adicionales al venir ya todo configurado.
```bash
git clone https://github.com/Joseleelsuper/FreeRTOSv202112.00.git
```

## Flujo general para ejecutar cualquier ejercicio

1. Entra al proyecto:

```bash
cd FreeRTOS/Demo/Proyecto_base_PCyTR
```

2. Edita `main_base.c` y deja activo solo el ejercicio que quieras probar.

3. Compila y ejecuta:

```bash
chmod +x build.sh
./build.sh
```

4. Para terminar la ejecucion, usa `Ctrl+C`.

> [!Important]
> No ejecutes varias practicas a la vez salvo que sea intencional. Si dejas muchas lineas `xTaskCreate(...)` activas, la salida se mezcla y es dificil de analizar.

Detalles importantes:

- Todos los errores se redirigen a `error.txt`.
- Si falla algun paso, el script para y te lo indica.
- En el paso `make profile`, el binario se lanza para generar `gmon.out`.
- Como el demo no termina solo, debes pulsar `Ctrl+C` cuando quieras cerrar para que se genere el perfil.
- Si no se genera `gmon.out`, revisa `error.txt`.
- Al final del script, se lanza otra vez `./build/posix_demo` para ejecucion normal.

Archivos de perfil generados:

- `build/prof_flat.txt`
- `build/prof_call_graph.txt`

## Donde activar cada funcion

Todas las funciones de practicas se ejecutan como tareas FreeRTOS. No se invocan directamente desde `main()`: se activan descomentando su `xTaskCreate(...)` dentro de:

- `funcs_p1()` para Practica 1
- `funcs_p2()` para Practica 2

Archivo: `FreeRTOS/Demo/Proyecto_base_PCyTR/main_base.c`

### Practica 1

#### Ejercicio 1 (`prvTask1_1`)

Activa:

```c
xTaskCreate( prvTask1_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
```

Salida esperada: `Iteracion N` cada ~1 segundo.

#### Ejercicio 2 (`prvTask1_2_1`, `prvTask1_2_2`)

Comenta el ejercicio 1 y activa:

```c
xTaskCreate( prvTask1_2_1, "Task1", configMINIMAL_STACK_SIZE, NULL, 5, NULL );
xTaskCreate( prvTask1_2_2, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
```

Salida esperada:

- `Task1: Iteracion ...` (periodo largo, 10 s)
- `Task2: Iteracion ...` (periodo corto, 1 s)

#### Ejercicio 3 (`prvTask1_3` + Idle hook)

Activa:

```c
xTaskCreate( prvTask1_3, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
```

Salida esperada intercalada:

- `Iteracion ...`
- `Ejecutando IdleTask...`

#### Ejercicio 4 (`prvTask1_4_1`, `prvTask1_4_2`)

Activa:

```c
xTaskCreate( prvTask1_4_1, "Task1", configMINIMAL_STACK_SIZE, NULL, 2, NULL );
xTaskCreate( prvTask1_4_2, "Task2", configMINIMAL_STACK_SIZE, NULL, 3, NULL );
```

Salida esperada: mensajes de Task1 y Task2, y cambio de prioridad de Task1 en la iteracion 5.

#### Ejercicio 5 (`prvTask1_5_1`, `prvTask1_5_2` + cola)

Ademas de crear tareas, necesitas inicializar cola y semilla aleatoria.

Activa este bloque:

```c
xQueue = xQueueCreate(10, sizeof(int));
if (xQueue == NULL) {
	printf("Error: No se pudo crear la cola\n");
}
srandom(xTaskGetTickCount());
xTaskCreate( prvTask1_5_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
xTaskCreate( prvTask1_5_2, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK2_PRIORITY, NULL );
```

Salida esperada:

- Productor: `Task1 (enviado): ...`
- Consumidor: `Task2 (recibido): ...`

### Practica 2

#### Ejercicio 1 (`prvOneTimeTask2_1`, `prvSporadicTask2_1`)

Activa:

```c
xTaskCreate( prvOneTimeTask2_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
xTaskCreate( prvSporadicTask2_1, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
```

Salida esperada:

- `OneTimeTask...` se ejecuta una sola vez y se elimina.
- `SporadicTask...` aparece con retardos aleatorios.

#### Ejercicio 2 (`prvTask2_2_1`, `prvTask2_2_2`, `prvTask2_2_3` + semaforos)

Primero crea semaforos, luego tareas:

```c
xSemaphoreT1T3 = xSemaphoreCreateBinary();
xSemaphoreT3T2 = xSemaphoreCreateBinary();
xTaskCreate( prvTask2_2_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
xTaskCreate( prvTask2_2_2, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
xTaskCreate( prvTask2_2_3, "Task3", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
```

Salida esperada: secuencia de sincronizacion `T1 -> T3 -> T2`.

Nota: puedes repetir la prueba cambiando `configUSE_PREEMPTION` en `FreeRTOSConfig.h` (0 o 1).

#### Ejercicio 3 (`prvTask2_3_1`, `prvTask2_3_2`)

Activa:

```c
xTaskCreate( prvTask2_3_1, "Task1", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
xTaskCreate( prvTask2_3_2, "Task2", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL );
```

Salida esperada: cambios en `Modify2_3_1` y `Modify2_3_2` impresos por consola.

#### Ejercicio 6 (EDF)

Activa semilla y planificador con sus tareas:

```c
srandom(33);
xTaskCreate(prvShortDeadlineTask2_6, "ShortDL", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
xTaskCreate(prvMediumDeadlineTask2_6, "MediumDL", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
xTaskCreate(prvLongDeadlineTask2_6, "LongDL", configMINIMAL_STACK_SIZE, NULL, TASK1_PRIORITY, NULL);
xTaskCreate(vEDFSchedulerTask2_6, "EDFScheduler", configMINIMAL_STACK_SIZE * 2, NULL, EDF_SCHEDULER_PRIORITY, NULL);
```

Salida esperada:

- Mensajes `Tarea de plazo corto/medio/largo...`
- Mensajes `EDF: Tarea ... Nueva prioridad ...`

Funciones internas relacionadas (en `Practica2/ej6.c`):

- `xEDFRegisterTask`
- `vEDFUpdateTaskDeadline`
- `vEDFUnregisterTask`
- `vEDFSchedulerTask2_6`

No se llaman manualmente desde consola; se usan desde las tareas cuando activas este bloque.

#### Ejercicio 7 (herencia de prioridad)

Activa:

```c
vStartPriorityInheritanceTasks();
```

Esta funcion crea internamente `prvTask2_7_1`, `prvTask2_7_2` y `prvTask2_7_3` y el mutex compartido.

Salida esperada:

- Intentos de adquirir mutex de T2/T3
- Cambios de ejecucion que evidencian herencia de prioridad

## Ejecucion recomendada por ejercicio

Para evitar conflictos:

1. Deja activo un solo ejercicio en `main_base.c`.
2. Ejecuta `./build.sh`.
3. Observa la consola.
4. Deten con `Ctrl+C`.
5. Cambia a otro ejercicio y repite.

## Resolucion de problemas rapida

- Si no compila: revisa `error.txt`.
- Si `make profile` se queda ejecutando: es normal, para con `Ctrl+C` cuando quieras terminar captura.
- Si no aparecen `prof_flat.txt` y `prof_call_graph.txt`: asegurate de haber detenido una ejecucion para que se cree `gmon.out`.
- Si no ves mensajes esperados: probablemente hay lineas de otro ejercicio aun activas en `main_base.c`.

## Autor

<table>
    <tr>
        <td align="center"><a href="https://joseleelportfolio.vercel.app/"><img src="https://github.com/Joseleelsuper.png" width="100px;" alt=""/><br /><sub><b>José Gallardo</b></sub></a></td>
    </tr>
</table>

---

> Volver al [índice](#índice)