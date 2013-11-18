
#pragma once
#include "Angel.h"
#include "Colors.h"
class Windmill 
{
public:
  Windmill ();
  ~Windmill ();

  mat4 getBaseTransform ();
  mat4 getBladeTransform (int i);

  void rotateBase (float amt);
  void rotateBlade (float amt);

  void reset ();

  void generateBaseTransform ();
  void generateBladeTransform ();

private:
  float baseRotation;
  float bladeRotation;

  mat4 baseTransform;
  mat4 bladeTransform[4];

  GLuint vao;
  //vec3 position;
};