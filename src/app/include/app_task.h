//
// Created by najamhaq  on 2026-01-25.
//

#ifndef DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_TASK_H
#define DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_TASK_H

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

class AppTask {

public:
  //  virtual ~AppTask() = default;

  virtual void init() = 0;
  virtual void run() = 0;
  virtual void resume() = 0;
  virtual void stop() = 0;

protected:
  TaskHandle_t taskHandle_ = nullptr;
};

#endif // DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_TASK_H
