//
// Created by najamhaq on 2026-01-24.
//

#ifndef MICROBIT_FREERTOS_SRC_PLATFORM_INCLUDE_UART_HW_H
#define MICROBIT_FREERTOS_SRC_PLATFORM_INCLUDE_UART_HW_H

#include <stdint.h>

void uart_hw_init(uint32_t baudrate);
void uart_hw_write_byte(uint8_t byte);

#endif // MICROBIT_FREERTOS_SRC_PLATFORM_INCLUDE_UART_HW_H
