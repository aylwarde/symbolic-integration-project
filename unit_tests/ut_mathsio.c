#include <gmp.h>
#include <stdbool.h>

#include "../mathsio.h"
#include "../frac.h"

int main() {

  char num1[] = "1048576";
  char num2[] = "2097152";

  mpz_t mpnum1, mpnum2, mpnum3, mpnum4;

  mpz_init(mpnum1);
  mpz_init(mpnum2);
  mpz_init(mpnum3);
  mpz_init(mpnum4);
  
  mpz_set_str(mpnum1, num1, 10);
  mpz_set_str(mpnum2, num2, 10);
  mpz_set_str(mpnum3, num1, 10);
  mpz_set_str(mpnum4, num2, 10);
  
  mpz_neg(mpnum3, mpnum3);
  mpz_neg(mpnum4, mpnum4);
  
  frac *fraca = init_f(mpnum1, mpnum4);
  frac *fracb = init_f(mpnum2, mpnum3);
  frac *fracc = init_f(mpnum1, mpnum1);

  printf("%s\n", string_f(fraca, false));
  printf("%s\n", string_f(fracb, false));
  printf("%s\n", string_f(fracc, false));
  printf("%s\n", string_f(fracc, true));
      
}
