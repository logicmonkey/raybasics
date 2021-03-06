#include <stdio.h>
#include "lm_vec3.h"

char lm_raybox( vec3 ro, vec3 rd, vec3 v0, vec3 v7, int debug ) {
  vec3 v1, v2, v3, v4, v5, v6;
  float r0, r1, r2, r3, r4, r5;
  int t01, t12, t23, t30, t45, t56, t67, t74, t14, t72, t36, t50;
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

  FlOps
    Ray Plücker coords: 6 x mul + 3 x add
    AABB edges 12 x [ 2 x mul + 2 x add ]

    30 mul + 27 add

    The 12 edges can be reduced to 6 using the silhouette of the box, so best
    case that gives:

    6 mul + 3 add + 6 (2 mul + 2 add) = 18 mul + 15 add

    This is very good - even compared to the signed volume calculation method.

    There is no silhouette logic in the following code, so all 12 edges are
    calculated.

  Calculations
    Each box edge is reduced to a unit vector in a single x, y or z directon
    and the relative orientation with the actual ray is calculated.

    plucker.wxm does the heavy lifting to reduce the possibility of a mistake.

------------------------------------------------------------------------------*/

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
    printf( "a%d b%d c%d d%d e%d f%d\n", a, b, c, d, e, f );
  }

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
