#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "Angel.h"
#include "Camera.h"
//#include "Colors.h"
#include "Cube.h"
#include "HeightMap.h"
#include "Windmill.h"
using namespace std;

Camera camera (vec4(0, 0, 2, 1), 0, 0, 0);
HeightMap hm (200, 200, .1, -2.0, .5, -.7);
int num_hm_elements;
GLuint* indices;

Windmill windmill;

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
  // draw windmill base
  glUniform1i (solidColor, 1);
  glUniform4fv (uColor, 1, colors[RED]);

  projectionViewModel = projectionView * windmill.getBaseTransform();
  glUniformMatrix4fv (vMvp, 1, GL_TRUE, projectionViewModel);  
  
  glBindVertexArray( cube_vao );
  glDrawArrays( GL_TRIANGLES, 0, NumVertices );

  // draw windmill blades
  for (int i=0; i < 4; ++i){
    glUniform1i (solidColor, 1);
    glUniform4fv (uColor, 1, colors[1+i]);

    projectionViewModel = projectionView * windmill.getBladeTransform(i);
    glUniformMatrix4fv (vMvp, 1, GL_TRUE, projectionViewModel);

    glBindVertexArray( cube_vao );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
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

void updateCamera ()
{
  if (keyDown ['u'] ){
    camera.rotateX (-1.0);
  }
  else if ( keyDown ['j'] ){
    camera.rotateX (1.0);
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
    camera.forward (0.05);
  }
  else if ( keyDown ['s'] ){
    camera.forward (-0.05);
  }
  if (keyDown ['a'] ){
    camera.strafe (-0.05);
  }
  else if ( keyDown ['d'] ){
    camera.strafe (0.05);
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

void timer (int val)
{
  if (going)
    windmill.rotateBlade (1.0);
  
  windmill.generateBaseTransform ();
  windmill.generateBladeTransform ();

  updateCamera ();
  
  projectionView = projection * camera.getViewMatrix ();

  glutPostRedisplay ();
  
  glutTimerFunc (17, timer, 0);
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
  glutSetWindowTitle("Flight Simulator");
  glutSetIconTitle("Flight Simulator");
  
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

  initShaders ();
  initCube ();
  initHeightMap ();
  
  glutMainLoop ();
  
  return 0;
}