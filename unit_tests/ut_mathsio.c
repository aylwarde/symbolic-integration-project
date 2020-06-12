#include <gmp.h>
#include <stdbool.h>

#include "../mathsio.h"
#include "../frac.h"
#include "../polynomials.h"

int main() {

  FILE *polyfile;

  polyfile = fopen("mathsio.txt","r");
  
  if( polyfile == NULL ) {

    printf("Invalid File Path\n");
    exit(1);
    
  }
  
  int polys, i;

  poly **polyarray = from_file_p(polyfile, &polys);

  fclose(polyfile);

  char num1[] = "123456789";
  char num2[] = "987654321";

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

  printf("%s\n", string_p(polyarray[0]));
  printf("%s\n", string_p(polyarray[1]));

  rational *rfa = init_r(polyarray[0], polyarray[1]);

  printf("\n%s\n", string_r(rfa));
}
