#include "gesture_predictor.h"

#include "constants.h"

//#include <TensorFlowLite.h>
//#include "tensorflow/lite/micro/micro_error_reporter.h"

int PredictGesture(tflite::ErrorReporter* error_reporter, float* output) {

  TF_LITE_REPORT_ERROR(
        error_reporter,
        "Output vector below:\n");

  int best_i = -1;
  float best_value = -999;
  for(int i = 0; i < kGestureCount; i++) {
      if(output[i] > best_value) {
          best_i = i;
          best_value = output[i];
      }
      /*TF_LITE_REPORT_ERROR(
        error_reporter,
        "%d -> %.5f \n", i, output[i]);*/
  }

  

  if(best_value > kDetectionThreshold) {
    return best_i; 
  } else return -1; 
  

  
  
}
