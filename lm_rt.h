#include "lm_vec3.h"

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

  float v;         // total volume used in t calc and barycentric denominator
  float va;        // numerator volume for t calc
  float v1;        // numerator volume for beta barycentric
  float v2;        // numerator volume for gamma barycentric

  // OUTPUTS
  // float t;           // distance to intersection point
  // float beta, gamma; // barycentrics

  // ---------------------------------------------------------------------------

  // Get the two triangle edge vectors and the normal at p0. The normal is used
  // in the signed scalar tiple product volume calcs V = (A X B).C = A.(B X C)
  // The volume calculation is covered in K&S [2006] and also here:
  // http://en.wikipedia.org/wiki/Triple_product#Scalar_triple_product

  edge0 = lm_vec3_sub( p1, p0 ); // vector p0->p1
  edge1 = lm_vec3_sub( p0, p2 ); // vector p2->p0 (note the direction)

  // The actual normal is required (not a unit vector)
  normal = lm_vec3_cross( edge1, edge0 );

  // At this point we're finished with p1 and p2. Also note that we could have
  // pre-computed E0, E1 and N and supplied them instead of p1, p2.

  rd = lm_vec3_norm( rd ); // normalise direction in case it hasn't been already

  // Calculate the denominator volume V = (E1 X E0).D = N.D
  v = lm_vec3_dot( normal, rd );

  // Tetrahedron edge from ray origin to first triangle vertex
  edge2 = lm_vec3_sub( p0, ro );

  // Re-using the normal vector at p0, volume Va = (E1 X E0).E2 = N.E2
  va = lm_vec3_dot( normal, edge2 );

  // Distance T = Va/V
  *t = va / v;

  // Calculate the normal at the ray origin* as this cross product is used
  // more than once. I = D X E2
  // * Note that D and E2 both go through the ray origin by definition
  interm = lm_vec3_cross( rd, edge2 );

  // Volume V1 = (D X E2).E1 = I.E1
  v1 = lm_vec3_dot( interm, edge1 );

  // Volume V2 = (D X E2).E0 = I.E0
  v2 = lm_vec3_dot( interm, edge0 );

  *beta  = v1 / v;
  *gamma = v2 / v;

  if ((( v1 < 0.0f && v2 < 0.0f && v < 0.0f) || (v1 > 0.0f && v2 > 0.0f && v > 0.0f) ) && ((v1 + v2) <= v) && (*beta >= 0.0f) && (*gamma >= 0.0f) && ((*beta + *gamma) < 1.0f)) {
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

  vec3 t0, t1;

  rd = lm_vec3_norm( rd );

  // ---------------------------------------------------------------------------
  t0.x.f = ( p0.x.f - ro.x.f ) / rd.x.f;
  t0.y.f = ( p0.y.f - ro.y.f ) / rd.y.f;
  t0.z.f = ( p0.z.f - ro.z.f ) / rd.z.f;

  t1.x.f = ( p1.x.f - ro.x.f ) / rd.x.f;
  t1.y.f = ( p1.y.f - ro.y.f ) / rd.y.f;
  t1.z.f = ( p1.z.f - ro.z.f ) / rd.z.f;

  float tmin = MAX( MIN( t0.x.f, t1.x.f ), MAX( MIN( t0.y.f, t1.y.f ), MIN( t0.z.f, t1.z.f )));
  float tmax = MIN( MAX( t0.x.f, t1.x.f ), MIN( MAX( t0.y.f, t1.y.f ), MAX( t0.z.f, t1.z.f )));

  if( tmin <= tmax ) {
    return 1;
  }

  return 0;
}

int lm_rt_raysphereint( vec3 ro, vec3 rd, vec3 p0, float rad, vec3 *normal ) {
  vec3 oc, p;
  float oc_sq, t, gc_sq, hg_sq;

  // ray origin to sphere centre and its squared length
  oc = lm_vec3_sub( p0, ro );
  oc_sq = lm_vec3_dot( oc, oc );

  // normalise the ray direction
  rd = lm_vec3_norm( rd );

  // distance along the ray, of the sphere centre from ray origin
  t = lm_vec3_dot( oc, rd );

  // pythagoras: oc^2 = gc^2 + t^2
  gc_sq = oc_sq - t*t;

  // r^2 = hg^2 + gc^2
  hg_sq = rad*rad - gc_sq;

  if( hg_sq < 0.0f ) {
    return 0;
  }
  // distance along ray to the intersection point
  t = t - sqrt( hg_sq );
  // p is the intersection point on the sphere
  p = lm_vec3_add( ro, lm_vec3_scale( t, rd ));
  // normal is from the centre to the intersection we could normalise
  // with lm_vec3_norm but we know the radius so just divide by that
  *normal = lm_vec3_scale( 1.0f/rad, lm_vec3_sub( p, p0 ));
  return 1;
}