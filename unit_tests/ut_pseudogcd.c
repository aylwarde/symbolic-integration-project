#include <stdio.h>

#include "psedodivision.h"

int main() {

  poly *gcd ;
  FILE *polyfile;

  polyfile = fopen("polytest.txt","r");
  
  if( polyfile == NULL ) {

    printf("Invalid File Path\n");
    exit(1);
    
  }
  
  int polys, i;

  poly **polyarray = from_file_p(polyfile, &polys);
  
  for ( i=0; i<polys; ++i) {
    display_p(polyarray[i]);
  }
  gcd= pseudogcd_p(polyarray[0],polyarray[1]);
  display_p(gcd);
  fclose(polyfile);
  
}
