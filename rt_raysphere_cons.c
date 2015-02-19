#include <stdio.h>
#include "lm_rt.h"

int main(){

  int x, y, hit;
  vec3 ro, rd;

  vec3 p0;
  vec3 n;
  float rad;

  // Set up single sphere
  //
  p0.x.f = 40.0f;
  p0.y.f = 40.0f;
  p0.z.f = 20.0f;

  rad = 14.0f;

  // All rays originate from 0,0,0 creating a frustum with one aligned axis
  //
  ro.x.f = 0.0f;
  ro.y.f = 0.0f;
  ro.z.f = 0.0f;

  for( y=0; y<60; y++ ) {
    for( x=0; x<160; x++ ) {

       rd.x.f = (float) x;
       rd.y.f = (float) y;
       rd.z.f = 8.0f;          // pinhole camera with screen at depth 8.0f

       // test rays against sphere
       hit = lm_rt_raysphereint( ro, rd, p0, rad, &n );

       if( hit == 0 ) {
         printf( "." );
       } else {
         printf( "#" );
       }
    }
    printf( "\n" );
  }

  return 0;
}
