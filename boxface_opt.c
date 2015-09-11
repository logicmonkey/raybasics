#include <stdio.h>
#include "lm_vec3.h"

void lm_box_test( vec3 a, vec3 b, vec3 c, vec3 d, vec3 ro, vec3 rd ) {
  vec3 ao, ad;
  int t0, t1, t2, t3;

  t0 = 0; t1 = 0; t2 = 0; t3 = 0;

  lm_vec3_sub( &ao, ro, a );
  lm_vec3_sub( &ad, rd, a );
  t0 = ((ao.y*ad.z-ao.z*ad.y) > 0) ? 1 : 0; // top edge

  lm_vec3_sub( &ao, ro, b );
  lm_vec3_sub( &ad, rd, b );
  t1 = ((ao.z*ad.x-ao.x*ad.z) < 0) ? 1 : 0; // right edge

  lm_vec3_sub( &ao, ro, c );
  lm_vec3_sub( &ad, rd, c );
  t2 = ((ao.y*ad.z-ao.z*ad.y) < 0) ? 1 : 0; // bottom edge

  lm_vec3_sub( &ao, ro, d );
  lm_vec3_sub( &ad, rd, d );
  t3 = ((ao.z*ad.x-ao.x*ad.z) > 0) ? 1 : 0; // left edge
  printf("%d", t0 & t1 & t2 & t3 );
}

int main() {
  vec3 a, b, c, d, ro, rd;

  // 12.0 : only centre ray hits
  // 13.0 : all rays hit
  //
  a.x = -12.0f; b.x = 12.0f; c.x =  12.0f; d.x = -12.0f;
  a.y =  12.0f; b.y = 12.0f; c.y = -12.0f; d.y = -12.0f;
  a.z =   1.0f; b.z =  1.0f; c.z =   1.0f; d.z =   1.0f;

/*

                    y
    *              *|             * (15,15,-1)
                    |
                    |
           a------->-------b (10,10,1)
           |        |      |
           |        |      |
           |        |      |
    *      ^       *|/     V      * (15,0,-1)
           |      --o------|-------x
           |       /|      |
           |      /        |
(-10,-10,1)d-----/-<-------c
                /
               /
    *         /    *              * (15,-15,-1)
             z
            /
           /
          # ro (0,0,10)
     RAY ORIGIN
*/

  ro.x =  0.0f;
  ro.y =  0.0f;
  ro.z = 10.0f;

// ray 1
  rd.x = -15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );

// ray 2
  rd.x =   0.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );

// ray 3
  rd.x =  15.0f;
  rd.y =  15.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );
  printf( "\n" );

// ray 4
  rd.x = -15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );

// ray 5
  rd.x =   0.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );

// ray 6
  rd.x =  15.0f;
  rd.y =   0.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );
  printf( "\n" );

// ray 7
  rd.x = -15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );

// ray 8
  rd.x =   0.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );

// ray 9
  rd.x =  15.0f;
  rd.y = -15.0f;
  rd.z =  -1.0f;

  lm_box_test( a, b, c, d, ro, rd );
  printf( "\n" );

  return 0;
}
