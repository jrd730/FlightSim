#include "Windmill.h"

Windmill::Windmill ()
{
  baseRotation = 0;
  bladeRotation = 0;
}

Windmill::~Windmill ()
{

}

static void rotate (float& angle, float amount)
{
  angle += amount;
  if (amount > 0 && angle > 360.0) angle -= 360.0;
  else if (amount < 0 && angle < 0) angle += 360.0;
}

mat4 Windmill::getBaseTransform ()
{
  return baseTransform; 
}

mat4 Windmill::getBladeTransform (int i)
{
  return bladeTransform[i];
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
  baseTransform *= Translate (0, -.5, 0);
  baseTransform *= RotateY (baseRotation);
  baseTransform *= Scale (5.0, 50.0, 5.0);
}

void Windmill::generateBladeTransform ()
{
  for (int i=0; i < 4; i++){
    bladeTransform[i] = mat4(1.0);
    bladeTransform[i] *= RotateY (baseRotation);
    bladeTransform[i] *= RotateZ (bladeRotation+(90.0*i));
    bladeTransform[i] *= Translate (0, 0.5, -0.1);
    bladeTransform[i] *= Scale (7.0, 40.0, 2.0);
  }
}
