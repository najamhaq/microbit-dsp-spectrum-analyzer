//
// Created by najamhaq on 2026-01-26.
//

#ifndef DSP_SPECTRUM_ANALYZER_SRC_DRIVERS_LED_MATRIX_H
#define DSP_SPECTRUM_ANALYZER_SRC_DRIVERS_LED_MATRIX_H
#pragma once
#include <cstdint>

class LedMatrix {
public:
  void init();
  uint32_t getFrame() const { return frame_; }
  void setFrame(uint32_t bits25); // bit = row*5 + col
  void setPixel(uint8_t row, uint8_t col, bool on);
  void clear();
  void isrTick(); // call from timer IRQ

private:
  volatile uint32_t frame_ = 0;
  uint8_t row_ = 0;

  // Precomputed port masks for quick writes
  uint32_t rows_p0_mask_ = 0;
  uint32_t rows_p1_mask_ = 0;
  uint32_t cols_p0_mask_ = 0;
  uint32_t cols_p1_mask_ = 0;

  uint32_t row_p0_bit_[5]{};
  uint32_t row_p1_bit_[5]{};
  uint32_t col_p0_bit_[5]{};
  uint32_t col_p1_bit_[5]{};

  void gpioInit_();
  void timerInit_(); // 1kHz row-scan timer + IRQ
};

#endif // DSP_SPECTRUM_ANALYZER_SRC_DRIVERS_LED_MATRIX_H
