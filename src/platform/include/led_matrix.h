//
// Created by najamhaq  on 2026-01-19.
//
// @formatter:on

#ifndef MB2_FREERTOS_LEDMATRIC_H
#define MB2_FREERTOS_LEDMATRIC_H
#include "microbit_ver_2.h"

// What Port 0 and Port 1 pins are used for the LED matrix?
#define LED_P0_MASK                                                                                \
  ((1u << 21) | (1u << 22) | (1u << 15) | (1u << 24) | (1u << 19) | (1u << 28) | (1u << 11) |      \
   (1u << 31) | (1u << 30))

#define LED_P1_MASK (1u << 5)

/**
 * We define array of row and column pins to map specific LED PINS
 * to their GPIO port and pin numbers.
 * Following are the GPIO pins for the LED matrix on micro:bit v2
 * Index define which row [or column]
 * Value define which GPIO pin it is connected to and on what port.
 */

static const gpio_t col_pins[5] = {{0, 28}, {0, 11}, {0, 31}, {1, 5}, {0, 30}};
static const gpio_t row_pins[5] = {{0, 21}, {0, 22}, {0, 15}, {0, 24}, {0, 19}};

class LEDMatrix {
public:
  //  static void setPixel(int x, int y, bool on);
  //  static void setAll(bool on);
  //   static void scanStep();    // called periodically (2ms)

  static void init() {
    // Configure row + columns as outputs
    // make sure we dont set anything else as output by accident and only set LED matrix pins
    NRF_P0->DIRSET = LED_P0_MASK; // All GPIO on Port 0 as outputs
    NRF_P1->DIRSET = LED_P1_MASK; // Port 1: make P1.05 an output
    // turn off everything
    for (int i = 0; i < 5; i++) {
      set_high(col_pins[i]); // Default HIGH (OFF)
      set_low(row_pins[i]);  // Default LOW (OFF)
    }
  }

  static void led_on(uint32_t row, uint32_t col) {
    set_low(col_pins[col]);  // Turn it ON
    set_high(row_pins[row]); // Turn it ON
  }
  static void led_off(uint32_t row, uint32_t col) {
    set_high(col_pins[col]); // Turn it OFF
    set_low(row_pins[row]);  // Turn it OFF
  }

private:
  static void set_high(gpio_t gpio) {
    if (gpio.port == 0) {
      NRF_P0->OUTSET = BIT(gpio.pin);
      return;
    }
    NRF_P1->OUTSET = BIT(gpio.pin);
  }

  static void set_low(gpio_t gpio) {
    if (gpio.port == 0) {
      NRF_P0->OUTCLR = BIT(gpio.pin);
      return;
    }
    NRF_P1->OUTCLR = BIT(gpio.pin);
  }
};

#endif // MB2_FREERTOS_LEDMATRIC_H