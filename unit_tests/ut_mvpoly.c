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
  
  printf("%s\n", string_monomial(vars, degs));

  mvpoly *mvp = init_mv(vars, degs);

  printf("%s\n", string_mv(mvp));

  printf("%d || %d\n", 29, degs_to_addr_offset(4, addr_offset_to_degs(4, 29, degs2), degs2));
}
