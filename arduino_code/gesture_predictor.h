#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_GESTURE_PREDICTOR_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_GESTURE_PREDICTOR_H_
#include <TensorFlowLite.h>
#include "tensorflow/lite/micro/micro_error_reporter.h"
extern int PredictGesture(tflite::ErrorReporter* error_reporter, float* output);

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_GESTURE_PREDICTOR_H_
