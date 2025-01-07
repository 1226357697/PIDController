#pragma once
class bezier
{
public:
  // ���Բ�ֵ
  float lerp(float p0, float p1, float t) ;

  // ���α�������ֵ
  float quadratic_bezier(float p0, float p1, float p2, float t);

  // ���α�������ֵ
  float cubic_bezier(float p0, float p1, float p2, float p3, float t);

  void setTarget(float target);
  

  void setMethod(int method);
  float run(float value);

private:
  int method_;
  float target_;
};

