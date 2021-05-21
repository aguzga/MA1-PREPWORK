#if defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)
#define ARDUINO_EXCLUDE_CODE
#endif  // defined(ARDUINO) && !defined(ARDUINO_ARDUINO_NANO33BLE)

#ifndef ARDUINO_EXCLUDE_CODE

#include "accelerometer_handler.h"

#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

#include "constants.h"


int begin_index = 0;


TfLiteStatus SetupAccelerometer(tflite::ErrorReporter* error_reporter) {
  // Switch on the IMU
  if (!IMU.begin()) {
    TF_LITE_REPORT_ERROR(error_reporter, "Failed to initialize IMU");
    return kTfLiteError;
  }


  IMU.setContinuousMode();


  TF_LITE_REPORT_ERROR(error_reporter, "Magic starts!");

  return kTfLiteOk;
}

bool ReadAccelerometer(tflite::ErrorReporter* error_reporter, float* input,
                       int length) {

    const float accelerationThreshold = 1.6;
    float aX, aY, aZ, gX, gY, gZ;
    float save_data[1800];
    int begin_index = 0;
    const int numSamples = 300;
    int samplesRead = numSamples;
    // wait for significant motion
    while (samplesRead == numSamples) {
      if (IMU.accelerationAvailable()) {
        // read the acceleration data
        IMU.readAcceleration(aX, aY, aZ);
  
        // sum up the absolutes
        float aSum = fabs(aX) + fabs(aY) + fabs(aZ);
  
        // check if it's above the threshold
        if (aSum >= accelerationThreshold) {
          // reset the sample read count
          samplesRead = 0;
          break;
        }
      }
    }

    Serial.println("Getting input data...");

    while (samplesRead < numSamples) {
      // check if both new acceleration and gyroscope data is
      // available
      if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
        // read the acceleration and gyroscope data
        IMU.readAcceleration(aX, aY, aZ);
        IMU.readGyroscope(gX, gY, gZ);
  
        samplesRead++;
  
        save_data[begin_index++] = aX;
        save_data[begin_index++] = aY;
        save_data[begin_index++] = aZ;
        save_data[begin_index++] = gX;
        save_data[begin_index++] = gY;
        save_data[begin_index++] = gZ;
  
        if(begin_index == length) {
          begin_index = 0;
        }
      }
    }

    TF_LITE_REPORT_ERROR(
        error_reporter,
        "Done reading data\n");
    for(int i = 0; i < length; i++) {
      input[i] = save_data[i];
    }

    return true;

}

#endif  // ARDUINO_EXCLUDE_CODE
