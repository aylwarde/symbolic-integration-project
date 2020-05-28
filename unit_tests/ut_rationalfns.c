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

  display_p(polyarray[0]);
  display_p(polyarray[1]);

  rational *rfa = init_r(polyarray[0], polyarray[1]);
  print_r(rfa);
  print_r(add_r(rfa,rfa));
  
  fclose(testfile);
}
