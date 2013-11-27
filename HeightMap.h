

#pragma once
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Angel.h"
#include "util.h"
using namespace std;

const GLuint PRIMITIVE_RESTART = 99999999;

class HeightMap
{
  public:

    // creates a flat plane of 3d points
    HeightMap (int height=16, int width=16, float spa = .01, float xo=0, float zo=0, float iy=0);
    ~HeightMap ();

    // Resets all y heights to zero
    void reset ();

    // 
    int getRows ();

    // 
    int getCols ();

    // total number of vertices
    int getSize ();
    
    vec4& at (int row, int column);

    void print ();

    // Procedurally generate values of y heights
    void generateHeights (vec4 genFunc (int i, int j));

    // Generates color values for each vertex
    void generateColors (vec4 genFunc (int i, int j));

    int sizeOfTriangleVertices ();

    void flattenTriangles (vec4* arr);

    // need a single array to pass to the gpu buffer
    // contains all vertices for all the tri strips along with colors for each
    // vertex
    void flatten (vec4* arr);

    // get the number of indexes needed to draw the height map as tri-strips
    // including primitive restart indexes 
    int sizeOfTriStripIndices ();

    // return an array of indices to vertices in the order needed to draw a set
    // of tri-strips from the height map the complete array requires a 
    // primitive restart index at the end of each row
    void flattenTriStripIndices (unsigned* arr);

  private:

    int rows, cols, size;

    float xoff, zoff, y;
    float spacing;

    vector < vector <vec4> > heights;
    vector < vector <vec4> > colors;

};