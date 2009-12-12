/*
 *  ppmLoader.h
 *  Project 4.3
 *
 *  Created by Andrew Huynh on 11/5/09.
 */

//! Load a ppm file from disk.
// @input filename The location of the PPM file.  If the file is not found, an error message
//		will be printed and this function will return 0
// @input width This will be modified to contain the width of the loaded image, or 0 if file not found
// @input height This will be modified to contain the height of the loaded image, or 0 if file not found
//
// @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured

unsigned char* loadPPM(const char* filename, int& width, int& height);
