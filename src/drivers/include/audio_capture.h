//
// Created by najamhaq on 2026-01-25.
//

#ifndef DSP_SPECTRUM_ANALYZER_SRC_DRIVERS_INCLUDE_AUDIO_CAPTURE_H
#define DSP_SPECTRUM_ANALYZER_SRC_DRIVERS_INCLUDE_AUDIO_CAPTURE_H

#include <cstddef>
#include <cstdint>

class AudioCapture {
public:
  static constexpr uint32_t kSampleRateHz = 8000;
  static constexpr size_t kFrameSize = 256;

  // Callback signature (called from ISR context!)
  using FrameReadyFn = void (*)(const int16_t* samples, size_t n, void* user);

  AudioCapture() = default;
  AudioCapture(const AudioCapture&) = delete;
  AudioCapture& operator=(const AudioCapture&) = delete;

  // Configure hardware. Safe to call before scheduler starts.
  void init();

  // Starts sampling. cb is invoked from ISR when a 256-sample frame is ready.
  void start(FrameReadyFn cb, void* user);

  // Stops sampling.
  void stop();

private:
  // ISR entry (C-linkage in the .cpp) will call this.
  static AudioCapture* s_instance;

  // ISR handler forwarder
  static void isrThunk();

  // Real ISR work
  void onSaadcEndIsr();

  FrameReadyFn cb_ = nullptr;
  void* user_ = nullptr;

  // Ping-pong buffers
  int16_t buf0_[kFrameSize]{};
  int16_t buf1_[kFrameSize]{};
  bool use_buf0_ = true;

  // Helpers for configuring SAADC/TIMER/PPI (implemented in .cpp)
  void configureSaadc_();
  void configureTimerPpi_();
};

#endif // DSP_SPECTRUM_ANALYZER_SRC_DRIVERS_INCLUDE_AUDIO_CAPTURE_H
