//
// Created by mnajm on 2026-01-26.
//

#ifndef DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_SCREEN_REFRESH_H
#define DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_SCREEN_REFRESH_H

#include "app_task.h"
#include "led_matrix.h"

class ScreenRefresh : public AppTask {
public:
  ScreenRefresh() { _ledMatrix.init(); }

  void init() override;
  void run() override;
  void resume() override;
  void stop() override;

private:
  LedMatrix _ledMatrix;
};

#endif // DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_SCREEN_REFRESH_H
