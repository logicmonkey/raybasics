#include <stdio.h>
#include "lm_vec3.h"

void  main() {

  vec3 a, b, c, ab, ac, bc, n;
  flong vtp;

  a.x.l = 0xbecf72f9;
  a.y.l = 0x3ed8c50c;
  a.z.l = 0xbf4f6fc4;

  b.x.l = 0x44281ac3;
  b.y.l = 0xc31248d5;
  b.z.l = 0x4421ffff;

  c.x.l = 0x44258246;
  c.y.l = 0xc3076f73;
  c.z.l = 0x441ccf19;

  ab = lm_vec3_sub( b, a );
//ab = lm_vec3_norm( ab );

  ac = lm_vec3_sub( c, a );
//ac = lm_vec3_norm( ac );

  // coplanar test (AxB).C = 0
  // i.e. is C also perp to the normal of A and B?
  //      or is the ABC parallelepiped volume zero?

  // normal calc
  //
  n = lm_vec3_cross( ac, ab );
  n = lm_vec3_norm( n );

  // it's a triangle - BC should always be coplanar

  // vector to be tested against the normal
  //
  bc = lm_vec3_sub( c, b );
//bc = lm_vec3_norm( bc );

  vtp.f = lm_vec3_dot( n, bc );

  printf( "%f\n", vtp.f );

/*
  lm_vec3_print( n );
  lm_vec3_print( bc );
 */
}
