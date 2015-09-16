#include <stdio.h>
#include "lm_vec3.h"

char lm_raybox( vec3 ro, vec3 rd, vec3 v0, vec3 v7, int debug ) {
  vec3 ad, bo, v1, v2, v3, v4, v5, v6;
  int t01, t12, t23, t30, t45, t56, t67, t74, t14, t72, t36, t50;
  int a, b, c, d, e, f;

/*
  Vertex numbering - 0 and 7 diagonally opposite

       6-----------5
       |\          .\
       | 3-----------0
       | |         . |       ^
       | |         . |      y|             4-----------5
       | |         . |       |             |     45    |
       7.|.........4 |       |             |           |
        \|          `|     --+------->     |74   F   56|
         2-----------1       |      x      |           |
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
*/

  // steal the appropriate x,y,z positions for V1...V6 from V0 and V7
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
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t01 = ((ad.z*bo.x-ad.x*bo.z) > 0) ? 1 : 0; // edge vector y component -ve

  lm_vec3_sub( &bo, ro, v1 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t12 = ((ad.y*bo.z-ad.z*bo.y) > 0) ? 1 : 0; // edge vector x component -ve

  lm_vec3_sub( &bo, ro, v2 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t23 = ((ad.z*bo.x-ad.x*bo.z) < 0) ? 1 : 0; // edge vector y component +ve

  lm_vec3_sub( &bo, ro, v3 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t30 = ((ad.y*bo.z-ad.z*bo.y) < 0) ? 1 : 0; // edge vector x component +ve

  // Face F

  lm_vec3_sub( &bo, ro, v6 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t67 = ((ad.z*bo.x-ad.x*bo.z) > 0) ? 1 : 0; // edge vector y component -ve

  lm_vec3_sub( &bo, ro, v7 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t74 = ((ad.y*bo.z-ad.z*bo.y) < 0) ? 1 : 0; // edge vector x component +ve

  lm_vec3_sub( &bo, ro, v4 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t45 = ((ad.z*bo.x-ad.x*bo.z) < 0) ? 1 : 0; // edge vector y component +ve

  lm_vec3_sub( &bo, ro, v5 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t56 = ((ad.y*bo.z-ad.z*bo.y) > 0) ? 1 : 0; // edge vector x component -ve

  // Face B
  lm_vec3_sub( &bo, ro, v3 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t36 = ((ad.y*bo.x-ad.x*bo.y) > 0) ? 1 : 0; // edge vector z component -ve

  lm_vec3_sub( &bo, ro, v5 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t50 = ((ad.y*bo.x-ad.x*bo.y) < 0) ? 1 : 0; // edge vector z component +ve

  // Face D
  lm_vec3_sub( &bo, ro, v4 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t14 = ((ad.y*bo.x-ad.x*bo.y) > 0) ? 1 : 0; // edge vector z component -ve

  lm_vec3_sub( &bo, ro, v7 ); // vertex to ray origin
  lm_vec3_add( &ad, rd, bo ); // vertex to ray via ray origin

  t72 = ((ad.y*bo.x-ad.x*bo.y) < 0) ? 1 : 0; // edge vector z component +ve

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

  if( a==0 ){ return 'A'; };
  if( b==0 ){ return 'B'; };
  if( c==0 ){ return 'C'; };
  if( d==0 ){ return 'D'; };
  if( e==0 ){ return 'E'; };
  if( f==0 ){ return 'F'; };
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =  -1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  15.0f;
  rd.y =  -1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x = -15.0f;
  rd.y =  -1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =  -1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  15.0f;
  rd.y =  -1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x = -15.0f;
  rd.y =  -1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y =  -1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  15.0f;
  rd.y =  -1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =  -1.0f;
  rd.y =  15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  -1.0f;
  rd.y =  15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x =  -1.0f;
  rd.y =   0.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =  -1.0f;
  rd.y =   0.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  -1.0f;
  rd.y =   0.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x =  -1.0f;
  rd.y = -15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =  -1.0f;
  rd.y = -15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  -1.0f;
  rd.y = -15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =   1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =  15.0f;
  rd.y =   1.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x = -15.0f;
  rd.y =   1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =   1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =  15.0f;
  rd.y =   1.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x = -15.0f;
  rd.y =   1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y =   1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =  15.0f;
  rd.y =   1.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   1.0f;
  rd.y =  15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x =   1.0f;
  rd.y =  15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x =   1.0f;
  rd.y =   0.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   1.0f;
  rd.y =   0.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x =   1.0f;
  rd.y =   0.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x =   1.0f;
  rd.y = -15.0f;
  rd.z = -15.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   1.0f;
  rd.y = -15.0f;
  rd.z =   0.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x =   1.0f;
  rd.y = -15.0f;
  rd.z =  15.0f;

  lm_vec3_sub( &rd, rd, ro );
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 3
  rd.x = -15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 4
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 6
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c\n", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 7
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray 9
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_vec3_sub( &rd, rd, ro );
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
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray B
  rd.x =  1.9f;
  rd.y =  2.0f;
  rd.z =  1.9f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray C
  rd.x =  2.0f;
  rd.y =  1.9f;
  rd.z =  1.9f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("\n");

  printf("Fire 3 rays at faces D,E,F from (-20, -20, -20)\n");

  ro.x = -20.0f;
  ro.y = -20.0f;
  ro.z = -20.0f;

// ray F
  rd.x =  -1.9f;
  rd.y =  -1.9f;
  rd.z =  -2.0f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray D
  rd.x =  -1.9f;
  rd.y =  -2.0f;
  rd.z =  -1.9f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

// ray E
  rd.x =  -2.0f;
  rd.y =  -1.9f;
  rd.z =  -1.9f;

  lm_vec3_sub( &rd, rd, ro );
  printf("%c", lm_raybox( ro, rd, v0, v7, 0 ));

  printf("\n");
  return 0;
}
