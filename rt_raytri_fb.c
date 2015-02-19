#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

#include "lm_rt.h"

int main() {
  // linux frame buffer variables
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;
  int x = 0, y = 0;
  long int location = 0;

  int v, offset, r, g, b;
  unsigned short int colour;

  // the ray tracer variables
  int hit;
  vec3 ro, rd;
  vec3 p0, p1, p2;
  vec3 q0, q1, q2;
  float beta, gamma, t;

  // Open the file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (!fbfd) {
    printf("Error: cannot open framebuffer device.\n");
    exit(1);
  }

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
    printf("Error reading fixed information - permissions on /dev/fb0?\n");
    exit(2);
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
    printf("Error reading variable information.\n");
    exit(3);
  }

  // Figure out the size of the screen in bytes
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
             fbfd, 0);
  if ((int)fbp == -1) {
    printf("Error: failed to map framebuffer device to memory.\n");
    exit(4);
  }

  // Set up single triangle
  //
  p0.x.f = (float) vinfo.xres / 4.0f;
  p0.y.f = (float) vinfo.yres / 4.0f;
  p0.z.f = 16.0f;

  p1.x.f = (float) vinfo.xres * 1.8f - 1.0f;
  p1.y.f = (float) vinfo.yres * 0.9f;
  p1.z.f = 16.0f;

  p2.x.f = (float) vinfo.yres * 0.9f;
  p2.y.f = (float) vinfo.yres * 1.8f - 1.0f;
  p2.z.f = 16.0f;

  // OK - and another...
  q0.x.f = p0.x.f;
  q0.y.f = p0.y.f;
  q0.z.f = p0.z.f * 6.0f;

  q1.x.f = p1.x.f;
  q1.y.f = p1.y.f;
  q1.z.f = p1.z.f * 6.0f;

  q2.x.f = p2.x.f;
  q2.y.f = p2.y.f;
  q2.z.f = p2.z.f * 6.0f;

  // All rays originate from 0,0,0 creating a frustum with one aligned axis
  //
  ro.x.f = 0.0f;
  ro.y.f = 0.0f;
  ro.z.f = 0.0f;

  // Figure out where in memory to put the pixel
  for ( y = 0; y < vinfo.yres; y++ )
    for ( x = 0; x < vinfo.xres; x++ ) {

      location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
             (y+vinfo.yoffset) * finfo.line_length;

      rd.x.f = (float) x;
      rd.y.f = (float) y;
      rd.z.f = 8.0f;          // pinhole camera with screen at depth 8.0f

      rd = lm_vec3_norm( rd );

      // test rays against two objects P and Q :)
      //t  = lm_rt_raytriint( ro, rd, p0, p1, p2 ) | lm_rt_raytriint( ro, rd, q0, q1, q2 );
      hit = lm_rt_raytriint( ro, rd, p0, p1, p2, &beta, &gamma, &t);
      v = (hit == 1) ? (int) 95.0f * ( beta + gamma ) : 0;

      hit = lm_rt_raytriint( ro, rd, q0, q1, q2, &beta, &gamma, &t);
      v = (hit == 1) ? (int) 95.0f * ( beta + gamma ) : v;

      if (v < 0) v = 0;
      if (v > 95) v = 95;
      offset = v % 32;
    
      if (v<32) {
        r = 0, g = 0; b = offset;
      }
      else if (v<64) {
        r= 0; g = offset; b = 31-offset;
      }
      else {
        r = offset; g = 31-offset; b = 0;
      }

      colour = r<<11 | g << 5 | b;
      *((unsigned short int*)(fbp + location)) = colour;

    }
  munmap(fbp, screensize);
  close(fbfd);
  return 0;
}
