//
// Created by najamhaq on 2026-01-25.
//

#ifndef DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_AUDIO_TASK_H
#define DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_AUDIO_TASK_H

#include "app_task.h"
class AudioTask : public AppTask {
public:
  void init() override;
  void run() override;
  void resume() override;
  void stop() override;
};
;
#endif // DSP_SPECTRUM_ANALYZER_SRC_APP_INCLUDE_AUDIO_TASK_H
