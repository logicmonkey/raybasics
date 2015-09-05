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
  float x, y, z;
} vec3;

void lm_vec3_scale( vec3 *r, float a, vec3 b ) {
  r->x = a * b.x;
  r->y = a * b.y;
  r->z = a * b.z;
}

void lm_vec3_add( vec3 *r, vec3 a, vec3 b ) {
  r->x = a.x + b.x;
  r->y = a.y + b.y;
  r->z = a.z + b.z;
}

void lm_vec3_sub( vec3 *r, vec3 a, vec3 b ) {
  r->x = a.x - b.x;
  r->y = a.y - b.y;
  r->z = a.z - b.z;
}

void lm_vec3_dot( float *r, vec3 a, vec3 b ) {
  *r = a.x*b.x + a.y*b.y + a.z*b.z;
}

void lm_vec3_cross( vec3 *r, vec3 a, vec3 b ) {
  r->x = a.y*b.z - a.z*b.y;
  r->y = a.z*b.x - a.x*b.z;
  r->z = a.x*b.y - a.y*b.x;
}

void lm_vec3_norm( vec3 *r, vec3 a ) {
  float m, dp;
  lm_vec3_dot( &dp, a, a );
  m = 1.0f / sqrt( dp );
  lm_vec3_scale( r, m, a );
}

void lm_vec3_print( vec3 a ) {
  flong t;
  t.f = a.x;
  printf( "%08x %f\n", t.l, t.f );
  t.f = a.y;
  printf( "%08x %f\n", t.l, t.f );
  t.f = a.z;
  printf( "%08x %f\n", t.l, t.f );
}
