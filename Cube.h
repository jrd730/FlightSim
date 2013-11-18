
const int NumVertices = 36;
vec4 points[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
vec4 vertices[8] = {
    vec4( -0.01, -0.01,  0.01, 1.0 ),
    vec4( -0.01,  0.01,  0.01, 1.0 ),
    vec4(  0.01,  0.01,  0.01, 1.0 ),
    vec4(  0.01, -0.01,  0.01, 1.0 ),
    vec4( -0.01, -0.01, -0.01, 1.0 ),
    vec4( -0.01,  0.01, -0.01, 1.0 ),
    vec4(  0.01,  0.01, -0.01, 1.0 ),
    vec4(  0.01, -0.01, -0.01, 1.0 )
};

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad( int a, int b, int c, int d )
{
  points[Index] = vertices[a]; Index++;
  points[Index] = vertices[b]; Index++;
  points[Index] = vertices[c]; Index++;
  points[Index] = vertices[a]; Index++;
  points[Index] = vertices[c]; Index++;
  points[Index] = vertices[d]; Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}