#include <stdio.h>
#include "lm_vec3.h"

void lm_plucker( int *s, vec3 a, vec3 b, vec3 ro, vec3 rd ) {
  float l0, l1, l2, l3, l4, l5; // line plucker coords
  float r0, r1, r2, r3, r4, r5; // ray plucker coords
  float side;

  // -- Mahovsky Wyvill --
  l0 = a.x*b.y - b.x*a.y;
  l1 = a.x*b.z - b.x*a.z;
  l2 = a.x - b.x;
  l3 = a.y*b.z - b.y*a.z;
  l4 = a.z - b.z;
  l5 = b.y - a.y;

  r0 = ro.x*rd.y - rd.x*ro.y;
  r1 = ro.x*rd.z - rd.x*ro.z;
  r2 = -rd.x;
  r3 = ro.y*rd.z - rd.y*ro.z;
  r4 = -rd.z;
  r5 = rd.y;

  side = r2*l3 + r5*l1 + r4*l0 + r1*l5 + r0*l4 + r3*l2;

  *s = (side < 0.0f) ? 1 : 0;
}

int main() {

  vec3 a, b, c, d, ro, rd;
  int t0, t1, t2, t3;

  a.x = -10.0f; b.x = 10.0f; c.x =  10.0f; d.x = -10.0f;
  a.y =  10.0f; b.y = 10.0f; c.y = -10.0f; d.y = -10.0f;
  a.z =   1.0f; b.z =  1.0f; c.z =   1.0f; d.z =   1.0f;

  ro.x =  0.0f;
  ro.y =  0.0f;
  ro.z = 10.0f;

// ray 1
  rd.x = -15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );

// ray 3
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

// ray 4
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );

// ray 6
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

// ray 7
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );

// ray 9
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &rd, rd, ro );
  lm_plucker( &t0, a, b, ro, rd );
  lm_plucker( &t1, b, c, ro, rd );
  lm_plucker( &t2, c, d, ro, rd );
  lm_plucker( &t3, d, a, ro, rd );
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

  return 0;
}
