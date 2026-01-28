//
// Created by najamhaq on 2026-01-26.
//

#include "led_matrix.h"
#include "nrf.h"

#define BIT(n) (1u << (n))
#define REFRESH_VIA_HARDWARE_TIMER 1

typedef struct {
  uint8_t port; // 0 or 1
  uint8_t pin;  // 0..31 (port0) or 0..15 (port1)
} gpio_t;

static const gpio_t col_pins[5] = {{0, 28}, {0, 11}, {0, 31}, {1, 5}, {0, 30}};
static const gpio_t row_pins[5] = {{0, 21}, {0, 22}, {0, 15}, {0, 24}, {0, 19}};

static LedMatrix* s_led = nullptr;
volatile uint32_t g_timer4_hits = 0;
extern "C" void TIMER4_IRQHandler(void) {
  if (NRF_TIMER4->EVENTS_COMPARE[0] != 0) {
    g_timer4_hits++;
    NRF_TIMER4->EVENTS_COMPARE[0] = 0;
    (void)NRF_TIMER4->EVENTS_COMPARE[0]; // optional read-back
    if (s_led) {
      s_led->isrTick();
    }
  }
}

void LedMatrix::init() {
  gpioInit_();
  s_led = this;
#if REFRESH_VIA_HARDWARE_TIMER
  timerInit_();
#endif
  clear();
}

void LedMatrix::setFrame(uint32_t bits25) {
  frame_ = (bits25 & 0x01FFFFFFu); // 25 bits
}

void LedMatrix::clear() { frame_ = 0; }

void LedMatrix::gpioInit_() {
  // Precompute per-pin masks and combined masks by port.
  for (int i = 0; i < 5; i++) {
    row_p0_bit_[i] = (row_pins[i].port == 0) ? BIT(row_pins[i].pin) : 0;
    row_p1_bit_[i] = (row_pins[i].port == 1) ? BIT(row_pins[i].pin) : 0;
    col_p0_bit_[i] = (col_pins[i].port == 0) ? BIT(col_pins[i].pin) : 0;
    col_p1_bit_[i] = (col_pins[i].port == 1) ? BIT(col_pins[i].pin) : 0;

    rows_p0_mask_ |= row_p0_bit_[i];
    rows_p1_mask_ |= row_p1_bit_[i];
    cols_p0_mask_ |= col_p0_bit_[i];
    cols_p1_mask_ |= col_p1_bit_[i];
  }

  // Configure all row/col pins as outputs.
  NRF_P0->DIRSET = rows_p0_mask_ | cols_p0_mask_;
  NRF_P1->DIRSET = rows_p1_mask_ | cols_p1_mask_;

  // Default OFF state:
  // rows OFF = low, cols OFF = high
  NRF_P0->OUTCLR = rows_p0_mask_;
  NRF_P1->OUTCLR = rows_p1_mask_;
  NRF_P0->OUTSET = cols_p0_mask_;
  NRF_P1->OUTSET = cols_p1_mask_;
}

void LedMatrix::timerInit_() {
  // Stop + clear first
  NRF_TIMER4->TASKS_STOP = 1;
  NRF_TIMER4->TASKS_CLEAR = 1;

  // Configure
  NRF_TIMER4->MODE = TIMER_MODE_MODE_Timer;
  NRF_TIMER4->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
  NRF_TIMER4->PRESCALER = 4; // 1 MHz tick (1 us)
  NRF_TIMER4->CC[0] = 1000;  // 1 ms
  NRF_TIMER4->SHORTS = TIMER_SHORTS_COMPARE0_CLEAR_Msk;

  // Clear event *before* enabling interrupt
  NRF_TIMER4->EVENTS_COMPARE[0] = 0;

  // Enable interrupt source
  NRF_TIMER4->INTENSET = TIMER_INTENSET_COMPARE0_Msk;

  // NVIC: clear pending then enable
  NVIC_ClearPendingIRQ(TIMER4_IRQn);
  NVIC_SetPriority(TIMER4_IRQn, 2);
  NVIC_EnableIRQ(TIMER4_IRQn);

  // Start
  NRF_TIMER4->TASKS_START = 1;
}

void LedMatrix::isrTick() {

  // Turn everything OFF first (avoid ghosting).
  NRF_P0->OUTCLR = rows_p0_mask_;
  NRF_P1->OUTCLR = rows_p1_mask_;
  NRF_P0->OUTSET = cols_p0_mask_;
  NRF_P1->OUTSET = cols_p1_mask_;

  const uint8_t r = row_;
  const uint32_t f = frame_;
  const uint32_t rowBits = (f >> (r * 5)) & 0x1Fu; // 5 bits for this row
                                                   //
  // For each col that should be ON: drive that col LOW.
  uint32_t clr_p0 = 0, clr_p1 = 0;
  for (int c = 0; c < 5; c++) {
    if (rowBits & (1u << c)) {
      clr_p0 |= col_p0_bit_[c];
      clr_p1 |= col_p1_bit_[c];
    }
  }
  if (clr_p0)
    NRF_P0->OUTCLR = clr_p0;
  if (clr_p1)
    NRF_P1->OUTCLR = clr_p1;

  // Enable selected row HIGH.
  if (row_p0_bit_[r])
    NRF_P0->OUTSET = row_p0_bit_[r];
  if (row_p1_bit_[r])
    NRF_P1->OUTSET = row_p1_bit_[r];

  row_ = (r + 1) % 5; // row increment member but roll back
}

void LedMatrix::setPixel(uint8_t row, uint8_t col, bool on) {
  const uint32_t bit = 1u << (row * 5 + col);
  if (on)
    frame_ |= bit;
  else
    frame_ &= ~bit;
}
