#include <stdio.h>
#include "lm_vec3.h"

char lm_raybox( vec3 ro, vec3 rd, vec3 v0, vec3 v7, int debug ) {
  vec3 ad, bo, v1, v2, v3, v4, v5, v6;
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
  bo.x = ro.x - v0.x;
  bo.z = ro.z - v0.z;
  ad.x = rd.x + bo.x;
  ad.z = rd.z + bo.z;

  t01 = ((ad.z*bo.x-ad.x*bo.z) > 0) ? 1 : 0;

  bo.y = ro.y - v1.y;
  bo.z = ro.z - v1.z;
  ad.y = rd.y + bo.y;
  ad.z = rd.z + bo.z;

  t12 = ((ad.y*bo.z-ad.z*bo.y) > 0) ? 1 : 0;

  bo.x = ro.x - v2.x;
  bo.z = ro.z - v2.z;
  ad.x = rd.x + bo.x;
  ad.z = rd.z + bo.z;

  t23 = ((ad.z*bo.x-ad.x*bo.z) < 0) ? 1 : 0;

  bo.y = ro.y - v3.y;
  bo.z = ro.z - v3.z;
  ad.y = rd.y + bo.y;
  ad.z = rd.z + bo.z;

  t30 = ((ad.y*bo.z-ad.z*bo.y) < 0) ? 1 : 0;

  // Face F
  bo.x = ro.x - v6.x;
  bo.z = ro.z - v6.z;
  ad.x = rd.x + bo.x;
  ad.z = rd.z + bo.z;

  t67 = ((ad.z*bo.x-ad.x*bo.z) > 0) ? 1 : 0;

  bo.y = ro.y - v7.y;
  bo.z = ro.z - v7.z;
  ad.y = rd.y + bo.y;
  ad.z = rd.z + bo.z;

  t74 = ((ad.y*bo.z-ad.z*bo.y) < 0) ? 1 : 0;

  bo.x = ro.x - v4.x;
  bo.z = ro.z - v4.z;
  ad.x = rd.x + bo.x;
  ad.z = rd.z + bo.z;

  t45 = ((ad.z*bo.x-ad.x*bo.z) < 0) ? 1 : 0;

  bo.y = ro.y - v5.y;
  bo.z = ro.z - v5.z;
  ad.y = rd.y + bo.y;
  ad.z = rd.z + bo.z;

  t56 = ((ad.y*bo.z-ad.z*bo.y) > 0) ? 1 : 0;

  // Edges for Faces B, C, D, E
  bo.x = ro.x - v3.x;
  bo.y = ro.y - v3.y;
  ad.x = rd.x + bo.x;
  ad.y = rd.y + bo.y;

  t36 = ((ad.y*bo.x-ad.x*bo.y) > 0) ? 1 : 0;

  bo.x = ro.x - v5.x;
  bo.y = ro.y - v5.y;
  ad.x = rd.x + bo.x;
  ad.y = rd.y + bo.y;

  t50 = ((ad.y*bo.x-ad.x*bo.y) < 0) ? 1 : 0;

  bo.x = ro.x - v4.x;
  bo.y = ro.y - v4.y;
  ad.x = rd.x + bo.x;
  ad.y = rd.y + bo.y;

  t14 = ((ad.y*bo.x-ad.x*bo.y) > 0) ? 1 : 0;

  bo.x = ro.x - v7.x;
  bo.y = ro.y - v7.y;
  ad.x = rd.x + bo.x;
  ad.y = rd.y + bo.y;

  t72 = ((ad.y*bo.x-ad.x*bo.y) < 0) ? 1 : 0;

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

  if( a==1 ){ return 'A'; };
  if( b==1 ){ return 'B'; };
  if( c==1 ){ return 'C'; };
  if( d==1 ){ return 'D'; };
  if( e==1 ){ return 'E'; };
  if( f==1 ){ return 'F'; };
  return '0';
}

int main() {
  vec3 v0, v7, ro, rd;

  v7.x = -2.0f; v0.x = 2.0f;
  v7.y = -2.0f; v0.y = 2.0f;
  v7.z = -2.0f; v0.z = 2.0f;

/*

                    y
    *              *|             * (15,15,0)
                    |
                    |
           +------->-------+
           |        |      |
           |        |      |
           |        |      |
    *      ^       *|/     V      * (15,0,0)
           |      --o------|-------x
           |       /|      |
           |      /        |
           +-----/-<-------+
                /
               /
    *         /    *              * (15,-15,0)
             z
            /
           /
          # ro (0,0,10)
     RAY ORIGIN
*/

  printf("A: Ray origin positive Z axis, direction O\n");

  ro.x =  0.0f;
  ro.y =  0.0f;
  ro.z = 10.0f;

// ray 1
  rd.x = -15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("B: Ray origin positive Y axis, direction O\n");

  ro.x =   0.0f;
  ro.y =  10.0f;
  ro.z =   0.0f;

// ray 1
  rd.x = -15.0f;
  rd.y =  -1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =  -1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  15.0f;
  rd.y =  -1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x = -15.0f;
  rd.y =  -1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =  -1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  15.0f;
  rd.y =  -1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x = -15.0f;
  rd.y =  -1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y =  -1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  15.0f;
  rd.y =  -1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("C: Ray origin positive X axis, direction O\n");

  ro.x = 10.0f;
  ro.y =  0.0f;
  ro.z =  0.0f;

// ray 1
  rd.x = -1.0f;
  rd.y =  15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =  -1.0f;
  rd.y =  15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  -1.0f;
  rd.y =  15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x =  -1.0f;
  rd.y =   0.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =  -1.0f;
  rd.y =   0.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  -1.0f;
  rd.y =   0.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x =  -1.0f;
  rd.y = -15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =  -1.0f;
  rd.y = -15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  -1.0f;
  rd.y = -15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("D: Ray origin negative Y axis, direction O\n");

  ro.x =   0.0f;
  ro.y = -10.0f;
  ro.z =   0.0f;

// ray 1
  rd.x = -15.0f;
  rd.y =   1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =   1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  15.0f;
  rd.y =   1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x = -15.0f;
  rd.y =   1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =   1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  15.0f;
  rd.y =   1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x = -15.0f;
  rd.y =   1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y =   1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  15.0f;
  rd.y =   1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("E: Ray origin negative X axis, direction O\n");

  ro.x = -10.0f;
  ro.y =   0.0f;
  ro.z =   0.0f;

// ray 1
  rd.x =   1.0f;
  rd.y =  15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   1.0f;
  rd.y =  15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =   1.0f;
  rd.y =  15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x =   1.0f;
  rd.y =   0.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   1.0f;
  rd.y =   0.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =   1.0f;
  rd.y =   0.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x =   1.0f;
  rd.y = -15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   1.0f;
  rd.y = -15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =   1.0f;
  rd.y = -15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("F: Ray origin negative Z axis, direction O\n");

  ro.x =  0.0f;
  ro.y =  0.0f;
  ro.z =-10.0f;

// ray 1
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x = -15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("Fire 3 rays at faces A,B,C from (20, 20, 20)\n");

  ro.x = 20.0f;
  ro.y = 20.0f;
  ro.z = 20.0f;

// ray A
  rd.x =  1.9f;
  rd.y =  1.9f;
  rd.z =  2.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray B
  rd.x =  1.9f;
  rd.y =  2.0f;
  rd.z =  1.9f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray C
  rd.x =  2.0f;
  rd.y =  1.9f;
  rd.z =  1.9f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("\n");

  printf("Fire 3 rays at faces D,E,F from (-20, -20, -20)\n");

  ro.x = -20.0f;
  ro.y = -20.0f;
  ro.z = -20.0f;

// ray D
  rd.x =  -1.9f;
  rd.y =  -2.0f;
  rd.z =  -1.9f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray E
  rd.x =  -2.0f;
  rd.y =  -1.9f;
  rd.z =  -1.9f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray F
  rd.x =  -1.9f;
  rd.y =  -1.9f;
  rd.z =  -2.0f;

  lm_vec3_sub( &rd, rd, ro );
  lm_vec3_norm( &rd, rd );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("\n");
  return 0;
}
