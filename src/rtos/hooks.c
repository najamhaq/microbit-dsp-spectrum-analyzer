//
// Created by najamhaq on 2026-01-18.
//

#include "FreeRTOS.h"
#include "task.h"

typedef enum {
  RTOS_TRAP_NONE = 0,
  RTOS_TRAP_MALLOC_FAILED = 1,
  RTOS_TRAP_STACK_OVERFLOW = 2,
  RTOS_TRAP_ASSERT = 3,
} rtos_trap_reason_t;

volatile rtos_trap_reason_t g_rtos_trap_reason = RTOS_TRAP_NONE;
volatile const char* g_rtos_trap_file = 0;
volatile int g_rtos_trap_line = 0;

static void rtos_trap(rtos_trap_reason_t reason) {
  g_rtos_trap_reason = reason;
  taskDISABLE_INTERRUPTS();
  for (;;) {
    /* Optional: place a breakpoint here */
    __asm volatile("nop");
  }
}

void vApplicationMallocFailedHook(void) { rtos_trap(RTOS_TRAP_MALLOC_FAILED); }

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName) {
  (void)xTask;
  (void)pcTaskName;
  rtos_trap(RTOS_TRAP_STACK_OVERFLOW);
}

void vAssertCalled(const char* file, int line) {
  g_rtos_trap_file = file;
  g_rtos_trap_line = line;
  rtos_trap(RTOS_TRAP_ASSERT);
}
