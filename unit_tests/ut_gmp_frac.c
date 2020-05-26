#include <stdio.h>
#include "../frac.h"

int main() {

	char *a_num = "3456543", *b_denom = "6543567346345";
	mpz_t a, b, c, d, e;
	frac *frac_a, *frac_b;

	//initialize two fractions
	mpz_init_set_str(b, b_denom, 10);
	mpz_init_set_str(a, a_num, 10);
	frac_a = init_f(a,b);
	frac_b = init_f(b,a);
	
	//test print_f
	print_f(frac_a);
	printf("\n");
	print_f(frac_b);
	printf("\n");
	
	//test reduce_f
	frac_a = reduce_f(frac_a);
	print_f(frac_a);
	printf("\n");

	//test add_f
	print_f(add_f(frac_a, frac_b));
	printf("\n");

	//test multiply_f
	print_f(multiply_f(frac_a, frac_b));
	printf("\n");

	//test negative_f
	print_f(negative_f(frac_a));
	printf("\n");
	print_f(add_f(frac_a, negative_f(frac_a)));
	printf("\n");

	//test zero_f
	if(zero_f(add_f(frac_a, negative_f(frac_a)))) {
		printf("oh she zero\n");
	}

	//test reciprocal_f
	print_f(reciprocal_f(frac_a));
	printf("\n");

	//test equals_f
	if(equals_f(reciprocal_f(frac_a), frac_b)) {
		printf("b is the reciprocal of a\n");
	}

	//test pow_f
	print_f(pow_f(frac_a, 0));
	printf("\n");
	print_f(pow_f(frac_a, -2));
	printf("\n");
	print_f(pow_f(frac_a, 2));
	printf("\n");
	
	//test divide_f
	print_f(divide_f(frac_a, frac_b));
	printf("\n");

	//test subtract_f
	print_f(subtract_f(frac_a,frac_a));
	printf("\n");

	//test gcd_f
	print_f(gcd_f(frac_a, frac_b));
	printf("\n");

	//test lcm_f
	print_f(lcm_f(frac_a, frac_b));
	printf("\n");

return 0;
}
