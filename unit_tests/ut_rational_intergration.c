#include "../Rintergration.h"
#include <stdbool.h>

int main()
{

  FILE *polyfile;

  polyfile = fopen("inttest.txt","r");
  
  if( polyfile == NULL ) {

    printf("Invalid File Path\n");
    exit(1);
    
  }
  
  int polys, i;

  poly **polyarray = from_file_p(polyfile, &polys);
  
  for ( i=0; i<polys; ++i) {
    display_p(polyarray[i]);
  }

  fclose(polyfile);
  
  poly *polya ,*polyb , *Q;
  rational *g,*temp;
  Logs *Rh;

  temp = init_r(polyarray[0],polyarray[1]);

  intergrate_r(temp, &g ,&Q ,&Rh);
  display_p(Q);
  print_r(g);
  fputs(Rh==NULL ? "true\n" : "false\n", stdout);
  return 0;
  
}

  
