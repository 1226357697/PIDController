#pragma once
class bezier
{
public:
  // 线性插值
  float lerp(float p0, float p1, float t) ;

  // 二次贝塞尔插值
  float quadratic_bezier(float p0, float p1, float p2, float t);

  // 三次贝塞尔插值
  float cubic_bezier(float p0, float p1, float p2, float p3, float t);

  void setTarget(float target);
  

  void setMethod(int method);
  float run(float value);

private:
  int method_;
  float target_;
};

