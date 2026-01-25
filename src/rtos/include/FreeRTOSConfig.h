//
// Created by najamhaq on 2026-01-18.
//

#ifndef MB2_FREERTOS_FREERTOSCONFIG_H
#define MB2_FREERTOS_FREERTOSCONFIG_H

/*
 * Minimal FreeRTOSConfig.h for:
 *  - BBC micro:bit v2
 *  - nRF52833 (ARM Cortex-M4F)
 *
 * Goal: proof-of-life bring-up (scheduler runs, vTaskDelay works, failures trap).
 */

#include <stdint.h>

/* -----------------------------------------------------------
 * Basic kernel behavior
 * ----------------------------------------------------------- */
#define configUSE_PREEMPTION 1
#define configUSE_TIME_SLICING 1
#define configTICK_RATE_HZ ((TickType_t)1000)
#define configMAX_PRIORITIES 5
#define configMINIMAL_STACK_SIZE ((configSTACK_DEPTH_TYPE)256)
#define configMAX_TASK_NAME_LEN 16

#define configSUPPORT_STATIC_ALLOCATION 0
#define configSUPPORT_DYNAMIC_ALLOCATION 1

/* We MUST match this to your actual CPU clock. nRF52833 runs at 64MHz when HFCLK is enabled. */
#define configCPU_CLOCK_HZ ((uint32_t)64000000)

/* -----------------------------------------------------------
 * Memory / heap
 * ----------------------------------------------------------- */
/* Using heap_4.c (malloc/free with coalescing). */
#define configTOTAL_HEAP_SIZE ((size_t)(16 * 1024))
#define configAPPLICATION_ALLOCATED_HEAP 0

/* -----------------------------------------------------------
 * Hook functions (we implement these in rtos/hooks.c)
 * ----------------------------------------------------------- */
#define configUSE_MALLOC_FAILED_HOOK 1
#define configCHECK_FOR_STACK_OVERFLOW 2

#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0

/* -----------------------------------------------------------
 * Optional API features (minimal set for bring-up)
 * ----------------------------------------------------------- */
#define configUSE_MUTEXES 1
#define configUSE_COUNTING_SEMAPHORES 1
#define configQUEUE_REGISTRY_SIZE 0

#define configUSE_16_BIT_TICKS 0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

/* -----------------------------------------------------------
 * Interrupt priority configuration (Cortex-M specific)
 * nRF52 implements 3 priority bits (0-7). If we later include CMSIS,
 * we can replace this with:  #define configPRIO_BITS __NVIC_PRIO_BITS
 * ----------------------------------------------------------- */
#ifndef configPRIO_BITS
#define configPRIO_BITS 3 /* 3 bits for nRF52 */
#endif

/* Lowest (numerically largest) priority. */
#define configKERNEL_INTERRUPT_PRIORITY                                                            \
  ((uint32_t)((1U << configPRIO_BITS) - 1U) << (8U - configPRIO_BITS))

/*
 * Highest ISR priority from which FreeRTOS API calls are allowed.
 * Smaller numeric value = higher priority on Cortex-M.
 *
 * IMPORTANT:
 *  - This MUST NOT be 0.
 *  - Any ISR that calls ...FromISR() must have priority numerically >= this value.
 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY ((uint32_t)(5U << (8U - configPRIO_BITS)))

/* -----------------------------------------------------------
 * Assertions (we implement vAssertCalled in rtos/hooks.c)
 * ----------------------------------------------------------- */
void vAssertCalled(const char* file, int line);
#define configASSERT(x)                                                                            \
  do {                                                                                             \
    if ((x) == 0)                                                                                  \
      vAssertCalled(__FILE__, __LINE__);                                                           \
  } while (0)

/* -----------------------------------------------------------
 * Cortex-M port requires these
 * ----------------------------------------------------------- */
#define configENABLE_FPU 1

/* -----------------------------------------------------------
 * API function inclusion (keep minimal; expand as needed)
 * ----------------------------------------------------------- */
#define INCLUDE_vTaskDelay 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1

#endif // MB2_FREERTOS_FREERTOSCONFIG_H