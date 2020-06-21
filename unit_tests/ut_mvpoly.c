#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../mathsio.h"
#include "../mvpolys.h"

int main() {
  char vars[3] = {'x', 'y', '\0'};
  char vars2[5] = {'w', 'x', 'y', 'z', '\0'};
  unsigned int degs[2] = {3,2};
  unsigned int degs2[4] = {3,2,3,2};
  unsigned int perm[4] = {0,2,1,3};
  unsigned int perm2[2] = {1,0};
  unsigned int i;
  
  printf("%s\n", string_monomial(vars, degs));

  mvpoly *mvp = init_mv(vars, degs);

  printf("%p\n", mvp);
  printf("%s\n", string_mv(mvp));

  char *vars3 = permute_arr_char(4, vars2, perm);

  printf("%s\n", permute_arr_char(2, vars, perm2));

  unsigned int *out = permute_arr_ui(2, degs, perm2);

  for (i=0; i<2; ++i) {
    printf("%d ", out[i]);
  }

  printf("\n");
  
  mvpoly *mvp2 = permute_mv(mvp, perm2);

  printf("%s\n", string_mv(mvp2));

  printf("%s\n", vars3);
}
