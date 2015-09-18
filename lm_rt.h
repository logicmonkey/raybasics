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

      Per box, the point r = ro+Rd can be pre-caclulated with 3 FP adds.
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

int lm_rt_pcrayboxint( vec3 ro, vec3 rd, vec3 v0, vec3 v7, int debug ) {
  vec3 raydir, raymom;

  vec3 v1, v2, v3, v4, v5, v6, diagonal;
  vec3 dir01, dir12, dir23, dir30, dir45, dir56, dir67, dir74, dir14, dir72, dir36, dir50;
  vec3 mom01, mom12, mom23, mom30, mom45, mom56, mom67, mom74, mom14, mom72, mom36, mom50;
  float w01, w12, w23, w30, w45, w56, w67, w74, w14, w72, w36, w50;
  float sw01, sw12, sw23, sw30, sw45, sw56, sw67, sw74, sw14, sw72, sw36, sw50;
  int t01, t12, t23, t30, t45, t56, t67, t74, t14, t72, t36, t50;
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

  Plucker Coordinates { direction : moment }

      R   = { Rd - ro : Rd x ro }
      Enm = { vn - vm : vn x vm }

------------------------------------------------------------------------------*/

  lm_vec3_sub( &raydir, rd, ro );

  lm_vec3_cross( &raymom, rd, ro );

  // copy the appropriate x,y,z positions for V1...V6 from V0 and V7
  lm_vec3_sub( &diagonal, v0, v7 );

  v1 = {0.0f, 0.0f, 0.0f};
  v2 = {0.0f, 0.0f, 0.0f};
  v3 = {0.0f, 0.0f, 0.0f};
  v4 = {0.0f, 0.0f, 0.0f};
  v5 = {0.0f, 0.0f, 0.0f};
  v6 = {0.0f, 0.0f, 0.0f};

  dir01.y = -diagonal.y;
  dir12.x = -diagonal.x;
  dir23.y =  diagonal.y;
  dir30.x =  diagonal.x;

  dir45.y =  diagonal.y;
  dir56.x = -diagonal.x;
  dir67.y = -diagonal.y;
  dir74.x =  diagonal.x;

  dir36.z = -diagonal.z;
  dir50.z =  diagonal.z;
  dir14.z = -diagonal.z;
  dir72.z =  diagonal.z;

  lm_vec3_cross( &mom01, v0, v1 );
  lm_vec3_cross( &mom12, v1, v2 );
  lm_vec3_cross( &mom23, v2, v3 );
  lm_vec3_cross( &mom30, v3, v0 );

  lm_vec3_cross( &mom45, v4, v5 );
  lm_vec3_cross( &mom56, v5, v6 );
  lm_vec3_cross( &mom67, v6, v7 );
  lm_vec3_cross( &mom74, v7, v4 );

  lm_vec3_cross( &mom36, v3, v6 );
  lm_vec3_cross( &mom50, v5, v0 );
  lm_vec3_cross( &mom14, v1, v4 );
  lm_vec3_cross( &mom72, v7, v2 );

  lm_vec3_dot( &w01, raymom, mom01 );
  lm_vec3_dot( &w12, raymom, mom12 );
  lm_vec3_dot( &w23, raymom, mom23 );
  lm_vec3_dot( &w30, raymom, mom30 );

  lm_vec3_dot( &w45, raymom, mom45 );
  lm_vec3_dot( &w56, raymom, mom56 );
  lm_vec3_dot( &w67, raymom, mom67 );
  lm_vec3_dot( &w74, raymom, mom74 );

  lm_vec3_dot( &w36, raymom, mom36 );
  lm_vec3_dot( &w50, raymom, mom50 );
  lm_vec3_dot( &w14, raymom, mom14 );
  lm_vec3_dot( &w72, raymom, mom72 );

  // sw dot products are sparse: 1 FP mul each
  lm_vec3_dot( &sw01, raydir, dir01 );
  lm_vec3_dot( &sw12, raydir, dir12 );
  lm_vec3_dot( &sw23, raydir, dir23 );
  lm_vec3_dot( &sw30, raydir, dir30 );

  lm_vec3_dot( &sw45, raydir, dir45 );
  lm_vec3_dot( &sw56, raydir, dir56 );
  lm_vec3_dot( &sw67, raydir, dir67 );
  lm_vec3_dot( &sw74, raydir, dir74 );

  lm_vec3_dot( &sw36, raydir, dir36 );
  lm_vec3_dot( &sw50, raydir, dir50 );
  lm_vec3_dot( &sw14, raydir, dir14 );
  lm_vec3_dot( &sw72, raydir, dir72 );

  t01 = (sw01 + w01 > 0) ? 1 : 0;
  t12 = (sw12 + w12 > 0) ? 1 : 0;
  t23 = (sw23 + w23 < 0) ? 1 : 0;
  t30 = (sw30 + w30 < 0) ? 1 : 0;

  t67 = (sw45 + w67 > 0) ? 1 : 0;
  t74 = (sw56 + w74 < 0) ? 1 : 0;
  t45 = (sw67 + w45 < 0) ? 1 : 0;
  t56 = (sw74 + w56 > 0) ? 1 : 0;

  t36 = (sw36 + w36 > 0) ? 1 : 0;
  t50 = (sw50 + w50 < 0) ? 1 : 0;
  t14 = (sw14 + w14 > 0) ? 1 : 0;
  t72 = (sw72 + w72 < 0) ? 1 : 0;

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
