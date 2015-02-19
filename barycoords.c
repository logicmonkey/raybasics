#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 a, b, c, p, v0, v1, v2;
  flong bu, bv;
  float dot00, dot01, dot11, dot02, dot12, det;

  a.x.l = 0xc4430000;
  a.y.l = 0x437c0000;
  a.z.l = 0xc4380000;

  b.x.l = 0xc4430000;
  b.y.l = 0x437c0000;
  b.z.l = 0xc4580000;

  c.x.l = 0xc42c0000;
  c.y.l = 0x437c0000;
  c.z.l = 0xc4580000;

  p.x.l = 0xc4281ac4;
  p.y.l = 0x431248db;
  p.z.l = 0xc4220000;

  v0 = lm_vec3_sub( b, a );
  v1 = lm_vec3_sub( c, a );
  v2 = lm_vec3_sub( p, a );
  
  dot00 = lm_vec3_dot( v0, v0 );
  dot01 = lm_vec3_dot( v0, v1 );
  dot11 = lm_vec3_dot( v1, v1 );
  dot02 = lm_vec3_dot( v0, v2 );
  dot12 = lm_vec3_dot( v1, v2 );

  det = dot00 * dot11 - dot01 * dot01;

  bu.f = (dot11 * dot02 - dot01 * dot12) / det;
  bv.f = (dot00 * dot12 - dot01 * dot02) / det;

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
  printf( "BUBV\n" );
  printf( "%08x %f\n", bu.l, bu.f );
  printf( "%08x %f\n", bv.l, bv.f );

  return 0;
}
