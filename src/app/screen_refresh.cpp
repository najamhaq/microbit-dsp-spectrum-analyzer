//
// Created by najamhaq on 2026-01-26.
//

#ifndef DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_AUDIO_TASK_H
#define DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_AUDIO_TASK_H

#include "screen_refresh.h"

static constexpr uint32_t px(uint32_t r, uint32_t c) { return 1u << (r * 5u + c); }
static LedMatrix g_leds;

void ScreenRefresh::init() {

  constexpr uint32_t kCheck = px(1, 4) | px(2, 3) | px(3, 0) | px(3, 2) | px(4, 1);
  g_leds.init();
  g_leds.setFrame(kCheck);
}

void ScreenRefresh::run() {
  TickType_t last = xTaskGetTickCount();
  uint32_t counter = 0u;
  uint32_t frame = g_leds.getFrame();
  for (;;) {
#if !REFRESH_VIA_HARDWARE_TIMER
    g_leds.isrTick();
#endif
    vTaskDelayUntil(&last, pdMS_TO_TICKS(1));
    auto seconds = counter++ / 1000u;
    if (counter % 1000 == 0) {
      if (seconds % 2 == 0) {
        frame = g_leds.getFrame();
        g_leds.setFrame(0);
        continue;
      }
      if (frame) {
        g_leds.setFrame(frame); // put frame back
      }
    }
  }
}

void ScreenRefresh::stop() {}
void ScreenRefresh::resume() {}

#endif // DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_AUDIO_TASK_H
