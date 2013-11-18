
// Simulates a camera moving through a 3d space and maintains a view 
// transformation matrix for use in the vertex shader.
// Abstracts the complexities of performing simple operations to adjust
// the camera's position and direction within a volume.

class Camera
{
  public:
    Camera (const vec4& pos, float rx, float ry, float rz) :
      eye (pos), x_rot (rx), y_rot (ry), z_rot (rz),
      viewMatrix (1.0)
    {
        look = vec4 (0, 0, 0, 1);
        up = vec4 (0, 1, 0, 0);

        viewMatrix = LookAt (eye, look, up);
    }

    ~Camera ()
    {

    }

    void setEye (const vec3& p) { eye = p; }
    void setLook (const vec3& l){ look = l; }
    void setUp (const vec3& u)  { up = u; }

    void setXRot (float rx) { x_rot = rx; }
    void setYRot (float ry) { y_rot = ry; }
    void setZRot (float rz) { z_rot = rz; }

    void rotate (float& angle, float amount)
    {
      angle += amount;
      if (amount > 0 && angle > 360.0) angle -= 360.0;
      else if (amount < 0 && angle < 0) angle += 360.0;
    }

    void rotateX (float amount)
    {    
      rotate (x_rot, amount);
      up = normalize ( mvmult ( RotateX (amount), up ) );
      look = eye + (mvmult (RotateX (amount), ( look - eye ) ) );
      viewMatrix = LookAt (eye, look, up);
    }

    void rotateY (float amount) {
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

    vec4 eye;
    vec4 look;
    vec4 up;

    float x_rot;
    float y_rot;
    float z_rot;
    

};