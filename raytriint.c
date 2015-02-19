//
// Code to perform a ray/triangle intersect test as per Kensler & Shirley [2006]
//
// **
// ** COMMENTED CODE IS MAINTAINED IN lm_rt.h WHERE THIS IS A FUNCTION
// **

#include <stdio.h>
#include "lm_vec3.h"

int main() {

  // INPUTS
  vec3 ro, rd;     // ray origin, direction
  vec3 p0, p1, p2; // test triangle vertex positions

  // INTERNALS
  vec3 edge0;      // triangle edge p0 -> p1
  vec3 edge1;      // triangle edge p2 -> p1
  vec3 normal;     // normal at p0 = e0 X e1
  vec3 edge2;      // tetrahedron edge ro -> p0 (not an obvious variable name)
  vec3 interm;     // normal at ro = e2 X rd used and re-used in volume calcs

  float v;         // total volume used in t calc and  barycentric denominator
  float va;
  float v1;
  float v2;

  // OUTPUTS
  flong t;         // distance to intersection point
  flong alpha;     // barycentrics
  flong beta;
  flong gamma;
  vec3 p;          // point of intersection on p0p1p2,  p = ro + t*rd

  // ---------------------------------------------------------------------------
  // Set up ray
  //
  ro.x.f = 0.0f;
  ro.y.f = 0.0f;
  ro.z.f = 0.0f;

  rd.x.f = 1.0f;
  rd.y.f = 1.0f;
  rd.z.f = 1.0f;

  // Set up triangle
  //
  p0.x.f = 3.99f;
  p0.y.f = 3.99f;
  p0.z.f = 4.0f;

  p1.x.f = 32.0f;
  p1.y.f = 0.0f;
  p1.z.f = 4.0f;

  p2.x.f = 0.0f;
  p2.y.f = 16.0f;
  p2.z.f = 4.0f;

/*// ---------------------------------------------------------------------------
  // Set up ray
  //
  ro.x.l = 0x44281ac3;
  ro.y.l = 0xc31248d5;
  ro.z.l = 0x4421ffff;

  rd.x.l = 0x3ecf72f9;
  rd.y.l = 0xbed8c50c;
  rd.z.l = 0x3f4f6fc4;

  // Set up triangle
  //
  p0.x.l = 0xc4430000;
  p0.y.l = 0x437c0000;
  p0.z.l = 0xc4380000;

  p1.x.l = 0xc4430000;
  p1.y.l = 0x437c0000;
  p1.z.l = 0xc4580000;

  p2.x.l = 0xc42c0000;
  p2.y.l = 0x437c0000;
  p2.z.l = 0xc4580000;
*/

  // ---------------------------------------------------------------------------

  edge0 = lm_vec3_sub( p1, p0 );
  edge1 = lm_vec3_sub( p0, p2 );

  normal = lm_vec3_cross( edge1, edge0 );

  rd = lm_vec3_norm( rd );

  v = lm_vec3_dot( normal, rd );

  edge2 = lm_vec3_sub( p0, ro );

  va = lm_vec3_dot( normal, edge2 );

  t.f = va / v;

  interm = lm_vec3_cross( rd, edge2 );

  v1 = lm_vec3_dot( interm, edge1 );

  beta.f = v1 / v;

  v2 = lm_vec3_dot( interm, edge0 );

  gamma.f = v2 / v;

  // Display results
  printf( "T          = %08x %f\n", t.l, t.f );
  printf( "V          = %f\n", v );
  printf( "Va         = %f\n", va );
  printf( "V1         = %f\n", v1 );
  printf( "V2         = %f\n", v2 );
  printf( "V1/V Beta  = %08x %f\n", beta.l, beta.f );
  printf( "V2/V Gamma = %08x %f\n", gamma.l, gamma.f );

  p = lm_vec3_add( ro, lm_vec3_scale( t.f, rd ));

  printf("P = O + t*D\n");
  lm_vec3_print( p );

  p = lm_vec3_add( p0, lm_vec3_add( lm_vec3_scale( beta.f, lm_vec3_sub( p1, p0 ) ), lm_vec3_scale( gamma.f, lm_vec3_sub( p2, p0 ) ) ) );

  printf("P = P0 + beta*(P1 - P0) + gamma*(P2 - P0)\n");
  lm_vec3_print( p );

  p = lm_vec3_add( lm_vec3_scale( 1.0f - beta.f - gamma.f, p0 ), lm_vec3_add( lm_vec3_scale( beta.f, p1 ), lm_vec3_scale( gamma.f, p2 ) ) );

  printf("P = (1 - beta - gamma)*P0 + beta*P1 + gamma*P2\n");
  lm_vec3_print( p );

  if (((v1 + v2) < v) && (beta.f >= 0.0f) && (gamma.f >= 0.0f)) {
    printf( "HIT\n" );
  } else {
    printf( "MISS\n" );
  }

  return 0;
}
