// vec3 tool kit with multiple precision floats  -=:LogicMonkey:=-
//
#include <gmp.h>
#include <mpfr.h>

typedef union {
  float f;
  long l;
} flong;

typedef struct {
  mpfr_t x, y, z;
} vec3;

void lm_vec3_scale( vec3 *r, mpfr_t a, vec3 b ) {
  mpfr_mul( r->x, a, b.x, MPFR_RNDN );
  mpfr_mul( r->y, a, b.y, MPFR_RNDN );
  mpfr_mul( r->z, a, b.z, MPFR_RNDN );
}

void lm_vec3_add( vec3 *r, vec3 a, vec3 b ) {
  mpfr_add( r->x, a.x, b.x, MPFR_RNDN );
  mpfr_add( r->y, a.y, b.y, MPFR_RNDN );
  mpfr_add( r->z, a.z, b.z, MPFR_RNDN );
}

void lm_vec3_sub( vec3 *r, vec3 a, vec3 b ) {
  mpfr_sub( r->x, a.x, b.x, MPFR_RNDN );
  mpfr_sub( r->y, a.y, b.y, MPFR_RNDN );
  mpfr_sub( r->z, a.z, b.z, MPFR_RNDN );
}

void lm_vec3_dot( mpfr_t *r, vec3 a, vec3 b ) {
  mpfr_t t, u, v;
  mpfr_init( t );
  mpfr_init( u );
  mpfr_init( v );

  mpfr_mul( t, a.x, b.x, MPFR_RNDN );
  mpfr_mul( u, a.y, b.y, MPFR_RNDN );
  mpfr_mul( v, a.z, b.z, MPFR_RNDN );

  mpfr_add( t, t, u, MPFR_RNDN );
  mpfr_add( *r, t, v, MPFR_RNDN );

  mpfr_clear( t );
  mpfr_clear( u );
  mpfr_clear( v );
}

void lm_vec3_cross( vec3 *r, vec3 a, vec3 b ) {
  mpfr_t t, u;
  mpfr_init( t );
  mpfr_init( u );

  /*
   * r->x = a.y*b.z - a.z*b.y;
   * r->y = a.z*b.x - a.x*b.z;
   * r->z = a.x*b.y - a.y*b.x;
   */
  mpfr_mul( t, a.y, b.z, MPFR_RNDN );
  mpfr_mul( u, a.z, b.y, MPFR_RNDN );
  mpfr_sub( r->x, t, u, MPFR_RNDN );

  mpfr_mul( t, a.z, b.x, MPFR_RNDN );
  mpfr_mul( u, a.x, b.z, MPFR_RNDN );
  mpfr_sub( r->y, t, u, MPFR_RNDN );

  mpfr_mul( t, a.x, b.y, MPFR_RNDN );
  mpfr_mul( u, a.y, b.x, MPFR_RNDN );
  mpfr_sub( r->z, t, u, MPFR_RNDN );

  mpfr_clear( t );
  mpfr_clear( u );
}

void lm_vec3_norm( vec3 *r, vec3 a ) {
  mpfr_t m, dot;

  mpfr_init( m );
  mpfr_init( dot );

  lm_vec3_dot( &dot, a, a );
  mpfr_rec_sqrt( m, dot, MPFR_RNDN );

  lm_vec3_scale( r, m, a );

  mpfr_clear( m );
  mpfr_clear( dot );
}

/* convert to float/long, so hex printing is compatible with the standard
// precision version of this function
*/
void lm_vec3_print( vec3 a ) {
  flong t;
  t.f = mpfr_get_flt( a.x, MPFR_RNDN );
  printf( "%08x ", t.l );
  mpfr_out_str( stdout, 10, 0, a.x, MPFR_RNDD );
  printf("\n");
  t.f = mpfr_get_flt( a.y, MPFR_RNDN );
  printf( "%08x ", t.l );
  mpfr_out_str( stdout, 10, 0, a.y, MPFR_RNDD );
  printf("\n");
  t.f = mpfr_get_flt( a.z, MPFR_RNDN );
  printf( "%08x ", t.l );
  mpfr_out_str( stdout, 10, 0, a.z, MPFR_RNDD );
  printf("\n");
}
