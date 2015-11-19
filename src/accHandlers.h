#include "global.h"

#ifndef ACCHANDLERS_H
#define ACCHANDLERS_H

static void accel_tap_handler(AccelAxisType axis, int32_t direction){
  switch(axis){
  case ACCEL_AXIS_X:
    text_layer_set_text(acc_layer, "x tapped");
    break;
  case ACCEL_AXIS_Y:
    text_layer_set_text(acc_layer, "y tapped");
    break;
  case ACCEL_AXIS_Z:
    text_layer_set_text(acc_layer, "z tapped");
    break;
  }
}

static void accel_raw_handler(AccelData *data, uint32_t num_samples)
{
  static char buffer[] = "XYZ: 9999 / 9999 / 9999";
  snprintf(buffer, sizeof("XYZ: 9999 / 9999 / 9999"), "XYZ: %d / %d / %d", data[0].x, data[0].y, data[0].z);
  text_layer_set_text(acc_raw_layer, buffer);
}

#endif
