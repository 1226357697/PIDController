#include "bezier.h"

float bezier::lerp(float p0, float p1, float t)
{
  return p0 + t * (p1 - p0);
}

float bezier::quadratic_bezier(float p0, float p1, float p2, float t)
{
  return (1 - t) * (1 - t) * p0 + 2 * (1 - t) * t * p1 + t * t * p2;
}

float bezier::cubic_bezier(float p0, float p1, float p2, float p3, float t)
{
  return (1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * p1 + 3 * (1 - t) * t * t * p2 + t * t * t * p3;
}

void bezier::setTarget(float target)
{
  target_ = target;
}

void bezier::setMethod(int method)
{
  method_ = method;
}

float bezier::run(float value)
{
  float result = 0;
  float t = 0.4;
  if (method_ == 1)
  {
    result = lerp(value, target_, t);
  }
  else if (method_ == 2)
  {
    float control_point = (value + target_) / 2;  // ¿ØÖÆµã
    result = quadratic_bezier(value, control_point, target_, t);
  }
  else if (method_ == 3)
  {
    float control_point1 = value + (target_ - value) / 3;
    float control_point2 = target_ - (target_ - value) / 3;
    result = cubic_bezier(value, control_point1, control_point2, target_, t);
  }
  return result;
}
