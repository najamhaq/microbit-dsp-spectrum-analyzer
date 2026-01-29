//
// Created by najamhaq on 2026-01-25.
//

#include "audio_task.h"
#include "nrf.h"
#include "screen_refresh.h"

#include <stddef.h>
#include <stdint.h>

static volatile int16_t g_saadc_sample = 0;
static inline void mic_power_on() {
  // RUN_MIC = P0.20
  NRF_P0->DIRSET = (1u << 20);
  NRF_P0->OUTSET = (1u << 20);
  // small settle delay (optional but helpful)
  for (volatile int i = 0; i < 10000; i++)
    __asm volatile("nop");
}

static inline void saadc_init_mic_ain3() {
  // Enable SAADC
  NRF_SAADC->ENABLE = (SAADC_ENABLE_ENABLE_Enabled << SAADC_ENABLE_ENABLE_Pos);

  // Optional: run offset calibration once after power-up
  NRF_SAADC->TASKS_CALIBRATEOFFSET = 1;
  while (NRF_SAADC->EVENTS_CALIBRATEDONE == 0) { /* wait */
  }
  NRF_SAADC->EVENTS_CALIBRATEDONE = 0;
  NRF_SAADC->TASKS_STOP = 1;
  while (NRF_SAADC->EVENTS_STOPPED == 0) { /* wait */
  }
  NRF_SAADC->EVENTS_STOPPED = 0;

  // Configure channel 0: PSELP = AIN3 (P0.05), single-ended
  NRF_SAADC->CH[0].PSELP = (SAADC_CH_PSELP_PSELP_AnalogInput3 << SAADC_CH_PSELP_PSELP_Pos);
  NRF_SAADC->CH[0].PSELN = (SAADC_CH_PSELN_PSELN_NC << SAADC_CH_PSELN_PSELN_Pos);

  // Channel config:
  // - Gain 1/4 and VDD/4 reference gives a decent input range for many mic modules.
  // - Tacq 10us is safe.
  NRF_SAADC->CH[0].CONFIG = (SAADC_CH_CONFIG_RESP_Bypass << SAADC_CH_CONFIG_RESP_Pos) |
                            (SAADC_CH_CONFIG_RESN_Bypass << SAADC_CH_CONFIG_RESN_Pos) |
                            (SAADC_CH_CONFIG_GAIN_Gain1_4 << SAADC_CH_CONFIG_GAIN_Pos) |
                            (SAADC_CH_CONFIG_REFSEL_VDD1_4 << SAADC_CH_CONFIG_REFSEL_Pos) |
                            (SAADC_CH_CONFIG_TACQ_10us << SAADC_CH_CONFIG_TACQ_Pos) |
                            (SAADC_CH_CONFIG_MODE_SE << SAADC_CH_CONFIG_MODE_Pos) |
                            (SAADC_CH_CONFIG_BURST_Disabled << SAADC_CH_CONFIG_BURST_Pos);

  // 12-bit resolution (stored in 16-bit sample)
  NRF_SAADC->RESOLUTION = (SAADC_RESOLUTION_VAL_12bit << SAADC_RESOLUTION_VAL_Pos);

  // Result buffer (DMA)
  NRF_SAADC->RESULT.PTR = (uint32_t)&g_saadc_sample;
  NRF_SAADC->RESULT.MAXCNT = 1;
}

static inline int16_t saadc_read_one() {
  NRF_SAADC->EVENTS_END = 0;

  NRF_SAADC->TASKS_START = 1;
  NRF_SAADC->TASKS_SAMPLE = 1;

  while (NRF_SAADC->EVENTS_END == 0) { /* wait */
  }
  NRF_SAADC->EVENTS_END = 0;

  NRF_SAADC->TASKS_STOP = 1;
  while (NRF_SAADC->EVENTS_STOPPED == 0) { /* wait */
  }
  NRF_SAADC->EVENTS_STOPPED = 0;

  return g_saadc_sample;
}

void AudioTask::init() {
  mic_power_on();
  saadc_init_mic_ain3();
}
void AudioTask::run() {
  TickType_t last = xTaskGetTickCount();
  // uint32_t counter = 0u;
  //  uint32_t frame = g_leds.getFrame();
  for (;;) {
    volatile int16_t s = saadc_read_one();
    if (p_screen_refresh_) {
      p_screen_refresh_->setFrame(s);
    }
    vTaskDelayUntil(&last, pdMS_TO_TICKS(1));
  }
}
void AudioTask::resume() {}
void AudioTask::setScreenRefresh(ScreenRefresh* p_screen_refresh) {
  p_screen_refresh_ = p_screen_refresh;
}
void AudioTask::stop() {}
