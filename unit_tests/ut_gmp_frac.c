#include <stdio.h>
#include "../frac.h"

int main() {

	char *a_num = "3456543", *b_denom = "6543567346345";
	mpz_t a, b;
	frac *frac_a, *frac_b, *frac_result;

	//initialize two fractions
	mpz_init_set_str(b, b_denom, 10);
	mpz_init_set_str(a, a_num, 10);
	frac_a = init_f(a,b);
	frac_b = init_f(b,a);

	//clean up
	mpz_clears(a, b, NULL);
	a_num = NULL;
	b_denom = NULL;
	
	//test print_f
	print_f(frac_a);
	printf("\n");
	print_f(frac_b);
	printf("\n");
	
	//test reduce_f
	reduce_f(frac_a);
	print_f(frac_a);
	printf("\n");

	//test add_f
	frac_result = add_f(frac_a, frac_b);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	//test multiply_f
	frac_result = multiply_f(frac_a, frac_b);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	//test negative_f
	frac_result = negative_f(frac_a);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	//test reciprocal_f
	frac_result = reciprocal_f(frac_a);

	//test equals_f
	if(equals_f(frac_result, frac_b)) {
		printf("b is the reciprocal of a\n");
	}
	free_f(frac_result);

	
	//test pow_f
	frac_result = pow_f(frac_a, 0);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);
	
	frac_result = pow_f(frac_a, 2);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);
	
	frac_result = pow_f(frac_a, -2);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);
	
	//test divide_f
	frac_result = divide_f(frac_a, frac_b);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	//test subtract_f
	frac_result = subtract_f(frac_a, frac_b);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	//test gcd_f
	frac_result = gcd_f(frac_a, frac_b);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	//test lcm_f
	frac_result = lcm_f(frac_a, frac_b);
	print_f(frac_result);
	printf("\n");
	free_f(frac_result);

	free_f(frac_a);
	free_f(frac_b);

return 0;
}
