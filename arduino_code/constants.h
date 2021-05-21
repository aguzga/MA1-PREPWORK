#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_CONSTANTS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_CONSTANTS_H_

// The expected accelerometer data sample frequency
const float kTargetHz = 119;

// What gestures are supported.
constexpr int kGestureCount = 3;
constexpr int kBicepsGesture = 0;
constexpr int kShouldersGesture = 1;
constexpr int kNoGesture = 2;

constexpr float kDetectionThreshold = 0.5;

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_MAGIC_WAND_CONSTANTS_H_
