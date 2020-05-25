#include <stdio.h>


#include "../int_utils.h"

int main() {
	
	mpz_t a, b, gcd;
	
	char *a_string = "-3728245", *b_string = "2225", *gcd_string;

	mpz_init_set_str(a, a_string, 10);
       	mpz_init_set_str(b, b_string, 10); 
	
	mpz_init(gcd);	
	gcd_z(gcd, a, b);
	
	mpz_get_str(gcd_string, 10, gcd);
	printf("%s\n", gcd_string);
	
	mpz_clear(a);
	mpz_clear(b);

return 0;
}
