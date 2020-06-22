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
    print_p(polyarray[i]);
  }

  fclose(polyfile);

  poly **subresultantprs = subresultant_p(polyarray[0], polyarray[1], &subprsout);

  for (i=0; i<subprsout; ++i) {
    print_p(subresultantprs[i]);
  }

  subresultantprs = subresultant_p(polyarray[2], polyarray[1], &subprsout);

  for (i=0; i<subprsout; ++i) {
    print_p(subresultantprs[i]);
  }

  subresultantprs = subresultant_p(polyarray[3], polyarray[4], &subprsout);

  for (i=0; i<subprsout; ++i) {
    print_p(subresultantprs[i]);
  }
  
}
