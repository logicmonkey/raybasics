#include <stdio.h>
#include "lm_vec3.h"

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

char lm_raybox( vec3 ro, vec3 rd, vec3 v0, vec3 v7, int debug ) {
  vec3 v1, v2, v3, v4, v5, v6;
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

   Full Plücker coordinates calculated

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
