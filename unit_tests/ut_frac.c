#include <stdio.h>
#include <stdbool.h>

#include "../frac.h"

int main() {

  frac *frac_a = init_f(3,6);
  frac *frac_b = init_f(2,6);
  frac *frac_c = init_f(7,3);
  frac *frac_d = init_f(0,5);

  print_f(reduce_f(frac_a));
  print_f(reduce_f(frac_b));

  print_f(add_f(frac_a, frac_b));
  print_f(multiply_f(frac_a, frac_b));

  print_f(negative_f(frac_a));
  print_f(reciprocal_f(frac_a));

  print_f(pow_f(frac_c, -3));

  print_f(init_f(5,1));
  
  print_f(subtract_f(frac_a, frac_b));
  print_f(divide_f(frac_a, frac_b));

  fputs(zero_f(frac_d) ? "true\n": "false\n", stdout);
  fputs(zero_f(frac_c) ? "true\n": "false\n", stdout);
  fputs(equal_f(frac_a, frac_a) ? "true\n": "false\n", stdout);
  fputs(equal_f(frac_a, reduce_f(frac_a)) ? "true\n": "false\n", stdout);
  fputs(equal_f(frac_a, frac_b) ? "true\n": "false\n", stdout);
}
