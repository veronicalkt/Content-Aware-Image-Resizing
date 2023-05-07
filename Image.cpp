#include <iostream>
#include "Matrix.hpp"
#include "Image.hpp"
#include <string>

using namespace std;

// Initializes an empty image
Image::Image():
  m_width(0), m_height(0), m_red_channel(),
  m_green_channel(), m_blue_channel() {}

// Initializes an Image with the given dimensions and
// with each pixel initialized to "zero" in each channel.
Image::Image(int width, int height):
  m_width(width), m_height(height), m_red_channel(width, height, 0),
  m_green_channel(width, height, 0), m_blue_channel(width, height, 0) {}

// Initializes an Image with the given dimensions and
// with each pixel initialized to the value specified by "fill".
Image::Image(int width, int height, const Pixel& fill):
  m_width(width), m_height(height), m_red_channel(width, height, fill.r),
  m_green_channel(width, height, fill.g),
  m_blue_channel(width, height, fill.b) {}

// Reads an image in PPM format (without comments, any kind of whitespace is ok)
// from the given input stream, constructs an Image object according
// to the data, and returns it.
// Note: Static member functions are called *without* an object of
// the class. See Image_public_test.cpp for an example of how to use
// this function.

Image Image::read_ppm(istream& is) {

  string ppm;
  Image img;
  int width;
  int height;
  int max;

  is >> ppm >> width >> height >> max;
  img = Image(width,height);

  for (int j = 0; j < height; ++j) {
    for (int k = 0; k < width; ++k) {
    int r;
    int g;
    int b;
    is >> r >> g >> b;
    Pixel p = {r,g,b};
    img.set_pixel(j,k,p);
    }
  }
  return img;
}


// Writes this image to the given output stream in PPM format.
// You must use the kind of whitespace specified here.
// First, prints out the header for the image like this:
//   P3 [newline]
//   WIDTH [space] HEIGHT [newline]
//   255 [newline]
// Next, prints out the rows of the image, each followed by a
// newline. Each pixel in a row is printed as three ints
// for its red, green, and blue components, in that order. Each
// int is followed by a space. This means that there will be an
// "extra" space at the end of each line. See the project spec
// for an example.
void Image::print(ostream& os) const {
  os << "P3" << "\n" << m_width << " " << m_height << "\n"
    << c_max_intensity << endl;
  for (int i = 0; i < m_height; ++i) {
    for (int j = 0; j < m_width; ++j) {
      os << m_red_channel.at(i,j) << ' ' << m_green_channel.at(i,j)  << ' '
        << m_blue_channel.at(i,j) << ' ';
      if (j == m_width - 1) {
        os << endl;
      }
    }
  }
}

// Returns the width of this Image.
int Image::get_width() const {
  return m_width;
}

// Returns the height of this Image.
int Image::get_height() const {
  return m_height;
}

// Returns the pixel at the given row and column.
// If row or column are out of bounds (either less than zero or row >= height
// or column >= width, the behavior is undefined.
Pixel Image::get_pixel(int row, int column) const {
  Pixel gotPixel;
  gotPixel.r = m_red_channel.at(row, column);
  gotPixel.g = m_green_channel.at(row, column);
  gotPixel.b = m_blue_channel.at(row, column);
  return gotPixel;
}

// Sets the pixel in this Image at the given row and column to the given color.
// If row or column are out of bounds (either less than zero or row >= height
// or column >= width, the behavior is undefined.
void Image::set_pixel(int row, int column, const Pixel& color) {
  m_red_channel.at(row, column) = color.r;
  m_green_channel.at(row, column) = color.g;
  m_blue_channel.at(row, column) = color.b;
}