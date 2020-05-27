#include "../frac.h"
#include <stdbool.h>



int main() {

  mpz_t ten, eleven, two;

  mpz_init_set_ui(ten, 10);
  mpz_init_set_ui(eleven, 11);
  mpz_init_set_ui(two, 2);

  frac *frac_a = init_f(ten, two);
  frac *frac_b = init_f(eleven, two);

  puts(latex_f(frac_a, "$$", "$$"));
  puts(latex_f(frac_b, "{", "}"));
  
}
