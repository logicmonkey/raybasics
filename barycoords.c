#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 a, b, c, p, v0, v1, v2;
  flong bu, bv, t;
  float dot00, dot01, dot11, dot02, dot12, det;

  t.l = 0xc4430000; a.x = t.f;
  t.l = 0x437c0000; a.y = t.f;
  t.l = 0xc4380000; a.z = t.f;

  t.l = 0xc4430000; b.x = t.f;
  t.l = 0x437c0000; b.y = t.f;
  t.l = 0xc4580000; b.z = t.f;

  t.l = 0xc42c0000; c.x = t.f;
  t.l = 0x437c0000; c.y = t.f;
  t.l = 0xc4580000; c.z = t.f;

  t.l = 0xc4281ac4; p.x = t.f;
  t.l = 0x431248db; p.y = t.f;
  t.l = 0xc4220000; p.z = t.f;

  lm_vec3_sub( &v0, b, a );
  lm_vec3_sub( &v1, c, a );
  lm_vec3_sub( &v2, p, a );

  lm_vec3_dot( &dot00, v0, v0 );
  lm_vec3_dot( &dot01, v0, v1 );
  lm_vec3_dot( &dot11, v1, v1 );
  lm_vec3_dot( &dot02, v0, v2 );
  lm_vec3_dot( &dot12, v1, v2 );

  det = dot00 * dot11 - dot01 * dot01;

  bu.f = (dot11 * dot02 - dot01 * dot12) / det;
  bv.f = (dot00 * dot12 - dot01 * dot02) / det;

  printf( "A\n" );
  printf( "%f\n", a.x );
  printf( "%f\n", a.y );
  printf( "%f\n", a.z );
  printf( "B\n" );
  printf( "%f\n", b.x );
  printf( "%f\n", b.y );
  printf( "%f\n", b.z );
  printf( "C\n" );
  printf( "%f\n", c.x );
  printf( "%f\n", c.y );
  printf( "%f\n", c.z );
  printf( "P\n" );
  printf( "%f\n", p.x );
  printf( "%f\n", p.y );
  printf( "%f\n", p.z );
  printf( "BUBV\n" );
  printf( "%08x %f\n", bu.l, bu.f );
  printf( "%08x %f\n", bv.l, bv.f );

  return 0;
}
