//
// Created by najamhaq on 2026-01-18.
//

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((noreturn)) void DebugMon_Handler(void) {
  while (1) {
    __asm volatile("nop");
  }
}

#ifdef __cplusplus
}
#endif
