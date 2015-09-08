#ifdef MP
#include "lm_vec3mp.h"
#else
#include "lm_vec3.h"
#endif

// the MIN and MAX macros are defined in sys/param.h but define for portability:
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

//
// Code to perform a ray/triangle intersect test as per Kensler & Shirley [2006]
// K&S's best performing algorithm is copied, but the comments are mine
//                                                           -=:LogicMonkey:=-

int lm_rt_raytriint( vec3 ro, vec3 rd, vec3 p0, vec3 p1, vec3 p2, float *beta, float *gamma, float *t ) {

  // INPUTS
  // vec3 ro, rd;     // ray origin, direction
  // vec3 p0, p1, p2; // test triangle vertex positions

  // INTERNALS
  vec3 edge0;      // triangle edge p0 -> p1
  vec3 edge1;      // triangle edge p2 -> p1
  vec3 normal;     // normal at p0 = e0 X e1
  vec3 edge2;      // tetrahedron edge ro -> p0 (not an obvious variable name)
  vec3 interm;     // normal at ro = e2 X rd used and re-used in volume calcs

#ifdef MP
  mpfr_t v;        // total volume used in t calc and barycentric denominator
  mpfr_t va;       // numerator volume for t calc
  mpfr_t v1;       // numerator volume for beta barycentric
  mpfr_t v2;       // numerator volume for gamma barycentric

  mpfr_init( v );
  mpfr_init( va );
  mpfr_init( v1 );
  mpfr_init( v2 );

  mpfr_init( edge0.x );
  mpfr_init( edge0.y );
  mpfr_init( edge0.z );
  mpfr_init( edge1.x );
  mpfr_init( edge1.y );
  mpfr_init( edge1.z );
  mpfr_init( edge2.x );
  mpfr_init( edge2.y );
  mpfr_init( edge2.z );
  mpfr_init( normal.x );
  mpfr_init( normal.y );
  mpfr_init( normal.z );
  mpfr_init( interm.x );
  mpfr_init( interm.y );
  mpfr_init( interm.z );
#else
  float v;         // total volume used in t calc and barycentric denominator
  float va;        // numerator volume for t calc
  float v1;        // numerator volume for beta barycentric
  float v2;        // numerator volume for gamma barycentric
#endif

  float cmp_v;
  float cmp_v1;
  float cmp_v2;

  // OUTPUTS
  // float t;           // distance to intersection point
  // float beta, gamma; // barycentrics

  // ---------------------------------------------------------------------------

  // Get the two triangle edge vectors and the normal at p0. The normal is used
  // in the signed scalar tiple product volume calcs V = (A X B).C = A.(B X C)
  // The volume calculation is covered in K&S [2006] and also here:
  // http://en.wikipedia.org/wiki/Triple_product#Scalar_triple_product

  lm_vec3_sub( &edge0, p1, p0 ); // vector p0->p1
  lm_vec3_sub( &edge1, p0, p2 ); // vector p2->p0 (note the direction)

  // The actual normal is required (not a unit vector)
  lm_vec3_cross( &normal, edge1, edge0 );

  // At this point we're finished with p1 and p2. Also note that we could have
  // pre-computed E0, E1 and N and supplied them instead of p1, p2.

  lm_vec3_norm( &rd, rd ); // normalise direction in case it hasn't been already

  // Calculate the denominator volume V = (E1 X E0).D = N.D
  lm_vec3_dot( &v, normal, rd );

  // Tetrahedron edge from ray origin to first triangle vertex
  lm_vec3_sub( &edge2, p0, ro );

  // Re-using the normal vector at p0, volume Va = (E1 X E0).E2 = N.E2
  lm_vec3_dot( &va, normal, edge2 );

  // Distance T = Va/V
#ifdef MP
  mpfr_t mp_temp;
  mpfr_init( mp_temp );
  mpfr_div( mp_temp, va, v, MPFR_RNDN );
  *t = mpfr_get_flt( mp_temp, MPFR_RNDN );
#else
  *t = va / v;
#endif

  // Calculate the normal at the ray origin* as this cross product is used
  // more than once. I = D X E2
  // * Note that D and E2 both go through the ray origin by definition
  lm_vec3_cross( &interm, rd, edge2 );

  // Volume V1 = (D X E2).E1 = I.E1
  lm_vec3_dot( &v1, interm, edge1 );

  // Volume V2 = (D X E2).E0 = I.E0
  lm_vec3_dot( &v2, interm, edge0 );

#ifdef MP
  mpfr_div( mp_temp, v1, v, MPFR_RNDN );
  *beta = mpfr_get_flt( mp_temp, MPFR_RNDN );
  mpfr_div( mp_temp, v2, v, MPFR_RNDN );
  *gamma = mpfr_get_flt( mp_temp, MPFR_RNDN );
  cmp_v  = mpfr_get_flt( v, MPFR_RNDN );
  cmp_v1 = mpfr_get_flt( v1, MPFR_RNDN );
  cmp_v2 = mpfr_get_flt( v2, MPFR_RNDN );
  mpfr_clear( edge0.x );
  mpfr_clear( edge0.y );
  mpfr_clear( edge0.z );
  mpfr_clear( edge1.x );
  mpfr_clear( edge1.y );
  mpfr_clear( edge1.z );
  mpfr_clear( edge2.x );
  mpfr_clear( edge2.y );
  mpfr_clear( edge2.z );
  mpfr_clear( normal.x );
  mpfr_clear( normal.y );
  mpfr_clear( normal.z );
  mpfr_clear( interm.x );
  mpfr_clear( interm.y );
  mpfr_clear( interm.z );
  mpfr_clear( mp_temp );
  mpfr_clear( v );
  mpfr_clear( va );
  mpfr_clear( v1 );
  mpfr_clear( v2 );
#else
  *beta  = v1 / v;
  *gamma = v2 / v;
  cmp_v  = v;
  cmp_v1 = v1;
  cmp_v2 = v2;
#endif

  if ((( cmp_v1 < 0.0f && cmp_v2 < 0.0f && cmp_v < 0.0f) || (cmp_v1 > 0.0f && cmp_v2 > 0.0f && cmp_v > 0.0f) ) && ((cmp_v1 + cmp_v2) <= cmp_v) && (*beta >= 0.0f) && (*gamma >= 0.0f) && ((*beta + *gamma) < 1.0f)) {
    return 1;
  }
  return 0;
}

//
// implemenets Kay & Kajiya's slab crossing point algorithm comparing the t values
// along the ray at each bound's extent (equates y = mx + c with R = O + tD)
//
int lm_rt_rayboxint( vec3 ro, vec3 rd, vec3 p0, vec3 p1 ) {

  // INPUTS
  // vec3 ro, rd;     // ray origin, direction
  // vec3 p0, p1;     // test box bounds

  // INTERNALS

  float t0x, t0y, t0z, t1x, t1y, t1z;

  lm_vec3_norm( &rd, rd );

  // ---------------------------------------------------------------------------

  vec3 t0, t1;
#ifdef MP
  mpfr_init( t0.x );
  mpfr_init( t0.y );
  mpfr_init( t0.z );
  mpfr_init( t1.x );
  mpfr_init( t1.y );
  mpfr_init( t1.z );
#endif
  lm_vec3_sub( &t0, p0, ro );
  lm_vec3_sub( &t1, p1, ro );

#ifdef MP
  mpfr_t mp_temp;
  mpfr_init( mp_temp );

  mpfr_div( mp_temp, t0.x, rd.x, MPFR_RNDN );
  t0x = mpfr_get_flt( mp_temp, MPFR_RNDN );
  mpfr_div( mp_temp, t0.y, rd.y, MPFR_RNDN );
  t0y = mpfr_get_flt( mp_temp, MPFR_RNDN );
  mpfr_div( mp_temp, t0.z, rd.z, MPFR_RNDN );
  t0z = mpfr_get_flt( mp_temp, MPFR_RNDN );

  mpfr_div( mp_temp, t1.x, rd.x, MPFR_RNDN );
  t1x = mpfr_get_flt( mp_temp, MPFR_RNDN );
  mpfr_div( mp_temp, t1.y, rd.y, MPFR_RNDN );
  t1y = mpfr_get_flt( mp_temp, MPFR_RNDN );
  mpfr_div( mp_temp, t1.z, rd.z, MPFR_RNDN );
  t1z = mpfr_get_flt( mp_temp, MPFR_RNDN );

  mpfr_clear( mp_temp );
#else

  t0x = t0.x / rd.x;
  t0y = t0.y / rd.y;
  t0z = t0.z / rd.z;

  t1x = t1.x / rd.x;
  t1y = t1.y / rd.y;
  t1z = t1.z / rd.z;
#endif

  float tmin = MAX( MIN( t0x, t1x ), MAX( MIN( t0y, t1y ), MIN( t0z, t1z )));
  float tmax = MIN( MAX( t0x, t1x ), MIN( MAX( t0y, t1y ), MAX( t0z, t1z )));

#ifdef MP
  mpfr_clear( t0.x );
  mpfr_clear( t0.y );
  mpfr_clear( t0.z );
  mpfr_clear( t1.x );
  mpfr_clear( t1.y );
  mpfr_clear( t1.z );
#endif

  if( tmin <= tmax ) {
    return 1;
  }

  return 0;
}

int lm_rt_raysphereint( vec3 ro, vec3 rd, vec3 p0, float rad, vec3 *normal ) {
  vec3 oc, p;
#ifdef MP
  mpfr_init( oc.x );
  mpfr_init( oc.y );
  mpfr_init( oc.z );
  mpfr_init( p.x );
  mpfr_init( p.y );
  mpfr_init( p.z );
  mpfr_t oc_sq, t, gc_sq, hg_sq, invrad, temp;
  mpfr_init( oc_sq );
  mpfr_init(   t   );
  mpfr_init( gc_sq );
  mpfr_init( hg_sq );
  mpfr_init_set_d( invrad, rad, MPFR_RNDN );
  mpfr_init( temp );
#else
  float oc_sq, t, gc_sq, hg_sq, invrad;
#endif

  // ray origin to sphere centre and its squared length
  lm_vec3_sub( &oc, p0, ro );
  lm_vec3_dot( &oc_sq, oc, oc );

  // normalise the ray direction
  lm_vec3_norm( &rd, rd );

  // distance along the ray, of the sphere centre from ray origin
  lm_vec3_dot( &t, oc, rd );

#ifdef MP
  mpfr_mul( temp, t, t, MPFR_RNDN );
  mpfr_sub( gc_sq, oc_sq, temp, MPFR_RNDN );
  mpfr_set_d( temp, rad, MPFR_RNDN );
  mpfr_mul( temp, temp, temp, MPFR_RNDN ); // rad^2
  mpfr_sub( hg_sq, temp, gc_sq, MPFR_RNDN );
  mpfr_sqrt( temp, hg_sq, MPFR_RNDN );
  mpfr_sub( t, t, temp, MPFR_RNDN );
  mpfr_set_d( temp, 1.0f, MPFR_RNDN );
  mpfr_div( invrad, temp, invrad, MPFR_RNDN );
#else
  // pythagoras: oc^2 = gc^2 + t^2
  gc_sq = oc_sq - t*t;

  // r^2 = hg^2 + gc^2
  hg_sq = rad*rad - gc_sq;

  // distance along ray to the intersection point
  t = t - sqrt( hg_sq );

  if( hg_sq < 0.0f ) {
    return 0;
  }

  invrad = 1.0f/rad;
#endif

  // p is the intersection point on the sphere
  lm_vec3_scale( &p, t, rd );
  lm_vec3_add( &p, ro, p );
  // normal is from the centre to the intersection we could normalise
  // with lm_vec3_norm but we know the radius so just divide by that
  lm_vec3_sub( &p, p, p0 );
  lm_vec3_scale( normal, invrad, p );

#ifdef MP
  mpfr_clear( temp );
  mpfr_clear( oc.x );
  mpfr_clear( oc.y );
  mpfr_clear( oc.z );
  mpfr_clear( p.x );
  mpfr_clear( p.y );
  mpfr_clear( p.z );
  mpfr_clear( oc_sq );
  mpfr_clear(   t   );
  mpfr_clear( gc_sq );
  mpfr_clear( invrad );
  if( mpfr_signbit( hg_sq ) ) {
    mpfr_clear( hg_sq );
    return 0;
  }
  mpfr_clear( hg_sq );
#endif

  return 1;
}
