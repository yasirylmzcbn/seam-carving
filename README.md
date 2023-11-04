# Seam Carver

#### This is a C++ program that can perform seam carving on ppm images to eliminate vertical seams from the image based on the seams' energy levels in relation to the seams around them.

### Usage:
Clone the repo:
```git clone ``` <br><br>
Enter the directory:
```cd seam-carving``` <br><br>
Compile and run:
```g++ functions.cpp seamcarving.cpp``` ```./a.out``` <br><br>
Enter the name of one of the ppm files in source_images or upload your own (including the file extension):<br><br>
Example: ```Fishing40x27.ppm```<br><br>
Enter target width and height (make sure they are less than or equal to the original dimensions) <br><br>
Example: ```35 25``` <br><br>
The output will be named carved{targetWidth}x{targetHeight}.ppm and will be found in the carved_images directory. <br><br><br>

If you are interested in how seam carving works, you can read more about it here: https://en.wikipedia.org/wiki/Seam_carving
