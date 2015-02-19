#include <stdio.h>
#include <stdlib.h>

int main() {
  int i[2];

  union flong {
    float f;
    long l;
  } f[2];

  float g[1];
  float * h;

  i[0] = 123;
  i[1] = 456;

  printf( "%d\n", i[0] );
  printf( "%d\n", i );     // pointer
  printf( "%d\n", *i );
  printf( "%d\n", &i[0] ); // pointer

  f[0].l = 0x40490fdb;
  f[1].l = 0x40c90fdb;
  printf( "%f\n", f[0].f );
  printf( "%d\n", f );     // pointer
  printf( "%f\n", (*f).f );
  printf( "%d\n", &f[0] ); // pointer

  *g = (*f).f;
  printf( "%f\n", *g );

  h = (float *) malloc ( sizeof( float ) );
  *h = (*f).f;
  printf( "%f\n", *h );

  (*f).f = *h * 2;
  printf( "%f\n", (*f).f );

  return 0;
}
