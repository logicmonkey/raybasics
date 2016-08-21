#include <stdio.h>
#include <stdlib.h>

#define PREC 24

typedef union
{
  long i;
  float f;
} flong;

int main( int argc, char *argv[] )
{
  flong x, xh, xl, gamma, delta, check;

  unsigned char str[33], xh_shift, xl_shift;
  signed char bit;

  unsigned int s;
  unsigned long C;

  x.i = strtol( argv[1], NULL, 16 ); // x as a hex sp float
  s   = strtod( argv[2], NULL );     // s-1 bits in x

  // Veltkamp split start
  //
  C       = (1 << s) + 1;   // 2^s + 1
  gamma.f = (float)C * x.f;
  delta.f = x.f - gamma.f;

  xh.f = gamma.f + delta.f;
  xl.f = x.f - xh.f;
  //
  // Veltkamp split end

  printf( "xh = %08x\nxl = %08x\n", xh.i, xl.i );

  check.f = xh.f + xl.f;
  printf( "x  = %08x (check)\n", check.i );

  // show xh and xl in binary

  for( bit = 31; bit >= 0; bit-- ){
    str[31-bit] = (xh.i >> bit)&1 ? '1' : '0';
  }
  str[32]='\0';
  printf( "xh = %s\n", str );

  for( bit = 31; bit >= 0; bit-- ){
    str[31-bit] = (xl.i >> bit)&1 ? '1' : '0';
  }
  str[32]='\0';
  printf( "xl = %s\n", str );

  // shift off the LSBs to prove they aren't required

  // xh needs PREC-s bits (+ 8 exponent + 1 sign)
  // xl needs s-1 bits (+8 exponent + 1 sign)
  xh_shift = 32 - (PREC - s + 9);
  xl_shift = 32 - (s - 1 + 9);

  printf( "xh 1 + 8 + %d bits\n", PREC-s );
  printf( "xl 1 + 8 + %d bits\n", s-1 );

  xh.i = xh.i >> xh_shift;
  xl.i = xl.i >> xl_shift;

  for( bit = 31; bit >= 0; bit-- ){
    if( bit > 31-xh_shift ) {
      str[31-bit] = '.';
    } else {
      str[31-bit] = (xh.i >> bit)&1 ? '1' : '0';
    }
  }
  str[32]='\0';
  printf( "xh = %s\n", str );

  for( bit = 31; bit >= 0; bit-- ){
    if( bit > 31-xl_shift ) {
      str[31-bit] = '.';
    } else {
      str[31-bit] = (xl.i >> bit)&1 ? '1' : '0';
    }
  }
  str[32]='\0';
  printf( "xl = %s\n", str );

  // restore bits to correct positions in single precision floats ...
  xh.i = xh.i << xh_shift;
  xl.i = xl.i << xl_shift;

  // ... and check again
  check.f = xh.f + xl.f;
  printf( "x  = %08x (re-check)\n", check.i );
}
