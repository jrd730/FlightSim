#version 150

in vec4 vPosition;
in vec4 vColor;

uniform vec4 uColor;
uniform int solidColor;
uniform mat4 vMvp;

out vec4 color;

void main() 
{
  if (solidColor == 1){
    color = uColor;
  }
  else{
    color = vColor;
  }
  gl_Position = vMvp * vPosition;
} 