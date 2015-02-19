#include <stdio.h>
#include "lm_rt.h"

int main(){

  int x, y, t;
  vec3 ro, rd;

  vec3 p0, p1;

  // Set up single triangle
  //
  p0.x.f = 40.0f;
  p0.y.f = 40.0f;
  p0.z.f = 16.0f;

  p1.x.f = p0.x.f + 40.0f;
  p1.y.f = p0.y.f + 50.0f;
  p1.z.f = p0.z.f + 60.0f;

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

       // test rays against box
       t  = lm_rt_rayboxint( ro, rd, p0, p1 );

       if( t == 0 ) {
         printf( "." );
       } else {
         printf( "#" );
       }
    }
    printf( "\n" );
  }

  return 0;
}
