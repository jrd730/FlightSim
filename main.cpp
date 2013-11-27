// simulates the view of a camera flying over a generated landscape

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include "Angel.h"
#include "Camera.h"
#include "Cube.h"
#include "HeightMap.h"
#include "Windmill.h"
using namespace std;

Camera camera (vec4(0, 0, 2, 1), 0, 0, 0);
HeightMap hm (200, 200, .3, -2.0, .7, -.7);
int num_hm_elements;
GLuint* indices;

const int NUM_WINDMILLS = 50;
Windmill windmill[NUM_WINDMILLS];
float wm_rotate = 0;

mat4 viewMatrix; 
mat4 projectionView;
mat4 projectionViewModel;

char keyDown [255];

bool going = true;

GLuint model_shader;
GLuint terrain_shader;

GLuint cube_vao;
GLuint hm_vao;

GLuint uColor;
GLuint vColor;
GLuint vPosition;
GLuint solidColor;
GLuint vViewMatrix;
GLuint vModelMatrix;
GLuint vProjectionMatrix;
GLuint vMvp;

float fixed_velocity = 0.1;
float velocity = 0.0;
float acceleration = 0.01;
float rotate_inc = 0.5;

//GLfloat Left= -2.0, Right=2.0, top=2.0, bottom= -2.0, near= -4.0, far=4.0;
//mat4 projection = Ortho(Left, Right, bottom, top, near, far);

mat4 projection = Perspective( 45.0, 1.0, 0.1, 100.0 );

///////////////////////////////////////////////////////////////////////////////
//
//

void initShaders ()
{
  model_shader = InitShader( "model.v", "model.f" );

  glUseProgram( model_shader );

  vMvp = glGetUniformLocation( model_shader, "vMvp" );
  vColor = glGetAttribLocation( model_shader, "vColor" );
  uColor = glGetUniformLocation( model_shader, "uColor" );
  vPosition = glGetAttribLocation( model_shader, "vPosition" );
  solidColor = glGetUniformLocation( model_shader, "solidColor" );
  vViewMatrix = glGetUniformLocation( model_shader, "vViewMatrix" );
  vModelMatrix = glGetUniformLocation( model_shader, "vModelMatrix" );
  vProjectionMatrix = glGetUniformLocation( model_shader, "vProjectionMatrix" );
}

///////////////////////////////////////////////////////////////////////////////
//
//

void initCube ()
{
  colorcube();

  glGenVertexArrays(1, &cube_vao);
  glBindVertexArray(cube_vao);

  GLuint buffer; 
  glGenBuffers (1, &buffer); 
  glBindBuffer (GL_ARRAY_BUFFER, buffer);
  
  glBufferData (GL_ARRAY_BUFFER, NumVertices*sizeof(vec4), points[0], GL_STATIC_DRAW);
  //glBufferSubData (GL_ARRAY_BUFFER, 0, NumVertices*sizeof(vec4), points[0]);

  glEnableVertexAttribArray ( vPosition );
  glVertexAttribPointer ( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
}

///////////////////////////////////////////////////////////////////////////////
//
//

void initWindmills ()
{
  for (int i = 0; i < NUM_WINDMILLS; ++i){
    windmill[i] = Windmill (vec3  (randomFloat()*50.0+2.0, 0.5, randomFloat()*50.0+2.0));
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//

void initHeightMap ()
{
  num_hm_elements = hm.sizeOfTriangleVertices()*2;
  vec4 hm_elements [num_hm_elements];
  hm.flattenTriangles (hm_elements);

  // num_hm_elements = hm.getSize () * 2;
  // vec4 hm_elements [num_hm_elements];
  // hm.flatten (hm_elements);

  // indices = new unsigned [ hm.sizeOfTriStripIndices () ];
  // hm.flattenTriStripIndices (indices);

  glGenVertexArrays(1, &hm_vao);
  glBindVertexArray(hm_vao);

  GLuint buffer; 
  glGenBuffers (1, &buffer); 
  glBindBuffer (GL_ARRAY_BUFFER, buffer); 

  glBufferData (GL_ARRAY_BUFFER, num_hm_elements*sizeof(vec4), hm_elements, GL_STATIC_DRAW);

  glEnableVertexAttribArray ( vPosition );
  glVertexAttribPointer ( vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(vec4)*2, BUFFER_OFFSET(0) );

  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer ( vColor, 4, GL_FLOAT, GL_FALSE, sizeof(vec4)*2, BUFFER_OFFSET(sizeof (vec4)));
}

/////////////////////////////////////////////////////////////////////////////////
//
//

void displayWindmill ()
{
  for (int i = 0; i < NUM_WINDMILLS; ++i)
  {
    // draw windmill base
    glUniform1i (solidColor, 1);
    glUniform4fv (uColor, 1, colors[RED]);

    projectionViewModel = projectionView * windmill[i].getBaseTransform();
    glUniformMatrix4fv (vMvp, 1, GL_TRUE, projectionViewModel);  
    
    glBindVertexArray( cube_vao );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

    // draw windmill blades
    for (int j=0; j < 4; ++j){
      glUniform1i (solidColor, 1);
      glUniform4fv (uColor, 1, colors[1+j]);

      projectionViewModel = projectionView * windmill[i].getBladeTransform(j);
      glUniformMatrix4fv (vMvp, 1, GL_TRUE, projectionViewModel);

      glBindVertexArray( cube_vao );
      glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    } 
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//

void displayTerrain ()
{
  glUniform1i (solidColor, 0);
  glUniformMatrix4fv (vMvp, 1, GL_TRUE, projectionView);

  glBindVertexArray( hm_vao );
  glPrimitiveRestartIndex (PRIMITIVE_RESTART);
  glDrawArrays( GL_TRIANGLES, 0, hm.sizeOfTriangleVertices () );
  //glDrawArrays( GL_LINE_STRIP, 0, hm.getSize () );
  //glDrawElements ( GL_TRIANGLE_STRIP, hm.sizeOfTriStripIndices (), GL_UNSIGNED_INT, indices);
}

///////////////////////////////////////////////////////////////////////////////
//
//

void display ()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  displayWindmill ();
  displayTerrain ();
  glutSwapBuffers();
}

///////////////////////////////////////////////////////////////////////////////
//
//

void updateCamera (int val)
{
  if (keyDown ['w'] ){
    camera.rotateX (-rotate_inc);
  }
  else if ( keyDown ['s'] ){
    camera.rotateX (rotate_inc);
  }

  if (keyDown ['a'] ){
    camera.rotateY (rotate_inc);
  }
  else if ( keyDown ['d'] ){
    camera.rotateY (-rotate_inc);
  }

  if (keyDown ['q'] ){
    camera.rotateZ (-rotate_inc);
  }
  else if ( keyDown ['e'] ){
    camera.rotateZ (rotate_inc);
  }

  if (keyDown ['u'] ){
    camera.forward (0.1);
  }
  else if ( keyDown ['j'] ){
    camera.forward (-0.1);
  }
  if (keyDown ['h'] ){
    camera.strafe (-0.1);
  }
  else if ( keyDown ['k'] ){
    camera.strafe (0.1);
  }

  if (keyDown ['p'] ){
    velocity += acceleration;
  }
  else if ( keyDown ['l'] ){
    velocity -= acceleration;
  }
  camera.forward (velocity);
  glutTimerFunc (17, updateCamera, 0);
}

///////////////////////////////////////////////////////////////////////////////
//
//

void keyboard (unsigned char key, int x, int y)
{
  keyDown [key] = true;

  switch (key){
    case 'y':
      rotate(wm_rotate, 5.0);
    break;
    case 'Y':
      rotate(wm_rotate, -5.0);
    break;
    case '~':
      going = !going;
    break;
    case 'r':
      //windmill.reset ();
    break;

    case '`':
      camera.resetViewMatrix ();
    break;
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//

void keyboardUp (unsigned char key, int x, int y)
{
  keyDown [key] = false;
}

///////////////////////////////////////////////////////////////////////////////
//
//

void special (int key, int x, int y)
{
  // switch (key){
  //   case GLUT_UP:

  //   break;

  //   case GLUT_LEFT:

  //   break;

  //   case GLUT_RIGHT:

  //   break;

  //   case GLUT_DOWN:

  //   break;
  // }
}

///////////////////////////////////////////////////////////////////////////////
//
//

void updateWindmill (int val)
{
  for (int i = 0; i < NUM_WINDMILLS; ++i)
  {
    if (going)
      windmill[i].rotateBlade (randomFloat()*2.0);
    
    windmill[i].setBaseRotation (wm_rotate);
    windmill[i].generateBaseTransform ();
    windmill[i].generateBladeTransform ();  
  }
  glutTimerFunc (17, updateWindmill, 0);
}

void setTitleBar (int val)
{
  stringstream ss;
  ss << "Altitude: " << camera.getEye().y;
  glutSetWindowTitle( ss.str().c_str() );
  glutTimerFunc (100, setTitleBar, 0);
}

void updateDisplay (int val)
{
  projectionView = projection * camera.getViewMatrix ();
  glutPostRedisplay ();
  glutTimerFunc (17, updateDisplay, 0);
}

///////////////////////////////////////////////////////////////////////////////
//
//

void reshape(GLsizei width, GLsizei height)
{
   glViewport( 0, 0, width, height );
    GLfloat aspect = GLfloat(width)/height;
    mat4  projection = Perspective( 45.0, 1.0/aspect, .1, 10.0 );
}

///////////////////////////////////////////////////////////////////////////////
//
//

int main (int argcp, char** argvp)
{
  srand (time (NULL));
  fill (keyDown, keyDown+255, 0);

  glutInit(&argcp, argvp);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize (640, 640);
  glutInitContextVersion( 3, 2 );
  glutInitContextProfile( GLUT_CORE_PROFILE );
  
  glutCreateWindow ("Flight Simulator");
  glutSetIconTitle("Flight Simulator");
  
  glewExperimental = GL_TRUE;
  glewInit ();

  glEnable( GL_DEPTH_TEST );
  glClearColor(0.1, 0.4, 0.9, 1.0);

  glutTimerFunc (1, updateCamera, 0);
  glutTimerFunc (1, updateWindmill, 0);
  glutTimerFunc (1, updateDisplay, 0);
  glutTimerFunc (1, setTitleBar, 0);
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutSpecialFunc (special);

  initShaders ();
  initCube ();
  initWindmills ();
  initHeightMap ();
  
  glutMainLoop ();
  return 0;
}