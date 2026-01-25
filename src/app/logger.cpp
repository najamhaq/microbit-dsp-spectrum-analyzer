//
// Created by najamhaq  on 2026-01-24.
//

extern "C" {
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
}

#include <stdint.h>

#include "include/logger.h"
#include "nrf.h"
#include "uart.h"

static Uart g_uart;

// Byte queue: simple and reliable.
static QueueHandle_t g_logQueue = nullptr;

// Tune these later.
static constexpr uint32_t kBaud = UARTE_BAUDRATE_BAUDRATE_Baud115200;
static constexpr UBaseType_t kUartTaskPrio = 2;
static constexpr uint32_t kUartTaskStack = 256;
static constexpr size_t kQueueLen = 512; // bytes

static void UartTxTask(void*) {
  // UART init happens inside the UART task OR before scheduler.
  // Since we already learned init order, we'll keep it explicit here:
  g_uart.init(kBaud);
  g_uart.write("BOOT  . OK\r\n");

  uint8_t b = 0;
  for (;;) {
    if (xQueueReceive(g_logQueue, &b, portMAX_DELAY) == pdTRUE) {
      g_uart.writeByte(b);
    }
  }
}

void logger_init() {
  configASSERT(g_logQueue == nullptr);
  g_logQueue = xQueueCreate(kQueueLen, sizeof(uint8_t));
  configASSERT(g_logQueue != nullptr);

  BaseType_t ok =
      xTaskCreate(UartTxTask, "uart_tx", kUartTaskStack, nullptr, kUartTaskPrio, nullptr);

  configASSERT(ok == pdPASS);
}

void log_write_n(const char* s, std::size_t n) {
  if (!g_logQueue || !s)
    return;

  // Non-blocking enqueue; drops bytes if queue is full.
  for (std::size_t i = 0; i < n; ++i) {
    uint8_t b = static_cast<uint8_t>(s[i]);
    (void)xQueueSend(g_logQueue, &b, 0);
  }
}

void log_write(const char* s) {
  if (!s)
    return;
  while (*s) {
    uint8_t b = static_cast<uint8_t>(*s++);
    (void)xQueueSend(g_logQueue, &b, 0);
  }
}
