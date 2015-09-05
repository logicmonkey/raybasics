// LibPNG example :: A.Greensted :: http://www.labbookpages.co.uk

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <png.h>
#include "lm_rt.h"

// This takes the float value 'val', converts it to red, green & blue values, then
// sets those values into the image memory buffer location pointed to by 'ptr'
inline void setRGB(png_byte *ptr, float val);

// This function actually writes out the PNG image file. The string 'title' is
// also written into the image file
int writeImage(char* filename, int width, int height, float *buffer, char* title);

float *lm_rt_primary_rays( int width, int height );

int main(int argc, char *argv[]) {
  // Make sure that the output filename argument has been provided
  if (argc != 2) {
    fprintf(stderr, "Please specify output file\n");
    return 1;
  }

  int width = 640;
  int height = 480;

  // Create image - a 1D array of floats, length: width * height
  float *buffer = lm_rt_primary_rays( width, height );
  if (buffer == NULL) {
    return 1;
  }

  // Save the image to a PNG file
  int result = writeImage(argv[1], width, height, buffer, "This is my test image");

  free(buffer);

  return result;
}

inline void setRGB(png_byte *ptr, float val) {
  int v = (int)(val * 767);
  if (v < 0) v = 0;
  if (v > 767) v = 767;
  int offset = v % 256;

  if (v<256) {
    ptr[0] = 0; ptr[1] = 0; ptr[2] = offset;
  }
  else if (v<512) {
    ptr[0] = 0; ptr[1] = offset; ptr[2] = 255-offset;
  }
  else {
    ptr[0] = offset; ptr[1] = 255-offset; ptr[2] = 0;
  }
}

int writeImage(char* filename, int width, int height, float *buffer, char* title) {
  int code = 0;
  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;
  png_bytep row;

  // Open file for writing (binary mode)
  fp = fopen(filename, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file %s for writing\n", filename);
    code = 1;
    goto finalise;
  }

  // Initialize write structure
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL) {
    fprintf(stderr, "Could not allocate write struct\n");
    code = 1;
    goto finalise;
  }

  // Initialize info structure
  info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL) {
    fprintf(stderr, "Could not allocate info struct\n");
    code = 1;
    goto finalise;
  }

  // Setup Exception handling
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "Error during png creation\n");
    code = 1;
    goto finalise;
  }

  png_init_io(png_ptr, fp);

  // Write header (8 bit colour depth)
  png_set_IHDR(png_ptr, info_ptr, width, height,
      8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
      PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  // Set title
  if (title != NULL) {
    png_text title_text;
    title_text.compression = PNG_TEXT_COMPRESSION_NONE;
    title_text.key = "Title";
    title_text.text = title;
    png_set_text(png_ptr, info_ptr, &title_text, 1);
  }

  png_write_info(png_ptr, info_ptr);

  // Allocate memory for one row (3 bytes per pixel - RGB)
  row = (png_bytep) malloc(3 * width * sizeof(png_byte));

  // Write image data
  int x, y;
  for (y=0 ; y<height ; y++) {
    for (x=0 ; x<width ; x++) {
      setRGB(&(row[x*3]), buffer[y*width + x]);
    }
    png_write_row(png_ptr, row);
  }

  // End write
  png_write_end(png_ptr, NULL);

  finalise:
  if (fp != NULL) fclose(fp);
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
  if (row != NULL) free(row);

  return code;
}

float *lm_rt_primary_rays( int width, int height ) {

  int x, y, hit;
  vec3 ro, rd;

  vec3 p0, n;
  float rad;

  float *buffer = (float *) malloc(width * height * sizeof(float));

  if (buffer == NULL) {
    fprintf(stderr, "Could not create image buffer\n");
    return NULL;
  }

  // Set up single sphere
  //
  p0.x = (float) width / 4.0f;
  p0.y = (float) height / 4.0f;
  p0.z = (float) width / 8.0f;

  rad = 0.965 * p0.z;

  // All rays originate from 0,0,0 creating a frustum with one aligned axis
  //
  ro.x = 0.0f;
  ro.y = 0.0f;
  ro.z = 0.0f;

  for( y=0; y<height; y++ ) {
    for( x=0; x<width; x++ ) {

      rd.x = (float) x;
      rd.y = (float) y;
      rd.z = 8.0f;          // pinhole camera with screen at depth 8.0f

      lm_vec3_norm( &rd, rd );

      // test rays against two objects P and Q :)
      hit = lm_rt_raysphereint( ro, rd, p0, rad, &n );

      buffer[ y * width + x ] = (hit == 1) ? sqrt(n.x*n.z + n.y*n.z) : 0.0f;
    }
  }
  return buffer;
}
