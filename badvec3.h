/*

  ** DO NOT USE THIS FILE - THIS IS HOW NOT TO DO IT **

  *tp = *vec3_norm( td );

  o[0].f = tp[0];
  o[1].f = tp[1];
  o[2].f = tp[2];

  printf( "%08x %f\n", o[0].l, o[0].f );
  printf( "%08x %f\n", o[1].l, o[1].f );
  printf( "%08x %f\n", o[2].l, o[2].f );

  vec3_norm returns a pointer to a float, so i put the thing pointed to by that
  pointer into the thing pointed to by tp. tp is a pointer to a union flong.

  so that works. kind of. except it only does it for tp[0] !

  o[1] and o[2] would have to be assigned with a memcpy. rightnow they just get
  junk.

  so let's not use arrays of floats (or arrays of union flong). eh?

 */
#include <stdlib.h>
#include <math.h>

float * vec3_add( float a[3], float b[3] ) {
  float * r;
  r = (float *) malloc( 3*sizeof( float ) );
  r[0] = a[0]+b[0];
  r[1] = a[1]+b[1];
  r[2] = a[2]+b[2];
  return r;
}

float * vec3_scale( float a, float b[3] ) {
  float * r;
  r = (float *) malloc( 3*sizeof( float ) );
  r[0] = a*b[0];
  r[1] = a*b[1];
  r[2] = a*b[2];
  return r;
}

float * vec3_sub( float a[3], float b[3] ) {
  float * r;
  r = (float *) malloc( 3*sizeof( float ) );
  r = vec3_add( a, vec3_scale( -1.0f, b ) );
  return r;
}

float * vec3_dot( float a[3], float b[3] ) {
  float * r;
  r = (float *) malloc( sizeof( float ) );
  *r = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
  return r;
}

float * vec3_cross( float a[3], float b[3] ) {
  float * r;
  r = (float *) malloc( 3*sizeof( float ) );
  r[0] = a[1]*b[2] - a[2]*b[1];
  r[1] = a[2]*b[0] - a[0]*b[2];
  r[2] = a[0]*b[1] - a[1]*b[0];
  return r;
}

float * vec3_norm( float a[3] ) {
  float * r;
  float m;
  m = 1.0f / sqrt( a[0]*a[0] + a[1]*a[1] + a[2]*a[2] );
  r = (float *) malloc( 3*sizeof( float ) );
  r = vec3_scale( m, a ); 
  return r;
}
