#include <stdio.h>
#include <stdlib.h>

#include "../polynomials.h"
#include "../rationalfns.h"

int main () {

  FILE * testfile;
  poly ** polyarray;
  int polys;

  testfile = fopen("rftest.txt", "r");

  if ( testfile  == NULL ) {

    printf("Invalid File Path\n");
    exit(1);
    
  }
  
  polyarray = from_file_p(testfile, &polys);

  int i;
  
  for ( i=0; i<polys; ++i) {
    display_p(polyarray[i]);
  }

  rational *rfa = init_r(polyarray[0], polyarray[1]);
  rational *rfb = init_r(polyarray[2], polyarray[1]);
  print_r(rfa);
  print_r(rfb);
  print_r(add_r(rfa,rfb));
  print_r(multiply_r(rfa, rfb));
  print_r(negative_r(rfb));
  print_r(reciprocal_r(rfb));

  print_r(pow_r(2,rfb));
  print_r(pow_r(0,rfb));
  print_r(pow_r(-2,rfb));
  print_r(subtract_r(rfa, rfb));
  
  fclose(testfile);
}
