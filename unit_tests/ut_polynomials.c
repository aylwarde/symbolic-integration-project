#include <stdio.h>

#include "../polynomials.h"
#include "../frac.h"

int main() {
  
  FILE *polyfile;

  polyfile = fopen("polytest.txt","r");
  
  if( polyfile == NULL ) {

    printf("Invalid File Path\n");
    exit(1);
    
  }
  
  int polys, i;

  poly **polyarray = from_file_p(polyfile, &polys);
  
  for ( i=0; i<polys; ++i) {
    print_p(polyarray[i]);
  }

  fclose(polyfile);

  print_f(content_p(polyarray[1]));
  printf("\n");
  
}
