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

int lm_rt_lmrayboxint( vec3 ro, vec3 rd, vec3 v0, vec3 v7, int debug ) {
  vec3 ar, bo, v1, v2, v3, v4, v5, v6;
  int t01, t12, t23, t30, t45, t56, t67, t74, t14, t72, t36, t50;
  int a, b, c, d, e, f;

/*------------------------------------------------------------------------------
   Ray/Box intersection test

    A purely geometric approach is taken without using Pluecker coordinates or
    floating point division.

       Vertex numbering - v0 and v7 diagonally opposite

         6-----------5
        /.          /|        ^
       3-----------0 |       y|
       | .         | |        |
       | .         | |        |/           4-----------5
       | .         | |      --+------->    |     45    |
       | 7.........|.4       /|      x     |           |
       |.          |/       /              |74   F   56|
       2-----------1      z/               |           |
                                           |     67    |
       6-----------5-----------4-----------7-----------6
       |    ~56    |    ~45    |    ~74    |    ~67    |
       |           |           |           |           |
       |~36  B  ~50|50   C   14|~14  D  ~72|72   E   36|
       |           |           |           |           |
       |    ~30    |    ~01    |    ~12    |    ~23    |
       3-----------0-----------1-----------2-----------3
       |     30    |
       |           |
       |23   A   01|
       |           |
       |     12    |
       2-----------1

    Geometric View of Intersection:
                                           ->
                                        ro+Rd
                                          *
                                         ,.
                                      , / .
                                   ,   /  .  ->      ->
                                ,     /   .  Ar = ro+Rd - v
                             ,       /    .       -> ->
                          ,         /     .     = Bo+Rd
                     v' *- - - - - / - - -* v
                         .        /     ,'
                                 /    ,'
                          .     /   ,'       ->
                               /  ,'         Bo = ro-v
                           .  / ,'
                             /,'
                            .'
                           ro

      v is a known vertex on the AA box, then the vector vv' = v'-v has a single
      x, y or z component depending on v' position relative to v.

      A ray has origin ro and direction vector Rd.

      Rd can be a normalised (unit) direction vector.

      Tetrahedron edge vectors Ar and Bo are easily found:

          Bo = ro - v
          Ar = Bo + Rd

      A signed parallelepiped volume is represented by the scalar triple product

          vol =  vv'.(Ar x Bo)

      where
                        ,              .
                       |   i    j    k  |
          Ar x Bo = det| Ar.x Ar.y Ar.z |
                       | Bo.x Bo.y Bo.z |
                        `              '

      The orientation of Rd relative to vv' is given by the sign of the volume.
      Since vv' is sparse, the scalar triple product uses just one cofactor of
      the determinant. Only the sign of the vv' component is required and this
      is implicit within the AA box vertex ordering.

      The relative orientation indicates which side of a box edge a ray passes.

      AA box faces are tested one by one but this could be done concurrently
      in hardware.

      Cofactors could use a magnitude comparison rather than a subtract and
      sign check.

    Flop summary:

      12 box edges X
        3 FP fpsub (Bo)
        3 FP fpadd (Ar)
        2 FP fpmul (cofactor)
        1 FP fpsub | mag comparator

        TOTAL 24 fpmul + 84 fpadd

    Error Propagation:

      Box vertex positions v are assumed to be exact.
      Ray origin ro and direction Rd are assumed to be exact.

      The box vertex position relative to the ray origin is calculated as the
      vector Bo. Any error in Bo is directly propagated to vector Ar which
      additionally picks up any error from adding Rd.

      Errors in Bo and Ar are multiplied before applying the sign test.

    Alternative Calculation of Vector Ar

      Per box, the point r = ro+Rd can be pre-calculated with 3 FP adds.
      Per edge test, vector Ar = r - v (3 FP subs)

      This is an overhead of 3 FP adds per box, but it removes Ar's dependence
      on Bo and its error.

      This approach does not look as though it carries any real benefit over
      the one already taken.

      Other pre-calculations could be possible since the box vertices and ray
      origin position are fixed points in space.

------------------------------------------------------------------------------*/

  // copy the appropriate x,y,z positions for V1...V6 from V0 and V7
  v1   = v0;
  v1.y = v7.y;

  v2   = v7;
  v2.z = v1.z;

  v3   = v0;
  v3.x = v7.x;

  v6   = v7;
  v6.y = v0.y;

  v5   = v0;
  v5.z = v7.z;

  v4   = v1;
  v4.z = v7.z;

  // Face A

  lm_vec3_sub( &bo, ro, v0 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t01 = ((ar.z*bo.x-ar.x*bo.z) > 0) ? 1 : 0; // edge vector y component -ve

  lm_vec3_sub( &bo, ro, v1 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t12 = ((ar.y*bo.z-ar.z*bo.y) > 0) ? 1 : 0; // edge vector x component -ve

  lm_vec3_sub( &bo, ro, v2 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t23 = ((ar.z*bo.x-ar.x*bo.z) < 0) ? 1 : 0; // edge vector y component +ve

  lm_vec3_sub( &bo, ro, v3 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t30 = ((ar.y*bo.z-ar.z*bo.y) < 0) ? 1 : 0; // edge vector x component +ve

  // Face F

  lm_vec3_sub( &bo, ro, v6 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t67 = ((ar.z*bo.x-ar.x*bo.z) > 0) ? 1 : 0; // edge vector y component -ve

  lm_vec3_sub( &bo, ro, v7 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t74 = ((ar.y*bo.z-ar.z*bo.y) < 0) ? 1 : 0; // edge vector x component +ve

  lm_vec3_sub( &bo, ro, v4 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t45 = ((ar.z*bo.x-ar.x*bo.z) < 0) ? 1 : 0; // edge vector y component +ve

  lm_vec3_sub( &bo, ro, v5 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t56 = ((ar.y*bo.z-ar.z*bo.y) > 0) ? 1 : 0; // edge vector x component -ve

  // Face B
  lm_vec3_sub( &bo, ro, v3 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t36 = ((ar.y*bo.x-ar.x*bo.y) > 0) ? 1 : 0; // edge vector z component -ve

  lm_vec3_sub( &bo, ro, v5 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t50 = ((ar.y*bo.x-ar.x*bo.y) < 0) ? 1 : 0; // edge vector z component +ve

  // Face D
  lm_vec3_sub( &bo, ro, v4 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t14 = ((ar.y*bo.x-ar.x*bo.y) > 0) ? 1 : 0; // edge vector z component -ve

  lm_vec3_sub( &bo, ro, v7 ); // vertex to ray origin
  lm_vec3_add( &ar, rd, bo ); // vertex to ray via ray origin

  t72 = ((ar.y*bo.x-ar.x*bo.y) < 0) ? 1 : 0; // edge vector z component +ve

  if( debug==1 ) {
    printf( "t01: %d\n", t01 );
    printf( "t12: %d\n", t12 );
    printf( "t23: %d\n", t23 );
    printf( "t30: %d\n", t30 );
    printf( "t45: %d\n", t45 );
    printf( "t56: %d\n", t56 );
    printf( "t67: %d\n", t67 );
    printf( "t74: %d\n", t74 );
    printf( "t50: %d\n", t50 );
    printf( "t14: %d\n", t14 );
    printf( "t72: %d\n", t72 );
    printf( "t36: %d\n", t36 );
  }

  a = ( t01 &  t12 &  t23 &  t30 ) & 1;
  b = (~t50 & ~t30 & ~t36 & ~t56 ) & 1;
  c = ( t14 & ~t01 &  t50 & ~t45 ) & 1;
  d = (~t72 & ~t12 & ~t14 & ~t74 ) & 1;
  e = ( t36 & ~t23 &  t72 & ~t67 ) & 1;
  f = ( t45 &  t56 &  t67 &  t74 ) & 1;

  return a|b|c|d|e|f;
}

void lm_plucker( int *s, vec3 a, vec3 b, vec3 ro, vec3 rd ) {
  float l0, l1, l2, l3, l4, l5; // line Plücker coords
  float r0, r1, r2, r3, r4, r5; // ray Plücker coords
  float side;

  // -- Mahovsky Wyvill -- 14 FP adds, 12 FP mul
  l0 = a.x*b.y - b.x*a.y;
  l1 = a.x*b.z - b.x*a.z;
  l2 = a.x - b.x;
  l3 = a.y*b.z - b.y*a.z;
  l4 = a.z - b.z;
  l5 = b.y - a.y;

  r0 = ro.x*rd.y - rd.x*ro.y;
  r1 = ro.x*rd.z - rd.x*ro.z;
  r2 = -rd.x;
  r3 = ro.y*rd.z - rd.y*ro.z;
  r4 = -rd.z;
  r5 = rd.y;

  side = r2*l3 + r5*l1 + r4*l0 + r1*l5 + r0*l4 + r3*l2;

  *s = (side < 0.0f) ? 1 : 0;
}

int lm_raybox_plucker( vec3 ro, vec3 rd, vec3 v0, vec3 v7 ) {

  vec3 v1, v2, v3, v4, v5, v6;
  int t01, t12, t23, t30, t45, t56, t67, t74, t14, t72, t36, t50;
  // six face test results
  int a, b, c, d, e, f;

/*------------------------------------------------------------------------------
   Ray/Box intersection test


       Vertex numbering - v0 and v7 diagonally opposite

         6-----------5
        /.          /|        ^
       3-----------0 |       y|
       | .         | |        |
       | .         | |        |/           4-----------5
       | .         | |      --+------->    |     45    |
       | 7.........|.4       /|      x     |           |
       |.          |/       /              |74   F   56|
       2-----------1      z/               |           |
                                           |     67    |
       6-----------5-----------4-----------7-----------6
       |    ~56    |    ~45    |    ~74    |    ~67    |
       |           |           |           |           |
       |~36  B  ~50|50   C   14|~14  D  ~72|72   E   36|
       |           |           |           |           |
       |    ~30    |    ~01    |    ~12    |    ~23    |
       3-----------0-----------1-----------2-----------3
       |     30    |
       |           |
       |23   A   01|
       |           |
       |     12    |
       2-----------1

------------------------------------------------------------------------------*/
  // copy the appropriate x,y,z positions for V1...V6 from V0 and V7
  v1   = v0;
  v1.y = v7.y;

  v2   = v7;
  v2.z = v1.z;

  v3   = v0;
  v3.x = v7.x;

  v6   = v7;
  v6.y = v0.y;

  v5   = v0;
  v5.z = v7.z;

  v4   = v1;
  v4.z = v7.z;
  // box is now set up from the initial two corners

  lm_plucker( &t01, v0, v1, ro, rd);
  lm_plucker( &t12, v1, v2, ro, rd);
  lm_plucker( &t23, v2, v3, ro, rd);
  lm_plucker( &t30, v3, v0, ro, rd);

  lm_plucker( &t67, v6, v7, ro, rd);
  lm_plucker( &t74, v7, v4, ro, rd);
  lm_plucker( &t45, v4, v5, ro, rd);
  lm_plucker( &t56, v5, v6, ro, rd);

  lm_plucker( &t36, v3, v6, ro, rd);
  lm_plucker( &t50, v5, v0, ro, rd);
  lm_plucker( &t14, v1, v4, ro, rd);
  lm_plucker( &t72, v7, v2, ro, rd);

  a = ( t01 &  t12 &  t23 &  t30 ) & 1;
  b = ( t50 & ~t30 &  t36 & ~t56 ) & 1;
  c = (~t14 & ~t01 & ~t50 & ~t45 ) & 1;
  d = ( t72 & ~t12 &  t14 & ~t74 ) & 1;
  e = (~t36 & ~t23 & ~t72 & ~t67 ) & 1;
  f = ( t45 &  t56 &  t67 &  t74 ) & 1;

  return a|b|c|d|e|f;
}

int lm_raybox_plucker_optimised( vec3 ro, vec3 rd, vec3 v0, vec3 v7 ) {

  vec3 v1, v2, v3, v4, v5, v6;
  float r0, r1, r2, r3, r4, r5; // ray Plücker coords
  float l0, l1, l2, l3, l4, l5; // line Plücker coords

  int t01, t12, t23, t30, t45, t56, t67, t74, t14, t72, t36, t50;
  // six face test results
  int a, b, c, d, e, f;

  // copy the appropriate x,y,z positions for V1...V6 from V0 and V7
  v1   = v0;
  v1.y = v7.y;

  v2   = v7;
  v2.z = v1.z;

  v3   = v0;
  v3.x = v7.x;

  v6   = v7;
  v6.y = v0.y;

  v5   = v0;
  v5.z = v7.z;

  v4   = v1;
  v4.z = v7.z;

/*------------------------------------------------------------------------------
   Ray/Box intersection test

   Optimised Plücker coordinates

       Vertex numbering - v0 and v7 diagonally opposite

         6-----------5
        /.          /|        ^
       3-----------0 |       y|
       | .         | |        |
       | .         | |        |/           4-----------5
       | .         | |      --+------->    |     45    |
       | 7.........|.4       /|      x     |           |
       |.          |/       /              |74   F   56|
       2-----------1      z/               |           |
                                           |     67    |
       6-----------5-----------4-----------7-----------6
       |    ~56    |    ~45    |    ~74    |    ~67    |
       |           |           |           |           |
       |36   B   50|~50  C  ~14|14   D   72|~72  E  ~36|
       |           |           |           |           |
       |    ~30    |    ~01    |    ~12    |    ~23    |
       3-----------0-----------1-----------2-----------3
       |     30    |
       |           |
       |23   A   01|
       |           |
       |     12    |
       2-----------1

------------------------------------------------------------------------------*/
  // box is now set up from the initial two corners

  r0 = ro.x*rd.y - rd.x*ro.y;
  r1 = ro.x*rd.z - rd.x*ro.z;
  r2 = -rd.x;
  r3 = ro.y*rd.z - rd.y*ro.z;
  r4 = -rd.z;
  r5 = rd.y;

  t01 = ( r2*v0.z - r4*v0.x - r1 ) < 0 ? 1 : 0;
  t12 = ( r5*v1.z + r4*v1.y + r3 ) < 0 ? 1 : 0;
  t23 = (-r2*v2.z + r4*v2.x + r1 ) < 0 ? 1 : 0;
  t30 = (-r5*v3.z - r4*v3.y - r3 ) < 0 ? 1 : 0;

  t67 = ( r2*v6.z - r4*v6.x - r1 ) < 0 ? 1 : 0;
  t74 = (-r5*v7.z - r4*v7.y - r3 ) < 0 ? 1 : 0;
  t45 = (-r2*v4.z + r4*v4.x + r1 ) < 0 ? 1 : 0;
  t56 = ( r5*v5.z + r4*v5.y + r3 ) < 0 ? 1 : 0;

  t36 = (-r2*v3.y - r5*v3.x + r0 ) < 0 ? 1 : 0;
  t50 = ( r2*v5.y + r5*v5.x - r0 ) < 0 ? 1 : 0;
  t14 = (-r2*v1.y - r5*v1.x + r0 ) < 0 ? 1 : 0;
  t72 = ( r2*v7.y + r5*v7.x - r0 ) < 0 ? 1 : 0;

  a = ( t01 &  t12 &  t23 &  t30 ) & 1;
  b = ( t50 & ~t30 &  t36 & ~t56 ) & 1;
  c = (~t14 & ~t01 & ~t50 & ~t45 ) & 1;
  d = ( t72 & ~t12 &  t14 & ~t74 ) & 1;
  e = (~t36 & ~t23 & ~t72 & ~t67 ) & 1;
  f = ( t45 &  t56 &  t67 &  t74 ) & 1;

  return a|b|c|d|e|f;
}
