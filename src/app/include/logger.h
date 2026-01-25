//
// Created by najamhaq on 2026-01-24.
//

#ifndef MICROBIT_FREERTOS_SRC_APP_INCLUDE_LOGGER_H
#define MICROBIT_FREERTOS_SRC_APP_INCLUDE_LOGGER_H

#include <cstddef>

void logger_init();                             // creates queue + starts UART task
void log_write(const char* s);                  // enqueue a null-terminated string
void log_write_n(const char* s, std::size_t n); // enqueue raw bytes

#endif // MICROBIT_FREERTOS_SRC_APP_INCLUDE_LOGGER_H
