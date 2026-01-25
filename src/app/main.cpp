extern "C" {
#include "FreeRTOS.h"
#include "microbit_ver_2.h"
#include "task.h"
}

//#include "app_queues.h"
//#include "led_matrix.h"
//#include "lock.h"
//#include "logger.h"
//#include "nrf.h"
//#include "uart.h"
//#include "ui_lock.h"

//static Uart uart;
//
//// forward declare task entry (C linkage if needed)
//extern "C" void ButtonTask(void*);
//extern "C" void InputController(void*);
//
//static void BlinkTask(void*) {
//  for (;;) {
//    __asm volatile("nop");
//    vTaskDelay(pdMS_TO_TICKS(500));
//    Lock lk(g_ledMutex);
//    LEDMatrix::led_on(3, 3);
//    vTaskDelay(pdMS_TO_TICKS(500));
//    LEDMatrix::led_off(3, 3);
//  }
//}
//
//void HeartbeatTask(void*) {
//  uart.write("UART task started\r\n");
//
//  for (;;) {
//    uart.write("heartbeat\r\n");
//    vTaskDelay(pdMS_TO_TICKS(2000));
//  }
//}
//
//void rtos_setup() {
//  g_ledMutex = xSemaphoreCreateMutex();
//  configASSERT(g_ledMutex != nullptr);
//  g_inputQueue = xQueueCreate(/*length=*/16, /*item_size=*/sizeof(InputEvent));
//  configASSERT(g_inputQueue != nullptr);
//
//  BaseType_t ok = xTaskCreate(ButtonTask, "Button",
//                              /*stackWords=*/256, nullptr,
//                              /*priority=*/tskIDLE_PRIORITY + 2, nullptr);
//
//  configASSERT(ok == pdPASS);
//
//  BaseType_t ok2 =
//      xTaskCreate(InputController, "InputProc", 256, nullptr, tskIDLE_PRIORITY + 2, nullptr);
//
//  configASSERT(ok2 == pdPASS);
//
//  ok2 = xTaskCreate(BlinkTask, "blink", 128, nullptr, 1, nullptr);
//  configASSERT(ok2 == pdPASS);
//}

//void hardware_setup() {
//  LEDMatrix::init();
//  buttons_init();
//  uart.init(UARTE_BAUDRATE_BAUDRATE_Baud115200);
//}
//void logger_setup() {
//  logger_init();
//  xTaskCreate(HeartbeatTask, "heartbeat", 256, nullptr, 1, nullptr);
//}
//
extern "C" int main(void) {
//  hardware_setup();
//  rtos_setup();
//  logger_setup();
//  vTaskStartScheduler();
  // If we get here, scheduler failed (heap/stack)
  for (;;) {
    __asm volatile("nop");
  }
}
