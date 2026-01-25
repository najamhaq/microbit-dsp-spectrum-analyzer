//
// Created by mnajm on 2026-01-19.
//

#ifndef MB2_FREERTOS_MB2_BOARD_H
#define MB2_FREERTOS_MB2_BOARD_H

#include "nrf.h"
#include <stdint.h>

#define MB2_BTN_A_PIN 14u
#define MB2_BTN_B_PIN 23u

#define NRF_PIN(port, pin) (((port) << 5) | (pin))

#define MB2_UART_TX_PIN NRF_PIN(0, 6) // P0.06
#define MB2_UART_RX_PIN NRF_PIN(1, 8) // P1.08

static inline uint32_t BIT(uint32_t pin) { return (1u << (pin)); }

// Some pins are not one the same port. Define a struct to represent a GPIO pin.
typedef struct {
  uint8_t port; // 0 or 1
  uint8_t pin;  // 0..31 (port0) or 0..15 (port1)
} gpio_t;

// Following are the GPIO pins for the LED matrix on micro:bit v2

#endif // MB2_FREERTOS_MB2_BOARD_H