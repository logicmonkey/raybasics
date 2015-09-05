#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 a, b, c, ab, ac, bc, n;
  flong t;

  t.l = 0xbecf72f9; a.x = t.f;
  t.l = 0x3ed8c50c; a.y = t.f;
  t.l = 0xbf4f6fc4; a.z = t.f;

  t.l = 0x44281ac3; b.x = t.f;
  t.l = 0xc31248d5; b.y = t.f;
  t.l = 0x4421ffff; b.z = t.f;

  t.l = 0x44258246; c.x = t.f;
  t.l = 0xc3076f73; c.y = t.f;
  t.l = 0x441ccf19; c.z = t.f;

  lm_vec3_sub( &ab, b, a );

  lm_vec3_sub( &ac, c, a );

  // coplanar test (AxB).C = 0
  // i.e. is C also perp to the normal of A and B?
  //      or is the ABC parallelepiped volume zero?

  // normal calc
  //
  lm_vec3_cross( &n, ac, ab );
#ifdef UNITNORMAL
  lm_vec3_norm( &n, n );
#endif

  // it's a triangle - BC should always be coplanar

  // vector to be tested against the normal
  //
  lm_vec3_sub( &bc, c, b );

  lm_vec3_dot( (float *) &t, n, bc );

  printf( "%f\n", t.f );
  printf( "%x\n", t.l );

  return 0;
}
