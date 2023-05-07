#include "processing.hpp"
#include "Image.hpp"
#include "Matrix.hpp"
#include <algorithm>
#include <vector>

namespace {
  // The implementation of squared_difference is provided for you.
  int squared_difference(Pixel p1, Pixel p2);
}

// Implement the functions declared in processing.hpp here.

namespace {
  // The implementation of squared_difference is provided for you.
  int squared_difference(Pixel p1, Pixel p2) {
    int dr = p2.r - p1.r;
    int dg = p2.g - p1.g;
    int db = p2.b - p1.b;
    // Divide by 100 is to avoid possible overflows
    // later on in the algorithm.
    return (dr*dr + dg*dg + db*db) / 100;
  }
}

using namespace std;

// Returns a copy of the given image that is rotated 90 degrees to the
// left (counterclockwise).
Image rotate_left(const Image& img) {

  int newWidth = img.get_height();
  int newHeight = img.get_width();
  Image leftImage(newWidth, newHeight);
  for (int i = 0; i < newHeight; ++i) {
    for (int j = 0; j < newWidth; ++j) {
      leftImage.set_pixel(i,j, img.get_pixel(j, newHeight - i - 1));
    }
  }
  return leftImage;
}

// Returns a copy of the given image that is rotated 90 degrees to the
// right (clockwise).
Image rotate_right(const Image& img) {

  int newWidth = img.get_height();
  int newHeight = img.get_width();
  Image rightImage(newWidth, newHeight);
  for (int i = 0; i < newHeight; ++i) {
    for (int j = 0; j < newWidth; ++j) {
      rightImage.set_pixel(i,j, img.get_pixel(newWidth - j - 1, i));
    }
  }
  return rightImage;
}

// Returns the energy Matrix computed from the given Image.
// See the assignment spec for details on computing the energy matrix.
Matrix compute_energy_matrix(const Image& img) {

  int height = img.get_height();
  int width = img.get_width();

  Matrix energyMatrix(width, height);
  vector<int> energiesThusFar(width*height);
  int maxEnergy = 0;

  for (int i = 1; i < img.get_height() - 1; ++i) {
    for (int j = 1; j < img.get_width() - 1; ++j) {
      Pixel N = img.get_pixel(i - 1, j);
      Pixel S = img.get_pixel(i + 1, j);
      Pixel W = img.get_pixel(i, j - 1);
      Pixel E = img.get_pixel(i, j + 1);
      int energyVal = squared_difference(N, S) + squared_difference(W, E);
      energiesThusFar.at(width * i + j) = energyVal;
      if (energyVal > maxEnergy) {
        maxEnergy = energyVal;
      }
    }
  }


  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {

      if (row == 0 || row == height-1) {
        energyMatrix.at(row,col) = maxEnergy;
      } else if(col == 0 || col == width -1) {
        energyMatrix.at(row,col) = maxEnergy;
      } else{
      int vecIndex = (width * row) + col;
      energyMatrix.at(row,col) = energiesThusFar.at(vecIndex);
      }
    }
  }
  return energyMatrix;
}


// Returns the vertical cost Matrix computed from the given Image.
// See the assignment spec for details on computing the cost matrix.
Matrix compute_vertical_cost_matrix(const Image& img) {

  Matrix energyMatrix = compute_energy_matrix(img);
  int width = energyMatrix.get_width();
  int height = energyMatrix.get_height();
  Matrix costMatrix(width, height);

  for (int col = 0; col < width; col++) {
    costMatrix.at(0, col) = energyMatrix.at(0, col);
  };

  for (int row = 1; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (col == 0) {
        costMatrix.at(row,col) = energyMatrix.at(row,col)
          + min(costMatrix.at(row-1, col), costMatrix.at(row-1, col+1));
      }
      else if (col == width - 1) {
        costMatrix.at(row,col) = energyMatrix.at(row,col)
          + min(costMatrix.at(row-1, col), costMatrix.at(row-1, col-1));
      }
      else {
        costMatrix.at(row,col) = energyMatrix.at(row,col)
          + min({costMatrix.at(row-1, col-1), costMatrix.at(row-1, col),
            costMatrix.at(row-1, col+1)});
      }
    }
  };
  return costMatrix;
}

// Returns a vector containing the column indices of each pixel along the
// vertical seam with the minimal cost according to the given cost matrix.
// The elements in the vector starts with the lowest numbered row (top of image)
// and progressing to the highest (bottom of image).
// While determining the seam, if any pixels tie for lowest cost, the leftmost
// one (i.e. with the lowest column number) is used.
// See the project spec for details on computing the minimal seam.
vector<int> find_minimal_vertical_seam(const Matrix& cost) {

  int height =  cost.get_height();
  int width = cost.get_width();
  vector<int> minSeamColumns(height);

  Matrix::Slice bottomrow_costs = cost.get_row_slice(height-1, 0, width-1);
  auto bottomRowMinCost = min_element(begin(bottomrow_costs.data), 
      end(bottomrow_costs.data));
  minSeamColumns.at(height-1) = distance(begin(bottomrow_costs.data),
      bottomRowMinCost);

  for (int row = height - 2; row >= 0; --row) {
    if (minSeamColumns.at(row+1) == 0) {
      int leftCol = 0;
      int rightCol = 1;
      Matrix::Slice row_costs = cost.get_row_slice(row, leftCol, rightCol);
      auto rowMinCost = min_element(begin(row_costs.data), end(row_costs.data));
      minSeamColumns.at(row) = distance(begin(row_costs.data), rowMinCost);
    } else if (minSeamColumns.at(row+1) == width - 1) {
      int leftCol = width - 2;
      int rightCol = width - 1;
      Matrix::Slice row_costs = cost.get_row_slice(row, leftCol, rightCol);
      auto rowMinCost = min_element(begin(row_costs.data), end(row_costs.data));
      minSeamColumns.at(row) = distance(begin(row_costs.data), rowMinCost);
    } else {
      int leftCol = minSeamColumns.at(row+1) - 1;
      int rightCol = minSeamColumns.at(row+1) + 1;
      Matrix::Slice row_costs = cost.get_row_slice(row, leftCol, rightCol);
      auto rowMinCost = min_element(begin(row_costs.data), end(row_costs.data));
      minSeamColumns.at(row) = leftCol +
                distance(begin(row_costs.data), rowMinCost);
    }
  }
  return minSeamColumns;
  }

// Returns a copy of img with the given vertical seam removed. That is, one
// pixel will be removed from every row in the image. The pixel
// removed from row r will be the one with column equal to seam[r].
// The width of the image will be one less than before.
// See the project spec for details on removing a vertical seam.
Image remove_vertical_seam(const Image& img, const vector<int>& seam) {
  int oldWidth = img.get_width();
  int newWidth = oldWidth - 1;
  int height = img.get_height();
  Image newImage(newWidth, height);

  for (int row = 0; row < height; ++row) {
    int pixelToBeRemoved = seam.at(row);

    if (pixelToBeRemoved == 0) {
      for (int col = 0; col < newWidth; ++col) {
        newImage.set_pixel(row, col, img.get_pixel(row,col+1));
      }
    } else if (pixelToBeRemoved == oldWidth - 1) {
      for (int col = 0; col < newWidth; ++col) {
        newImage.set_pixel(row, col, img.get_pixel(row,col));
      }
    } else {
      for (int col = 0; col < pixelToBeRemoved; ++col) {
        newImage.set_pixel(row, col, img.get_pixel(row, col));
      }
      for (int col = pixelToBeRemoved; col < newWidth; ++col) {
        newImage.set_pixel(row, col, img.get_pixel(row, col+1));
      }
    }
  }
    /*
      for(int j = 0; j < pixelToBeRemoved; j++) {
        Pixel oldPixel = img.get_pixel(i, j);
        newImage.set_pixel(i, j, oldPixel);
      }
    }

    for(int j = pixelToBeRemoved + 1; j < width; j++) {
      Pixel oldPixel = img.get_pixel(i, j);
      newImage.set_pixel(i, j, oldPixel);
    }
  }*/
  return newImage;
}

// Returns a copy of img with its width reduced to be new_width by using
// the seam carving algorithm. See the spec for details.
// Requires that 0 < new_width <= img.get_width(), otherwise the behavior
// is undefined.
Image seam_carve_width(const Image& img, int new_width) {
  int oldWidth = img.get_width();
  Image currentImage = img;

  for (int width = oldWidth; width > new_width; --width) {
    Matrix costMatrix = compute_vertical_cost_matrix(currentImage);
    vector<int> minCostSeam = find_minimal_vertical_seam(costMatrix);
    currentImage = remove_vertical_seam(currentImage, minCostSeam);
  }
  return currentImage;
}

// Returns a copy of img with its height reduced to be new_height by using
// the seam carving algorithm. See the spec for details.
// Requires that 0 < new_height <= img.get_height(), otherwise the behavior
// is undefined.
Image seam_carve_height(const Image& img, int new_height) {
  Image rotatedImage = rotate_left(img);
  Image adjustedImage = seam_carve_width(rotatedImage, new_height);
  Image fixedImage = rotate_right(adjustedImage);
  return fixedImage;
}

// Returns a copy of img with its width and height reduced to be
// new_width and new_height, respectively.
//
// NOTE: This is equivalent to applying seam_carve_width(img, new_width)
// and then applying seam_carve_height(img, new_height).

// Requires that 0 < new_height <= img.get_height() and
// 0 < new_height <= img.get_height(), otherwise the behavior
// is undefined.
Image seam_carve(const Image& img, int newWidth, int newHeight) {
  Image newWidthImage = seam_carve_width(img, newWidth);
  Image finalImage = seam_carve_height(newWidthImage, newHeight);
return finalImage;
}