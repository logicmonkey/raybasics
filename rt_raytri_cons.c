#include <stdio.h>
#include "lm_rt.h"

int main(){

  int x, y, hit;
  vec3 ro, rd;

  vec3 p0, p1, p2;
  vec3 q0, q1, q2;

  float temp_t, temp_beta, temp_gamma;
  float t, beta, gamma;

  // Set up single triangle
  //
  p0.x.f = 40.0f;
  p0.y.f = 40.0f;
  p0.z.f = 16.0f;

  p1.x.f = 318.0f;
  p1.y.f = 0.0f;
  p1.z.f = 16.0f;

  p2.x.f = 0.0f;
  p2.y.f = 118.0f;
  p2.z.f = 16.0f;

  // OK - and another...
  q0.x.f = 80.0f + 40.0f;
  q0.y.f = 30.0f + 40.0f;
  q0.z.f = 4.0f * 16.0f;

  q1.x.f = 80.0f + 320.0f;
  q1.y.f = 30.0f + 0.0f;
  q1.z.f = 4.0f * 16.0f;

  q2.x.f = 80.0f + 0.0f;
  q2.y.f = 30.0f + 120.0f;
  q2.z.f = 4.0f * 16.0f;

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

       // test rays against two objects P and Q :) 
       //t  = lm_rt_raytriint( ro, rd, p0, p1, p2 ) | lm_rt_raytriint( ro, rd, q0, q1, q2 );
       hit = lm_rt_raytriint( ro, rd, p0, p1, p2, &temp_beta, &temp_gamma, &temp_t);

       if( hit == 0 ) {
         printf( "." );
       } else {
         printf( "#" );
         beta = temp_beta;
         gamma = temp_gamma;
         t = temp_t;
       }
    }
    printf( "\n" );
  }

  printf( "%f %f %f\n", beta, gamma, t );

  return 0;
}
