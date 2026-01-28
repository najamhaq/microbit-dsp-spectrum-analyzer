extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

#include "nrf.h"
/////////////////////////
#include "audio_task.h"
#include "screen_refresh.h"

// AudioTask g_audioTask;
ScreenRefresh g_screenRefresh;

static void ScreenRefreshRun(void* arg) { g_screenRefresh.run(); }

// static void AudioRun(void* arg) {
//   g_audioTask.run();
// }
//
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
