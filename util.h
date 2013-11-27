#pragma once

static float randomFloat () {
  return ((float)rand()) / ((float)RAND_MAX);
}

static void rotate (float& angle, float amount)
{
  angle += amount;
  if (amount > 0 && angle > 360.0) angle -= 360.0;
  else if (amount < 0 && angle < 0) angle += 360.0;
}