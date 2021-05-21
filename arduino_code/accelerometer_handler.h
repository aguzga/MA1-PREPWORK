#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_ACCELEROMETER_HANDLER_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_ACCELEROMETER_HANDLER_H_

#define kChannelNumber 6

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"

extern int begin_index;
extern TfLiteStatus SetupAccelerometer(tflite::ErrorReporter* error_reporter);
extern bool ReadAccelerometer(tflite::ErrorReporter* error_reporter,
                              float* input, int length);

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_ACCELEROMETER_HANDLER_H_
