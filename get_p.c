#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 a, b, c, p;
  flong bu, bv;

  a.x.l = 0xc4430000;
  a.y.l = 0x437c0000;
  a.z.l = 0xc4380000;

  b.x.l = 0xc4430000;
  b.y.l = 0x437c0000;
  b.z.l = 0xc4580000;

  c.x.l = 0xc42c0000;
  c.y.l = 0x437c0000;
  c.z.l = 0xc4580000;

  bu.l = 0x3d180c11;
  bv.l = 0x3f64a7c2;

//
//    P = A + u(B - A) + v(C - A)
//
  p = lm_vec3_add( a, lm_vec3_add( lm_vec3_scale( bu.f, lm_vec3_sub( b, a ) ), lm_vec3_scale( bv.f, lm_vec3_sub( c, a ) ) ) );

  printf( "A\n" );
  printf( "%08x %f\n", a.x.l, a.x.f );
  printf( "%08x %f\n", a.y.l, a.y.f );
  printf( "%08x %f\n", a.z.l, a.z.f );
  printf( "B\n" );
  printf( "%08x %f\n", b.x.l, b.x.f );
  printf( "%08x %f\n", b.y.l, b.y.f );
  printf( "%08x %f\n", b.z.l, b.z.f );
  printf( "C\n" );
  printf( "%08x %f\n", c.x.l, c.x.f );
  printf( "%08x %f\n", c.y.l, c.y.f );
  printf( "%08x %f\n", c.z.l, c.z.f );
  printf( "P\n" );
  printf( "%08x %f\n", p.x.l, p.x.f );
  printf( "%08x %f\n", p.y.l, p.y.f );
  printf( "%08x %f\n", p.z.l, p.z.f );

  return 0;
}
