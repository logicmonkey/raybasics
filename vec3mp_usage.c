#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>
#include "lm_vec3mp.h"

void main(){
  vec3 a, b, c;
  mpfr_t dot;
  flong t;

  mpfr_init_set_d( a.x, 1.0f, MPFR_RNDN );
  mpfr_init_set_d( a.y, 2.0f, MPFR_RNDN );
  mpfr_init_set_d( a.z, 3.0f, MPFR_RNDN );

  mpfr_init_set_d( b.x, 3.0f, MPFR_RNDN );
  mpfr_init_set_d( b.y, 2.0f, MPFR_RNDN );
  mpfr_init_set_d( b.z, 1.0f, MPFR_RNDN );

  t.l = 0x3f800000;
  mpfr_init_set_d( c.x, t.f, MPFR_RNDN );
  mpfr_init_set_d( c.y, t.f, MPFR_RNDN );
  mpfr_init_set_d( c.z, t.f, MPFR_RNDN );
  lm_vec3_print( c );

  mpfr_init_set_d( dot,  0.0f, MPFR_RNDN );

  lm_vec3_add( &a, a, a );
  lm_vec3_print( a );

  lm_vec3_dot( &dot, a, a );

  mpfr_out_str( stdout, 10, 0, dot, MPFR_RNDD );
  printf("\n");

  lm_vec3_cross( &c, a, b );
  lm_vec3_print( c );

  lm_vec3_scale( &c, dot, c );
  lm_vec3_print( c );

  lm_vec3_norm( &c, c );
  lm_vec3_print( c );

  t.f = mpfr_get_flt( c.x, MPFR_RNDN );
  printf( "%f\n\%08x\n", t.f, t.l );

  mpfr_clear( a.x );
  mpfr_clear( a.y );
  mpfr_clear( a.z );
  mpfr_clear( b.x );
  mpfr_clear( b.y );
  mpfr_clear( b.z );
  mpfr_clear( c.x );
  mpfr_clear( c.y );
  mpfr_clear( c.z );
  mpfr_clear( dot );
}
