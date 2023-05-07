#include <iostream>
#include <vector>
#include "Matrix.hpp"

using namespace std;

// Initializes an empty Matrix
Matrix::Matrix(): m_width(0), m_height(0), m_data() {}

// Initializes a Matrix with the given dimensions and
// with each cell initialized to zero.
Matrix::Matrix(int width, int height):
  m_width(width), m_height(height), m_data(width * height, 0) {}

// Initializes a Matrix with the given dimensions and
// with each cell initialized to the value specified by fill_value.
Matrix::Matrix(int width, int height, int fill_value):
  m_width(width), m_height(height), m_data(width * height, fill_value) {}

// Returns the width of this Matrix.
int Matrix::get_width() const {
  return m_width;
}

// Returns the height of this Matrix.
int Matrix::get_height() const {
  return m_height;
}

// Returns a modifiable reference to the cell at the given row and column.
// If row or column are out of bounds (either less than zero or row >= height
// or column >= width, the behavior is undefined.
int& Matrix::at(int row, int column) {
  int cell = (m_width * row) + column;
  return m_data[cell];
}

// Returns a non-modifiable reference to the cell at the given row and column.
// If row or column are out of bounds (either less than zero or row >= height
// or column >= width, the behavior is undefined.
const int& Matrix::at(int row, int column) const {
  int cell = (m_width * row) + column;
  return m_data[cell];
}

// Returns a Slice representing the data from the requested portion of the
// specified row. The range [col_start, col_end) is inclusive of col_start
// and exclusive of col_end.
// - If col_start is less than zero, the first element of the returned slice
// will be the first element of the row.
// of the row.
// - If col_end is greater than the width of this Matrix, the last element of
// the returned slice will be the last element of the row.
// - col_start must be strictly less than col_end,
// otherwise the behavior is undefined.
// - If row is out of bounds (less than zero or >= height),
// the behavior is undefined.
//
// The members col_start and col_end of the returned Slice will reflect
// any adjustments made to the input parameters.
// For example, if this method is called with the col_start argument set to -1,
// the value of col_start in the returned Slice object will instead be zero.
//
// Note: When defining this method in Matrix.cpp, you will need to change
// The return type from Slice to Matrix::Slice. In the body of the method
// definition, you will be able to refer to it as Slice.
Matrix::Slice Matrix::get_row_slice(int row, int col_start, int col_end) const {
  if (col_start < 0) {
    col_start = 0;
  }
  if (col_end > (m_width - 1)) {
    col_end = m_width - 1;
  };
  int col_startIndex = (m_width * row) + col_start;
  int col_endIndex = (m_width * row) + col_end;
  Slice finalSlice;
  finalSlice.row = row;
  finalSlice.col_start = col_start;
  finalSlice.col_end = col_end;
  for (int i = col_startIndex; i <= col_endIndex; i++) {
    finalSlice.data.push_back(m_data[i]);
  };
  return finalSlice;
}

// Prints this Matrix to the stream referenced by os as follows:
  // First, prints the width and height for the Matrix to os:
  // WIDTH [space] HEIGHT [newline]
  // Then prints the rows of the Matrix to os with one row per line.
  // Each element is followed by a space and each row is followed
  // by a newline. This means there will be an "extra" space at
  // the end of each line.
  void Matrix::print(ostream& os) const {
    os << m_width << " " << m_height << endl;
    for (int i = 0; i < m_height; i++) {
      for (int j = 0; j < m_width; j++) {
        os << at(i, j) << " ";
        if (j == m_width - 1) {
          os << endl;
        }
      }
    }
  }