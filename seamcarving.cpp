#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using std::cout, std::endl, std::cin, std::string, std::stringstream;

int main() {
  // 2D array of pixels
  static Pixel image[MAX_WIDTH][MAX_HEIGHT];  
  
  // initialize 2D array
  initializeImage(image);

  // declare variables
  string filename;
  unsigned int width = 0;
  unsigned int height = 0;
  unsigned int targetWidth = 0;
  unsigned int targetHeight = 0;
  
  // accept filename
  cout << "Input filename: ";
  cin >> filename;

  try {
    // attempt to load image
    loadImage(filename, image, width, height);
    cout << "Detected image width: " << width << endl;
    cout << "Detected image height: " << height << endl;
  } catch (std::exception &ex) {
    cout << "Exception thrown: " << ex.what() << endl;
    return 1; // exit with error
  }

  // accept target dimensions
  cout << "Input target width: ";
  cin >> targetWidth;
  cout << "Input target height: ";
  cin >> targetHeight;

  // TODO: add code to validate input (part 1)
  // Exit program with error if there are problems with 
  //   targetWidth or targetHeight
  if(targetWidth > width || targetHeight > height || targetWidth <= 0 || targetHeight <= 0) {
    cout << ("Invalid target dimensions") << endl;
    return 1;
  }


  cout << "Start carving..." << endl;
  unsigned int seam[MAX_HEIGHT];
  width = 40;
  loadHorizontalSeam(image, 1, 6, 5, seam);
  for(int i = 0; i < 5; i++) {
    cout << seam[i] << endl;
  }
  while ((width - targetWidth > 0) || (height - targetHeight > 0)) {
    // find and remove a vertical seam
    if (width - targetWidth > 0) {
      unsigned int verticalSeam[MAX_HEIGHT];
      findMinVerticalSeam(image, width, height, verticalSeam);
      removeVerticalSeam(image, width, height, verticalSeam);
    }
    // find and remove a horizontal seam
    if (height - targetHeight > 0) {
      unsigned int horizontalSeam[MAX_WIDTH];
      findMinHorizontalSeam(image, width, height, horizontalSeam);
      removeHorizontalSeam(image, width, height, horizontalSeam);
    }
  }
  
  try {
    // attempt to output image
    stringstream ss;
    ss << "carved" << width << "X" << height << ".ppm";
    string filepath = "carved_images/" + ss.str();
    outputImage(filepath.c_str(), image, width, height);
  } catch (std::exception &ex) {
    cout << "Exception thrown: " << ex.what() << endl;
    return 1; // exit with an error
  }

  cout << "Image output" << endl;

  return 0; // exit without errors
}