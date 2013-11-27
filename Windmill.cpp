#include "Windmill.h"

Windmill::Windmill (const vec3& pos)
{
  baseRotation = 0;
  bladeRotation = 0;
  position = pos;
}

Windmill::~Windmill ()
{

}

mat4 Windmill::getBaseTransform ()
{
  return baseTransform; 
}

mat4 Windmill::getBladeTransform (int i)
{
  return bladeTransform[i];
}

void Windmill::setBaseRotation (float r)
{
  baseRotation = r;
}

void Windmill::rotateBase (float amt)
{
  rotate (baseRotation, amt);
}

void Windmill::rotateBlade (float amt)
{
  rotate (bladeRotation, amt);
}

void Windmill::reset ()
{
  baseRotation = 0;
  bladeRotation = 0;
}

void Windmill::generateBaseTransform ()
{
  baseTransform = mat4(1.0);
  baseTransform *= Translate (position.x, position.y, position.z);
  baseTransform *= Translate (0, -.5, 0);
  baseTransform *= RotateY (baseRotation);
  baseTransform *= Scale (5.0, 50.0, 5.0);
}

void Windmill::generateBladeTransform ()
{
  for (int i=0; i < 4; i++){
    bladeTransform[i] = mat4(1.0);
    bladeTransform[i] *= Translate (position.x, position.y, position.z);
    bladeTransform[i] *= RotateY (baseRotation);
    bladeTransform[i] *= RotateZ (bladeRotation+(90.0*i));
    bladeTransform[i] *= Translate (0, 0.5, -0.1);
    bladeTransform[i] *= Scale (7.0, 40.0, 2.0);
  }
}
