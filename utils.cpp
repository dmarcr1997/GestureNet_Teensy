#include "utils.h"

float randomf(float min, float max) {
  return min + ((float)random(10000) / 10000.0f) * (max - min);
}