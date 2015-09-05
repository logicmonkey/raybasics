#include <stdio.h>
#include "lm_vec3.h"

void main() {

  vec3 o, d;
  flong t;

  t.l = 0xbecf72f9; d.x = t.f;
  t.l = 0x3ed8c50c; d.y = t.f;
  t.l = 0xbf4f6fc4; d.z = t.f;

  t.l = 0x44281ac3; o.x = t.f;
  t.l = 0xc31248d5; o.y = t.f;
  t.l = 0x4421ffff; o.z = t.f;

/*
  o.x.l = 0x44258246;
  o.y.l = 0xc3076f73;
  o.z.l = 0x441ccf19;
*/

  t.l = 0x4379b1f1;
  t.l = 0x41cd0077;

  /*
   *    O' = O + tD        ::  where t is the distance in normalised direction D
   *    o = lm_vec3_add( o, lm_vec3_scale( t.f, lm_vec3_norm( d )));
   */

  lm_vec3_norm( &d, d );
  lm_vec3_scale( &d, t.f, d );
  lm_vec3_add( &o, o, d );

  lm_vec3_print( o );
}
