
#pragma once
#include "Angel.h"
#include "Colors.h"
#include "util.h"
class Windmill 
{
public:
  Windmill (const vec3& pos=vec3(0, 0, 0));
  ~Windmill ();

  mat4 getBaseTransform ();
  mat4 getBladeTransform (int i);

  void setBaseRotation (float r);
  void rotateBase (float amt);
  void rotateBlade (float amt);

  void reset ();

  void generateBaseTransform ();
  void generateBladeTransform ();

private:
  float baseRotation;
  float bladeRotation;

  vec3 position;
  mat4 baseTransform;
  mat4 bladeTransform[4];

  GLuint vao;
};