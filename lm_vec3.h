// vec3 tool kit -=:LogicMonkey:=-
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef union {
  float f;
  long l;
} flong;

typedef struct {
  flong x, y, z;
} vec3;

vec3 lm_vec3_scale( float a, vec3 b ) {
  vec3 r;
  r.x.f = a * b.x.f;
  r.y.f = a * b.y.f;
  r.z.f = a * b.z.f;
  return r;
}

vec3 lm_vec3_add( vec3 a, vec3 b ) {
  vec3 r;
  r.x.f = a.x.f+b.x.f;
  r.y.f = a.y.f+b.y.f;
  r.z.f = a.z.f+b.z.f;
  return r;
}

vec3 lm_vec3_sub( vec3 a, vec3 b ) {
  vec3 r;
  r = lm_vec3_add( a, lm_vec3_scale( -1.0f, b ) );
  return r;
}

float lm_vec3_dot( vec3 a, vec3 b ) {
  float r;
  r = a.x.f*b.x.f + a.y.f*b.y.f + a.z.f*b.z.f;
  return r;
}

vec3 lm_vec3_cross( vec3 a, vec3 b ) {
  vec3 r;
  r.x.f = a.y.f*b.z.f - a.z.f*b.y.f;
  r.y.f = a.z.f*b.x.f - a.x.f*b.z.f;
  r.z.f = a.x.f*b.y.f - a.y.f*b.x.f;
  return r;
}

vec3 lm_vec3_norm( vec3 a ) {
  vec3 r;
  float m;
  m = 1.0f / sqrt( a.x.f*a.x.f + a.y.f*a.y.f + a.z.f*a.z.f );
  r = lm_vec3_scale( m, a ); 
  return r;
}

void lm_vec3_print( vec3 a ) {
  printf( "%08x %f\n", a.x.l, a.x.f );
  printf( "%08x %f\n", a.y.l, a.y.f );
  printf( "%08x %f\n", a.z.l, a.z.f );
}
