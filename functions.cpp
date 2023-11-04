#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include "functions.h"

using std::cout, std::endl, std::string;

void initializeImage(Pixel image[][MAX_HEIGHT]) {
  // iterate through columns
  for (unsigned int col = 0; col < MAX_WIDTH; col++) {
    // iterate through rows
    for (unsigned int row = 0; row < MAX_HEIGHT; row++) {
      // initialize pixel
      image[col][row] = {0, 0, 0};
    }
  }
}


void loadImage(string filename, Pixel image[][MAX_HEIGHT], unsigned int& width, unsigned int& height) {
  std::ifstream inFS;
  string line1 = "";
  unsigned int w, h, pixels = 0;
  string path = "source_images/" + filename;
  inFS.open(path.c_str());
  if(!inFS.is_open())
    throw std::runtime_error("Failed to open " + path);
  
  inFS >> line1;
  if(line1 != "P3")
    throw std::runtime_error("Invalid type " + line1);

  inFS >> w >> h;
  if(w <= 0 || h <= 0 || w > MAX_WIDTH || h > MAX_HEIGHT) {
    throw std::runtime_error("Invalid dimensions");
  }
  short r, g, b, maxRGBVal = 0;
  inFS >> maxRGBVal;
  width = w;
  height = h;
  Pixel lastPix = {0,0,0};
  for (unsigned int row = 0; row < height; row++) {
    for (unsigned int col = 0; col < width; col++) {
      if(!(inFS >> r >> g >> b)) {
        break;
      }
      if(r < 0 || r > maxRGBVal || g < 0 || g > maxRGBVal || b < 0 || b > maxRGBVal){
        throw std::runtime_error("Invalid color value");
      }
      pixels++;
      // cout << r << " " << g << " " << b << " " << pixels << endl;
      lastPix = {r,g,b};
      image[col][row] = lastPix;
    }
  }
  inFS >> r;

  if(pixels < width * height)
    throw std::runtime_error("Invalid color value");
  if(lastPix.r != r) {
    throw std::runtime_error("Too many values");
  }
  if(pixels > width * height)
    throw std::runtime_error("Too many values");
}

void outputImage(string filename, Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height) {
  std::ofstream outFS;
  // cout << filename << endl;
  outFS.open(filename);
  if(!outFS.is_open())
    throw std::runtime_error("Failed to open " + filename);
  outFS << "P3" << endl << width << " " << height << endl << "255" << endl;
  
  for (unsigned int row = 0; row < height; row++) {
    for (unsigned int col = 0; col < width; col++) {
      outFS << image[col][row].r << " " << image[col][row].g << " " << image[col][row].b << endl;
    }
  }
  
  outFS.close();
}
unsigned int square(unsigned int x) { return x*x; }

int calcEnergy(Pixel x, Pixel y){
  return square(x.r - y.r) + square(x.g - y.g) + square(x.b - y.b);
}

unsigned int energy(Pixel image[][MAX_HEIGHT], unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
  unsigned int energy = 0;
  // non-border pixels
  if(x != 0 && x != width-1 && y != 0 && y != height-1) {
    energy = calcEnergy(image[x-1][y], image[x+1][y]) + calcEnergy(image[x][y-1], image[x][y+1]);
    }
  // border pixels
  else if(x != 0 && x != width-1) {
    if(y == 0)
      energy = calcEnergy(image[x][height - 1], image[x][1]) + calcEnergy(image[x-1][y], image[x+1][y]);
    if(y == height - 1)
      energy = calcEnergy(image[x][0], image[x][y-1]) + calcEnergy(image[x-1][y], image[x+1][y]);
  }
  else if(y != 0 && y != height-1) {
    if(x == 0)
      energy = calcEnergy(image[width - 1][y], image[1][y]) + calcEnergy(image[x][y-1], image[x][y+1]);
    if(x == width - 1)
      energy = calcEnergy(image[0][y], image[x-1][y]) + calcEnergy(image[x][y-1], image[x][y+1]);
  }
  else {
    unsigned int x1 = 0;
    unsigned int x2 = 0;
    unsigned int y1 = 0;
    unsigned int y2 = 0;
    if(x==0) {
      x1 = width - 1;
      x2 = x+1;
    }
    else {
      x1 = x-1;
      x2 = 0;
    }
    if(y==0) {
      y1 = height - 1;
      y2 = y+1;
    }
    else {
      y1 = y-1;
      y2 = 0;
    }
    energy = calcEnergy(image[x1][y], image[x2][y]) + calcEnergy(image[x][y1], image[x][y2]);
  }

  // cout << "energy is: " << energy << " " << x << " " << y << " " << width << " " << height << endl;
  return energy;
}
// debugging function
void printEnergies(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height) {
  cout << "--------------------------" << endl;
  for(unsigned int i = 0; i < height; i++) {
    for(unsigned int j = 0; j < width; j++) {
      cout << energy(image, j, i, width, height) << ", ";
    }
    cout << endl;
  }
  cout << "--------------------------" << endl;
}

// uncomment functions as you implement them (part 2)

unsigned int loadVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int start_col, unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int col = start_col;
  int totalEnergy = energy(image, start_col, 0, width, height);
  unsigned int l,f,r = 0;
  seam[0] = start_col;

  for(unsigned int i = 0; i < height; i++) {
    if(i != height-1){
      if(col == width-1)
        l = INT32_MAX;
      else
        l = energy(image, col+1, i+1, width, height);
      f = energy(image, col, i+1, width, height);
      if(col == 0)
        r = INT32_MAX;
      else
        r = energy(image, col-1, i+1, width, height);
    
      if(std::min(f,std::min(l, r)) == f) {
        // cout << "inf" << endl;
        totalEnergy += f;
        seam[i+1] = col;
      }
      else if(std::min(l,std::min(f, r)) == l) {
        // cout << "inl" << endl;
        totalEnergy += l;
        if(col != width-1)
          col++;
        seam[i+1] = col;
      }
      else if(r < l && r < f) {
        // cout << "inr" << endl;
        totalEnergy += r;
        if(col != 0)
          col--;
        seam[i+1] = col;
      }
    }
  }

  return totalEnergy;
}

void printImage(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height) {
  cout<<"printing"<<endl;
  for(unsigned int r = 0; r < height; r++){
    for(unsigned int c = 0; c < width; c++) {
      cout << image[c][r].r << " ";
      // << image[r][c].g << " " << image[r][c].b << ", "
    }
    cout << endl;
  }
  cout << endl;
}

unsigned int loadHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int start_row, unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int row = start_row;
  printImage(image, width, height);
  printEnergies(image, width, height);
  int totalEnergy = energy(image, 0, start_row, width, height);
  unsigned int l,f,r = 0;
  seam[0] = start_row;

  for(unsigned int i = 0; i < width; i++) {
    if(i != width-1){
      if(row == height-1)
        l = INT32_MAX;
      else
        l = energy(image, i+1, row+1, width, height);
      f = energy(image, i+1, row, width, height);
      if(row == 0)
        r = INT32_MAX;
      else
        r = energy(image, i+1, row-1, width, height);
      if(f <= r && f <= l) {
        cout << "inf" << endl;
        cout << f << endl;
        totalEnergy += f;
        seam[i+1] = row;
      }
      else if(l <= r && l < f) {
        cout << "inl" << endl;
        cout << l << endl;
        totalEnergy += l;
        if(row != height-1)
          row++;
        seam[i+1] = row;
      }
      else if(r < l && r < f) {
        cout << "inr" << endl;
        cout << r << endl;
        totalEnergy += r;
        if(row != 0)
          row--;
        seam[i+1] = row;
      }
    }
  }

  cout << totalEnergy << endl;
  return totalEnergy;
}

void findMinVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height, unsigned int seam[]) {
  unsigned int minEnergyCol = 0;
  unsigned int minEnergy = INT32_MAX;
  unsigned int currEnergy = 0;
  for(unsigned int i = 0; i < width; i++) {
    currEnergy = loadVerticalSeam(image, i, width, height, seam);
    if(currEnergy < minEnergy){
      minEnergy = currEnergy;
      minEnergyCol = i;
    }
  }
  loadVerticalSeam(image, minEnergyCol, width, height, seam);
}

void findMinHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int height, unsigned int seam[]) {
  //  TODO: implement (part 2)
  unsigned int minEnergyRow = 0;
  unsigned int minEnergy = INT32_MAX;
  unsigned int currEnergy = 0;
  for(unsigned int i = 0; i < height; i++) {
    currEnergy = loadVerticalSeam(image, i, width, height, seam);
    if(currEnergy < minEnergy){
      minEnergy = currEnergy;
      minEnergyRow = i;
    }
  }
  loadVerticalSeam(image, minEnergyRow, width, height, seam);
}


void removeVerticalSeam(Pixel image[][MAX_HEIGHT], unsigned int& width, unsigned int height, unsigned int verticalSeam[]) {
  Pixel silinecek;
  // printImage(image, width, height);

  // cout << "seam:" << endl;
  // for(unsigned int j = 0; j < height; j++)
  //   cout << verticalSeam[j] << " ";
  // cout << endl << "------------------------------" << endl;

  for (unsigned int r = 0; r < height; r++) {
    silinecek = image[verticalSeam[r]][r];
    for (unsigned int i = verticalSeam[r]; i < width-1; i++) {
      image[i][r] = image[i+1][r];
    }
  }
  width--;
  // printImage(image, width, height);
}

void removeHorizontalSeam(Pixel image[][MAX_HEIGHT], unsigned int width, unsigned int& height, unsigned int horizontalSeam[]) {
  //  TODO: implement (part 2)
  Pixel silinecek;
  for (unsigned int c = 0; c < width; c++) {
    silinecek = image[c][horizontalSeam[c]];
    for (unsigned int i = horizontalSeam[c]; i < height-1; i++) {
      image[c][i] = image[c][i+1];
    }
  }
  height--;

}
