#include <stdio.h>
#include "../polynomials.h"

int main()
{
	poly *poly_a, *poly_b;

	poly_a = initialize_p(3);
	assign_coeffs_p(poly_a);
	
	//test display_p	
	display_p(poly_a);

	//test printlatex_p
	latex_p(poly_a);
	
	//test strip_p
	strip_p(poly_a);
	display_p(poly_a);

	//test zero_p
	if(zero_p(poly_a)) {

		printf("zero polynomial\n");
	}

	//test copy_p
	poly_b = copy_p(poly_a);
	display_p(poly_b);

	//test negative_p
	display_p(negative_p(poly_a));

	//test scale_p
	mpz_t five; mpz_init_set_si(five, 5);
	mpz_t six; mpz_init_set_si(six, 6);

	frac *fraction;
	fraction = init_f(five, six);
	display_p(scale_p(fraction, poly_a));

	//test add_p
	display_p(add_p(poly_a, negative_p(poly_a)));

	//test subtract_p
	display_p(subtract_p(poly_a, poly_a));

	//test multiply_p
	display_p(multiply_p(poly_a, poly_a));

	//test pow_p
	display_p(pow_p(2, poly_a));
	display_p(pow_p(4, poly_a));

	//test divide_p
	display_p(divide_p(pow_p(2, poly_a), poly_a)[0]);
	display_p(divide_p(pow_p(2, poly_a), poly_a)[1]);
	printf("-----------------\n");

	//test derivative_p
	display_p(poly_a);
	display_p(derivative_p(poly_a));

	//test gcd_p
	display_p(gcd_p(poly_a, pow_p(2, poly_a)));

	//test content_p
	print_f(content_p(poly_a));

	//test pseudo division
	display_p(pseudodiv_p(pow_p(2,poly_a),poly_a)[0]);
	display_p(pseudodiv_p(pow_p(2,poly_a),poly_a)[1]);

	//test pseudogcd
	display_p(pseudogcd_p(poly_a,pow_p(2,poly_a)));

	//test equals_p
	if(equals_p(poly_a, poly_a)) {
		printf("\ntest passed\n");

	}


return 0;
}
