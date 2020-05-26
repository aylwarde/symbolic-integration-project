#include <stdio.h>
#include <gmp.h>

#include "../int_utils.h"

int main() {
	
	mpz_t a; mpz_init(a);
	mpz_t b; mpz_init(b);
	mpz_t gcd; mpz_init(gcd);

	unsigned long int a1= 135;
	unsigned long int b1= 45;
	char *str, *str2;

	mpz_set_ui(a, a1);
	mpz_set_ui(b, b1);

	gcd_z(gcd, a, b);
	

	mpz_get_str(str, 10, gcd);
	printf("%s\n", str);
	
		
	mpz_clear(a);
	mpz_clear(b);
	
	
	return 0;

}
