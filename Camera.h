
// Simulates a camera moving through a 3d space and maintains a view 
// transformation matrix for use in the vertex shader.
// Abstracts the complexities of performing simple operations to adjust
// the camera's position and direction within a volume.

const static vec4 baseLook (0, 0, 1, 1);
const static vec4 baseEye (0, 0, 0, 1);
const static vec4 baseUp (0, 1, 0, 0);

class Camera
{
  public:
    Camera (const vec4& pos, float rx, float ry, float rz) :
      eye (pos), x_rot (rx), y_rot (ry), z_rot (rz),
      viewMatrix (1.0)
    {
      resetViewMatrix ();
    }

    ~Camera ()
    {
      
    }

    void resetViewMatrix ()
    {
      look = baseLook;
      eye = baseEye;
      up = baseUp;

      x_rot = 0;
      y_rot = 0;
      z_rot = 0;

      viewMatrix = LookAt (eye, look, up);
    }

    void setEye (const vec4& p) { eye = p; }
    void setLook (const vec4& l){ look = l; }
    void setUp (const vec4& u)  { up = u; }

    const vec4& getEye () { return eye; }
    const vec4& getLook () { return look; }
    const vec4& getUp () { return up; }

    void setXRot (float rx) { x_rot = rx; }
    void setYRot (float ry) { y_rot = ry; }
    void setZRot (float rz) { z_rot = rz; }

    void rotate (float& angle, float amount, float max=360.0)
    {
      angle += amount;
      if (amount > 0 && angle > max) angle -= max;
      else if (amount < 0 && angle < 0) angle += max;
    }

    void rotateX (float amount)
    {    
      rotate (x_rot, amount);
      look = eye + (mvmult (RotateX (amount), ( look - eye ) ) );
      up = normalize ( mvmult ( RotateX (amount), up ) );
      viewMatrix = LookAt (eye, look, up);
    }

    void rotateY (float amount)
    {
      rotate (y_rot, amount);
      look = eye + (mvmult (RotateY (amount), ( look - eye ) ) );
      viewMatrix = LookAt (eye, look, up);
    }

    void rotateZ (float amount)
    {
      rotate (z_rot, amount);
      up = normalize ( mvmult ( RotateZ (amount), up ) );
      viewMatrix = LookAt (eye, look, up);
    }

    void forward (float amount)
    {
      vec4 dv = ( look - eye ) * amount;
      eye += dv;
      look += dv;
      viewMatrix = LookAt (eye, look, up);
    }

    void strafe (float amount)
    {
      vec4 dv = cross ( (look - eye), up ) * amount;
      eye += dv;
      look += dv;
      viewMatrix = LookAt (eye, look, up);
    }

    const mat4& getViewMatrix (){
        return viewMatrix;
    }

  private:

    mat4 viewMatrix;
    mat4 rotationMatrix;

    vec4 eye;
    vec4 look;
    vec4 up;

    float x_rot;
    float y_rot;
    float z_rot;
};