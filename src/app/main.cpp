extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

#include "nrf.h"
/////////////////////////
#include "audio_task.h"
#include "screen_refresh.h"

AudioTask g_audioTask;
ScreenRefresh g_screenRefresh;

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

static void ScreenRefreshRun(void* arg) {

  mic_power_on();
  saadc_init_mic_ain3();

  for (;;) {
    volatile int16_t s = saadc_read_one();
    (void)s;
    g_screenRefresh.run();
    // Put a breakpoint here and clap/talk: s should change.
  }
}

static void AudioRun(void* arg) {
  g_audioTask.init();
  g_audioTask.run();
}
void task_setup() {
  //  g_audioTask.init();
  g_screenRefresh.init();
  //
  //  xTaskCreate(AudioRun, "Audio", 256, nullptr, tskIDLE_PRIORITY + 3, nullptr);
  xTaskCreate(ScreenRefreshRun, "LED", 256, nullptr, tskIDLE_PRIORITY + 3, nullptr);
}

extern "C" int main(void) {
  task_setup();
  vTaskStartScheduler();
}
