#include <stdio.h>
#include "lm_vec3.h"

void lm_a_dot_b_cross_c( int *s, vec3 a, vec3 b, vec3 c, vec3 ro ) {
  vec3 ob, oc, n;
  float dot;

  lm_vec3_sub( &ob, b, ro ); // box
  lm_vec3_sub( &oc, c, ro ); // box

  lm_vec3_cross( &n, ob, oc );
  lm_vec3_dot( &dot, a, n );

  *s = (dot > 0.0f) ? 1 : 0;

}
int main() {

  vec3 a, b, c, d, ro, rd, r;
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
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );

// ray 3
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

// ray 4
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );

// ray 6
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

// ray 7
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );

// ray 9
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;
  lm_vec3_sub( &r, rd, ro );
  lm_a_dot_b_cross_c( &t0, r, a, b, ro );
  lm_a_dot_b_cross_c( &t1, r, b, c, ro );
  lm_a_dot_b_cross_c( &t2, r, c, d, ro );
  lm_a_dot_b_cross_c( &t3, r, d, a, ro );
  printf("%d", t0&t1&t2&t3 );
  printf( "\n" );

  return 0;
}
