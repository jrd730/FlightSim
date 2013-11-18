#version 150

in  vec4 vPosition;
out vec4 color;

uniform vec4 vColor;
uniform mat4 vModelMatrix;
uniform mat4 vViewMatrix;
uniform mat4 vProjectionMatrix;
uniform vec4 xlate;

void main() 
{
    color = vColor;
    //gl_Position = vModelMatrix * vPosition;
    //gl_Position = vViewMatrix * vModelMatrix * vPosition;
    gl_Position = vProjectionMatrix * vViewMatrix * vModelMatrix * vPosition;
    //gl_Position = vec4(vPosition.x+xlate[0], vPosition.y+xlate[1], vPosition.zw);
} 