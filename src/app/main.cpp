extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

/////////////////////////
#include "audio_task.h"
#include "screen_refresh.h"

AudioTask g_audioTask;
ScreenRefresh g_screenRefresh;

static void ScreenRefreshRun(void* arg) {

  for (;;) {
    g_screenRefresh.run();
    // Put a breakpoint here and clap/talk: s should change.
  }
}

static void AudioRun(void* arg) {
  g_audioTask.init();
  g_audioTask.run();
}
void task_setup() {
  g_screenRefresh.init();
  g_audioTask.init();
  g_audioTask.setScreenRefresh(&g_screenRefresh);
  //
  xTaskCreate(AudioRun, "Audio", 256, nullptr, tskIDLE_PRIORITY + 3, nullptr);
  xTaskCreate(ScreenRefreshRun, "LED", 256, nullptr, tskIDLE_PRIORITY + 3, nullptr);
}

extern "C" int main(void) {
  task_setup();
  vTaskStartScheduler();
}
