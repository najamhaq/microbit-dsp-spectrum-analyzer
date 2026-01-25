extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}
//
extern "C" int main(void) {
  // If we get here, scheduler failed (heap/stack)
  for (;;) {
    __asm volatile("nop");
  }
}
