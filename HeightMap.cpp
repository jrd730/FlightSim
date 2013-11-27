#include "HeightMap.h"

 HeightMap :: HeightMap (int height, int width, float spa, float xo, float zo, float iy) :
  rows (height), cols (width), spacing (spa), xoff (xo), zoff (zo), y (iy)
{
  size = rows*cols;
  heights.resize (height);
  colors.resize (height);
  for (int i = 0; i < heights.size(); ++i){
    heights[i].resize (width);
    colors[i].resize (width);
  }
  reset ();
}

HeightMap :: ~HeightMap (){}

// Resets all y heights to zero
void HeightMap :: reset ()
{
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < cols; ++j)
    {
      heights[i][j] = vec4 ((float)(j)*spacing + xoff, y+(randomFloat()/4.0), (float)(i)*spacing + zoff, 1);
      colors[i][j] = vec4 (0, (1.3 - randomFloat()), 0, 1);
    }
  }
}

int HeightMap :: getRows () { return rows; }
int HeightMap :: getCols () { return cols; }
int HeightMap :: getSize () { return size; }

vec4& HeightMap :: at (int row, int column)
{
  return heights[row][column]; 
} 

void HeightMap :: print ()
{
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < cols; ++j){
      cout << heights[i][j] << colors[i][j] << endl;
    }
  }
}

// Procedurally generate values of y heights
void HeightMap :: generateHeights (vec4 genFunc (int i, int j))
{
 for (int i = 0; i < rows; ++i){
    for (int j = 0; j < cols; ++j){
      heights[i][j] = genFunc (i, j);
    }
  }
}

// Generates color values for each vertex
void HeightMap :: generateColors (vec4 genFunc (int i, int j))
{
 for (int i = 0; i < rows; ++i){
    for (int j = 0; j < cols; ++j){
      heights[i][j] = genFunc (i, j);
    }
  }
}

int HeightMap :: sizeOfTriangleVertices ()
{
  return (rows-1)*(cols-1)*2*3; 
}

void HeightMap :: flattenTriangles (vec4* arr)
{
  int index = 0;
  for (int i = 0; i < rows-1; ++i){
    for (int j = 0; j < cols-1; ++j){
      arr[index++] = heights[i][j];
      arr[index++] = colors[i][j];
      arr[index++] = heights[i+1][j];
      arr[index++] = colors[i+1][j];
      arr[index++] = heights[i][j+1];
      arr[index++] = colors[i][j+1];

      arr[index++] = heights[i][j+1];
      arr[index++] = colors[i][j+1];
      arr[index++] = heights[i+1][j+1];
      arr[index++] = colors[i+1][j+1];
      arr[index++] = heights[i+1][j];
      arr[index++] = colors[i+1][j];
    }
  }
}

// need a single array to pass to the gpu buffer
// contains all vertices for all the tri strips along with colors for each
// vertex
void HeightMap :: flatten (vec4* arr)
{
  int index = 0;
  for (int i = 0; i < rows; ++i){
    for (int j = 0; j < cols; ++j){
      arr[index] = heights[i][j];
      arr[index+1] = colors[i][j];
      index += 2;
    }
  }
}

// get the number of indexes needed to draw the height map as tri-strips
// including primitive restart indexes 
int HeightMap :: sizeOfTriStripIndices (){
  return 
    (rows * cols)
    + ( (rows-2) * cols )
    + (rows-1); 
}

// return an array of indices to vertices in the order needed to draw a set
// of tri-strips from the height map the complete array requires a 
// primitive restart index at the end of each row
void HeightMap :: flattenTriStripIndices (unsigned* arr)
{
  unsigned index = 0;

  for (unsigned i = 0; i < rows-1; ++i){
    
    arr [index] = (i * cols);
    arr [index+1] = ( (i+1) * cols);
    
    index += 2;

    for (unsigned j = 1; j < cols; ++j){
      
      arr [index] = (i * cols) + j;          
      arr [index+1] = ( (i+1) * cols) + j;  

      index += 2;
    }

    arr [index] = PRIMITIVE_RESTART;
    ++index;

  }
}