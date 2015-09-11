#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 o, a, b, c, d, ro, rd, r;
  int t0, t1, t2, t3;

  o.x = 0.0f;
  o.y = 0.0f;
  o.z = 0.0f;

  a.x = -10.0f;
  a.y =  10.0f;
  a.z =   1.0f;

  b.x =  10.0f;
  b.y =  10.0f;
  b.z =   1.0f;

  c.x =  10.0f;
  c.y = -10.0f;
  c.z =   1.0f;

  d.x = -10.0f;
  d.y = -10.0f;
  d.z =   1.0f;

  ro.x =  0.0f;
  ro.y =  0.0f;
  ro.z = 10.0f;

// ray 1
  rd.x = -15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, ro, a );
  lm_vec3_sub( &r, rd, A );
  t0 = (1 * () > 0) ? 0 : 1;
  printf("%d", t0&t1&t2&t3 );

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );

// ray 3
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

// ray 4
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );

// ray 6
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

// ray 7
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );

// ray 9
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

  return 0;
}
