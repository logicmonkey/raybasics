#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 p, q, t;
  flong s;

  s.f = 100.0f;

  p.x.f = 1.0f;
  p.y.f = 2.0f;
  p.z.f = 3.0f;

  q = lm_vec3_scale( (float) s.f, p );
  printf( "%f %f %f\n", q.x.f, q.y.f, q.z.f );

  q = lm_vec3_add( q, q );
  printf( "%f %f %f\n", q.x.f, q.y.f, q.z.f );

  t.x.f = 13.0f;
  t.y.f = 12.0f;
  t.z.f = 5.0f;

  q = lm_vec3_cross( p, t );
  printf( "%f %f %f\n", q.x.f, q.y.f, q.z.f );
  printf( "%f\n", lm_vec3_dot( p, t ) );

  q = lm_vec3_norm( q );
  printf( "%f %f %f\n", q.x.f, q.y.f, q.z.f );

  printf( "%08x %08x %08x\n", q.x.l, q.y.l, q.z.l );

  q = lm_vec3_sub( q, p );
  printf( "%f %f %f\n", q.x.f, q.y.f, q.z.f );

  return 0;
}
