
#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "output_handler.h"

#include "Arduino.h"

void HandleOutput(tflite::ErrorReporter* error_reporter, int kind) {
  // The first time this method runs, set up our LED
  static bool is_initialized = false;
  if (!is_initialized) {
    pinMode(LED_BUILTIN, OUTPUT);
    is_initialized = true;
  }

  // Print some ASCII art for each gesture and control the LED.
  if (kind == 0) {
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "BICEPS movement detected\n\r");
  } else if (kind == 1) {
    digitalWrite(LED_BUILTIN, HIGH);
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "SHOULDERS movement detected\n\r");
  } else if (kind == 2) {
    digitalWrite(LED_BUILTIN, LOW);
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "NO REAL movement detected\n\r");
  } else if (kind == -1) {
    TF_LITE_REPORT_ERROR(
        error_reporter,
        "Not confident enough.\n\r");
  }
}

#endif  // ARDUINO_EXCLUDE_CODE
