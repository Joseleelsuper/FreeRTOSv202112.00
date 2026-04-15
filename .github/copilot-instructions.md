# Copilot Instructions for FreeRTOSv202112.00

## Scope and Edit Boundaries
- Most of this repository is upstream FreeRTOS code. Coursework-specific changes are centered in `FreeRTOS/Demo/Proyecto_base_PCyTR`.
- Unless explicitly requested, keep edits inside `FreeRTOS/Demo/Proyecto_base_PCyTR` and avoid broad changes under `FreeRTOS/Source` or `FreeRTOS-Plus/Source`.

## Runtime Architecture (PCyTR demo)
- Program entry is `FreeRTOS/Demo/Proyecto_base_PCyTR/main.c`: it sets SIGINT handling, starts trace facilities, initializes console, then calls `main_base()`.
- Exercise orchestration is in `FreeRTOS/Demo/Proyecto_base_PCyTR/main_base.c`: `funcs_p1()` and `funcs_p2()` choose active exercises, then `vTaskStartScheduler()` starts FreeRTOS.
- Exercise implementations live in `FreeRTOS/Demo/Proyecto_base_PCyTR/Practica1/*.c` and `FreeRTOS/Demo/Proyecto_base_PCyTR/Practica2/*.c`.
- Shared runtime state (queue, semaphores, shared variables) is declared globally in `main_base.c` and consumed via `extern` in exercise files.

## Build, Run, and Debug Workflow
- Work from `FreeRTOS/Demo/Proyecto_base_PCyTR`.
- Standard run path: `./build.sh` (runs clean, profile build, profiling stage, then final execution; stderr goes to `error.txt`).
- Manual build path: `make clean && make PROFILE=1 && ./build/posix_demo`.
- Profiling flow: `make profile` runs the demo and expects Ctrl+C to generate `gmon.out`, then writes `build/prof_flat.txt` and `build/prof_call_graph.txt`.
- Sanitizers are supported: `make SANITIZE_ADDRESS=1` or `make SANITIZE_LEAK=1`.

## Project-Specific Coding Patterns
- To switch exercises, comment/uncomment `xTaskCreate(...)` calls in `funcs_p1()`/`funcs_p2()` in `main_base.c`.
- Prefer running one exercise at a time to keep console output and timing behavior interpretable.
- Initialize sync primitives before creating dependent tasks (for example `xQueueCreate`, `xSemaphoreCreateBinary`, mutex creation helpers).
- Keep priorities within `configMAX_PRIORITIES` from `FreeRTOSConfig.h` and reuse local macros (`TASK1_PRIORITY`, `TASK2_PRIORITY`, `EDF_SCHEDULER_PRIORITY`).
- Preserve the existing style in this demo: C99-like FreeRTOS patterns and Spanish comments/log messages in practice files.

## Integration and Configuration Points
- `FreeRTOS/Demo/Proyecto_base_PCyTR/Makefile` links local demo sources plus kernel and trace sources from `../../../FreeRTOS` and `../../../FreeRTOS-Plus`.
- Build behavior is controlled by Make variables (`PROFILE`, `TRACE_ON_ENTER`, `COVERAGE_TEST`, `SANITIZE_ADDRESS`, `SANITIZE_LEAK`).
- Scheduler behavior for some exercises depends on `configUSE_PREEMPTION` in `FreeRTOS/Demo/Proyecto_base_PCyTR/FreeRTOSConfig.h`.

## Validation Expectations for Agent Changes
- There is no single lightweight unit-test target for this demo folder; validate by compiling and running the affected exercise flow.
- For behavior changes, report the exact command used and the key console output that confirms the change.

## Generated Files and Commit Hygiene
- Do not commit generated outputs such as `build/`, `*.dump`, `gmon.out`, profiling outputs, or `error.txt`.
- Keep docs synchronized when changing activation flow, especially `README.md` and `FreeRTOS/Demo/Proyecto_base_PCyTR/Readme.md`.
