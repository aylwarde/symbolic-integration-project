#include <stdio.h>

#include "../polynomials.h"
#include "../frac.h"
#include "../subresultant.h"

int main() {
  
  FILE *polyfile;

  polyfile = fopen("book_examples/subresultant.txt","r");
  
  if( polyfile == NULL ) {

    printf("Invalid File Path\n");
    exit(1);    
  }
  
  int polys, i, subprsout;

  poly **polyarray = from_file_p(polyfile, &polys);
  
  for ( i=0; i<polys; ++i) {
    display_p(polyarray[i]);
  }

  fclose(polyfile);

  poly **subresultantprs = subresultant(polyarray[0], polyarray[1], &subprsout);
  display_p(subresultantprs[0]);

  for (i=0; i<subprsout; ++i) {
    display_p(subresultantprs[i]);
  }  
}
