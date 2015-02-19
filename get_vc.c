#include <stdio.h>
#include "lm_vec3.h"

int main() {

  vec3 a, b, c, p; 
  flong bu, bv;
  float inv;

  a.x.l = 0xc44e0000;
  a.y.l = 0x43600000;
  a.z.l = 0xc45c0000;
  b.x.l = 0xc4460000;
  b.y.l = 0x43800000;
  b.z.l = 0xc45c0000;
  c.x.l = 0xc4430000;
  c.y.l = 0x43860000;
  c.z.l = 0xc45bf800;
  p.x.l = 0xc4281ac4;
  p.y.l = 0x431248db;
  p.z.l = 0xc4220000;

  bu.l = 0x3d180c11;
  bv.l = 0x3f64a7c2;

//            P - A - u(B - A)
//    C = A + ----------------
//                   v

  inv = 1.0f / bv.f;

  c = lm_vec3_add( a, lm_vec3_scale( inv, lm_vec3_sub( lm_vec3_sub( p, a ), lm_vec3_scale( bu.f, lm_vec3_sub( b, a ) ) ) ) );

  printf( "%08x %f\n", c.x.l, c.x.f );
  printf( "%08x %f\n", c.y.l, c.y.f );
  printf( "%08x %f\n", c.z.l, c.z.f );

  return 0;
}
