//
// Created by mnajm on 2026-01-25.
//

#include "audio_capture.h"

// Later we’ll include nRF headers here, e.g.
// #include "nrf.h"

AudioCapture* AudioCapture::s_instance = nullptr;

void AudioCapture::init() {
  s_instance = this;
  configureSaadc_();
  configureTimerPpi_();
}

void AudioCapture::start(FrameReadyFn cb, void* user) {
  cb_ = cb;
  user_ = user;

  // TODO:
  // - Prime first DMA buffer (buf0_)
  // - Enable SAADC END interrupt
  // - Start SAADC and TIMER
}

void AudioCapture::stop() {
  // TODO:
  // - Stop TIMER
  // - Stop SAADC
  // - Disable interrupts
}

void AudioCapture::isrThunk() {
  if (s_instance) {
    s_instance->onSaadcEndIsr();
  }
}

void AudioCapture::onSaadcEndIsr() {
  // TODO:
  // - Clear SAADC END event
  // - Swap buffers and re-arm DMA to the other buffer
  // - Call cb_(readyBuf, kFrameSize, user_) if set
}

void AudioCapture::configureSaadc_() {
  // TODO:
  // - Configure SAADC channel for micro:bit v2 MIC_IN (P0.05 / AIN3)
  // - Set resolution, gain, reference, acquisition time
}

void AudioCapture::configureTimerPpi_() {
  // TODO:
  // - Configure TIMER to tick at 8 kHz
  // - Configure PPI: TIMER compare event -> SAADC TASK_SAMPLE
}
