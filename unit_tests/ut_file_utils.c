#include <stdlib.h>

#include "../file_utils.h"

int main() {

  FILE * testfile;
  
  testfile = fopen("polytest.txt", "r");
  
  if ( testfile == NULL ) {
    
    printf("Could not open file\n");
    exit(1);
    
  }
  
  printf("%s\n",file_to_str(testfile));

  fclose(testfile);
  
}
