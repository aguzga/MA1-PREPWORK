#include <TensorFlowLite.h>

#include "main_functions.h"

#include "accelerometer_handler.h"
#include "constants.h"
#include "gesture_predictor.h"
#include "model_tiny_gym.h"
#include "output_handler.h"

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

//globals used for arduino ide compatibility with sketches
namespace {
  tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* model_input = nullptr;
  int input_length;

  //memory to be used for input, output and intermediate arrays in inference
  //size determined through experimentation
  constexpr int kTensorArenaSize = 80*1024;
  uint8_t tensor_arena[kTensorArenaSize];
} //namespace done

void setup() {
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  model = tflite::GetModel(gym_model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter, "Model is schema version %d and"
                                         "it's not equal to supported version %d.",
                                         model->version(), TFLITE_SCHEMA_VERSION);}
    return;               
  }

  // Pull in only the operation implementations we need.
  // This relies on a complete list of all the ops needed by this graph.
  // An easier approach is to just use the AllOpsResolver, but this will
  // incur some penalty in code space for op implementations that are not
  // needed by this graph.


  TfLiteStatus tflite_status;
  
  
  static tflite::MicroMutableOpResolver<7> micro_op_resolver;  // NOLINT
  tflite_status = micro_op_resolver.AddConv2D();
  if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "conv2d failed");}
  }
  tflite_status = micro_op_resolver.AddDepthwiseConv2D();
  if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "depthwise conv2d failed");}
  }
  tflite_status = micro_op_resolver.AddFullyConnected();
  if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "fully connected failed");}
  }
  tflite_status = micro_op_resolver.AddMaxPool2D();
  if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "maxpool failed");}
  }
   tflite_status = micro_op_resolver.AddRelu();
   if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "Relu failed");}
  }
  tflite_status = micro_op_resolver.AddReshape(); 
  if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "reshape failed");}
  }
  tflite_status = micro_op_resolver.AddSoftmax();
  if (tflite_status != kTfLiteOk)
  {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "softmax failed");}
  }

   
                                                                                                                                            
  
  //build interpreter for model to be ran
  static tflite::MicroInterpreter static_interpreter(model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  //Allocate memory for model's tensors from tensor_arena
  /*while(1)*/ TfLiteStatus alloc_status = interpreter->AllocateTensors();
  if(alloc_status != kTfLiteOk) {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "Tensor alloc failed");}
  }

  // Obtain pointer to the model's input tensor.
  model_input = interpreter->input(0);
  if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
      (model_input->dims->data[1] != 300) ||
      (model_input->dims->data[2] != kChannelNumber) ||
      (model_input->type != kTfLiteFloat32)) {
    while(1) {TF_LITE_REPORT_ERROR(error_reporter,
                         "Bad input tensor parameters in model");}
    return;
  }

  input_length = model_input->bytes / sizeof(float);

  TfLiteStatus setup_status = SetupAccelerometer(error_reporter);
  if (setup_status != kTfLiteOk) {
    
    /*while(1)*/ TF_LITE_REPORT_ERROR(error_reporter, "Set up failed\n");
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  // Attempt to read new data from the accelerometer.
  bool got_data =
      ReadAccelerometer(error_reporter, model_input->data.f, input_length);
  // If there was no new data, wait until next time.
  if (!got_data) return;

  

  // Run inference, and report any error.
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    /*while(1)*/ TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on index: %d\n",
                         begin_index);
    
    return;
  }
  // Analyze the results to obtain a prediction
  int gesture_index = PredictGesture(error_reporter, interpreter->output(0)->data.f);

  for(int i = 0; i < kGestureCount; i++) {
      Serial.println(interpreter->output(0)->data.f[i]);
      /*TF_LITE_REPORT_ERROR(
        error_reporter,
        "%d -> %.5f \n", i, output[i]);*/
  }

  // Produce an output
  HandleOutput(error_reporter, gesture_index);
}
