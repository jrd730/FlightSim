#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "Angel.h"
#include "Camera.h"
#include "Colors.h"
#include "Cube.h"
#include "HeightMap.h"
#include "Windmill.h"
using namespace std;

Camera camera (vec4(0, 0, 2, 1), 0, 0, 0);

HeightMap hm (512, 512, .01);

Windmill windmill;

mat4 viewMatrix; 

char keyDown [255];

bool going = true;

GLuint cube_vao;
GLuint terrain_vao;

GLuint xlate;
GLuint vColor;
GLuint vPosition;
GLuint vViewMatrix;
GLuint vModelMatrix;
GLuint vProjectionMatrix;

GLfloat Left= -2.0, Right=2.0, top=2.0, bottom= -2.0, near= -4.0, far=4.0;

//mat4 projection = Ortho(Left, Right, bottom, top, near, far);
mat4 projection = Perspective( 45.0, 1.0, 0.1, 10.0 );

///////////////////////////////////////////////////////////////////////////////
//
//

void display ()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );  

  // draw windmill base
  glUniform4fv (vColor, 1, colors[RED]);
  glUniformMatrix4fv (vModelMatrix, 1, GL_TRUE, windmill.getBaseTransform());
  glUniformMatrix4fv (vViewMatrix, 1, GL_TRUE, camera.getViewMatrix() );
  glUniformMatrix4fv (vProjectionMatrix, 1, GL_TRUE, projection );
  
  glBindVertexArray( cube_vao );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  // draw windmill blades
  for (int i=0; i < 4; ++i){
    glUniform4fv (vColor, 1, colors[1+i]);
    glUniformMatrix4fv (vModelMatrix, 1, GL_TRUE, windmill.getBladeTransform(i));
    glUniformMatrix4fv (vViewMatrix, 1, GL_TRUE, camera.getViewMatrix() );
    glUniformMatrix4fv (vProjectionMatrix, 1, GL_TRUE, projection );

    glBindVertexArray( cube_vao );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
  }

  // draw height map


  glutSwapBuffers();
}


///////////////////////////////////////////////////////////////////////////////
//
//

void updateCamera ()
{
  if (keyDown ['u'] ){
    camera.rotateX (1.0);
  }
  else if ( keyDown ['j'] ){
    camera.rotateX (-1.0);
  }

  if (keyDown ['h'] ){
    camera.rotateY (1.0);
  }
  else if ( keyDown ['k'] ){
    camera.rotateY (-1.0);
  }

  if (keyDown ['q'] ){
    camera.rotateZ (1.0);
  }
  else if ( keyDown ['e'] ){
    camera.rotateZ (-1.0);
  }

  if (keyDown ['w'] ){
    camera.forward (0.01);
  }
  else if ( keyDown ['s'] ){
    camera.forward (-0.01);
  }
  if (keyDown ['a'] ){
    camera.strafe (-0.01);
  }
  else if ( keyDown ['d'] ){
    camera.strafe (0.01);
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//

void keyboard (unsigned char key, int x, int y)
{
  keyDown [key] = true;

  switch (key){
    case 'y':
      windmill.rotateBase (5.0);
    break;
    case 'Y':
      windmill.rotateBase (-5.0);
    break;
    case '~':
      going = !going;
    break;
    case 'r':
      windmill.reset ();
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

void timer (int val)
{
  if (going)
    windmill.rotateBlade (1.0);
  
  windmill.generateBaseTransform ();
  windmill.generateBladeTransform ();

  updateCamera ();
  
  glutPostRedisplay ();
  glutTimerFunc (17, timer, 0);
}

///////////////////////////////////////////////////////////////////////////////
//
//

void initCube ()
{
  colorcube();

  glGenVertexArrays(1, &cube_vao);
  glBindVertexArray(cube_vao);

  GLuint buffer; // handle to a buffer object
  glGenBuffers (1, &buffer); // ask GPU for a buffer
  glBindBuffer (GL_ARRAY_BUFFER, buffer); // tells GPU what kind of buffer 
  
  // allocate space for the buffer
  glBufferData (GL_ARRAY_BUFFER, NumVertices*sizeof(vec4), NULL, 
    GL_STATIC_DRAW);

  // copy cube vertices into the newly created buffer
  glBufferSubData (GL_ARRAY_BUFFER, 0, NumVertices*sizeof(vec4), points[0]);

  // enables the attribute in the shader
  glEnableVertexAttribArray ( vPosition );

  // specifies how many and what kind of components comprise the attribute
  glVertexAttribPointer ( vPosition, 4, GL_FLOAT, GL_FALSE, 0, 
    BUFFER_OFFSET(0));

  //glEnableVertexAttribArray( vColor );

  // glEnableVertexAttribArray ( vTransform );
  // glVertexAttribPointer ( vTransform, 16, GL_FLOAT, GL_FALSE, 0, 
  //   BUFFER_OFFSET(0));
}

///////////////////////////////////////////////////////////////////////////////
//
//

void initHeightMap ()
{
  glGenVertexArrays(1, &terrain_vao);
  glBindVertexArray(terrain_vao);

  // initialize buffer
  GLuint buffer; 
  glGenBuffers (1, &buffer); 
  glBindBuffer (GL_ARRAY_BUFFER, buffer); 

  // allocate space for the buffer
  glBufferData (GL_ARRAY_BUFFER, hm.getSize()*sizeof(vec4), NULL, 
    GL_STATIC_DRAW);

  vec4 heightMapVertices [hm.getSize ()];
  
  hm.flatten (heightMapVertices);

  // copy cube vertices into the newly created buffer
  glBufferSubData (GL_ARRAY_BUFFER, 0, hm.getSize()*sizeof(vec4), 
    heightMapVertices);

  // enables the attribute in the shader
  glEnableVertexAttribArray ( vPosition );

  // specifies how many and what kind of components comprise the attribute
  glVertexAttribPointer ( vPosition, 4, GL_FLOAT, GL_FALSE, 0, 
    BUFFER_OFFSET(0));
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
  glutInitWindowSize (480, 480);
  glutInitContextVersion( 3, 2 );
  glutInitContextProfile( GLUT_CORE_PROFILE );
  
  glutCreateWindow ("Windmill");
  glutSetWindowTitle("Windmill");
  glutSetIconTitle("Windmill");
  
  glewExperimental = GL_TRUE;
  glewInit ();

  glEnable( GL_DEPTH_TEST );
  glClearColor(0.1, 0.4, 0.9, 1.0);

  glutTimerFunc (40, timer, 0);
  glutDisplayFunc(display);
  //glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyboardUp);
  glutSpecialFunc (special);

  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
  glUseProgram( program );

  xlate = glGetUniformLocation( program, "xlate" );
  vColor = glGetUniformLocation( program, "vColor" );
  vPosition = glGetAttribLocation( program, "vPosition" );
  vViewMatrix = glGetUniformLocation( program, "vViewMatrix" );
  vModelMatrix = glGetUniformLocation( program, "vModelMatrix" );
  vProjectionMatrix = glGetUniformLocation( program, "vProjectionMatrix" );

  initCube ();
  initHeightMap ();
  glutMainLoop ();
  
  return 0;
}