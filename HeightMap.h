#pragma once
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// should work for the following types: byte, int, long, float, double
//template <typename T>

class HeightMap
{
  public:

    // Creates a 2d grid of height*width z heights.
    HeightMap (int height=512, int width= 512, float spa = .01) :
      rows (height), cols (width), spacing (spa)
    {
      heights.resize (height);
      for (int i = 0; i < heights.size(); ++i){
        heights[i].resize (width);
      }
      reset ();
    }
    
    ~HeightMap (){}

    // Resets all z heights to zero
    void reset ()
    {
      for (int i = 0; i < rows; ++i){
        for (int j = 0; j < cols; ++j)
        {
          heights[i][j] = vec4 (i*spacing, j*spacing, 0, 1);
        }
      }
    }

    int getHeight () { return rows; }
    int getWidth () { return cols; }
    int getSize () { return rows*cols; }
    
    vec4& at (int row, int column)
    {
      return heights[row][column]; 
    } 

    // Procedurally generate values of z heights
    void generate (vec4 genFunc (int i, int j))
    {
     for (int i = 0; i < getHeight (); ++i){
        for (int j = 0; j < getWidth (); ++j){
          heights[i][j] = genFunc (i, j);
        }
      }
    }

    // Copies the elements of heights into a 1d array pointed to by arr.
    // arr must be initialized to a size of at least height*width
    void flatten (vec4* arr)
    {
      int index = 0;
      for (int i = 0; i < getHeight (); ++i){
        for (int j = 0; j < getWidth (); ++j){
          arr[index] = heights[i][j];
          ++index;
        }
      }
    }


  private:

    int rows, cols;
    float spacing;

    vector < vector <vec4> > heights;
    vector < vector <vec4> > colors;

};