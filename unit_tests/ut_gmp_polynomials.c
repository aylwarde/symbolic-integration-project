#include <stdio.h>
#include "../polynomials.h"

int main()
{
	poly *poly_a, *poly_b;

	poly_a = initialize_p(3);
	assign_coeffs_p(poly_a);
	
	//test display_p	
	print_p(poly_a);

	//test printlatex_p
	latex_p(poly_a);
	
	//test strip_p
	strip_p(poly_a);
	print_p(poly_a);

	//test zero_p
	if(zero_p(poly_a)) {

		printf("zero polynomial\n");
	}

	//test copy_p
	poly_b = copy_p(poly_a);
	print_p(poly_b);

	//test negative_p
	print_p(negative_p(poly_a));

	//test scale_p
	mpz_t five; mpz_init_set_si(five, 5);
	mpz_t six; mpz_init_set_si(six, 6);

	frac *fraction;
	fraction = init_f(five, six);
	print_p(scale_p(fraction, poly_a));

	//test add_p
	print_p(add_p(poly_a, negative_p(poly_a)));

	//test subtract_p
	print_p(subtract_p(poly_a, poly_a));

	//test multiply_p
	print_p(multiply_p(poly_a, poly_a));

	//test pow_p
	print_p(pow_p( poly_a,2));
	print_p(pow_p( poly_a,4));
	print_p(pow_p( poly_a,0));

	//test divide_p
	print_p(divide_p(pow_p( poly_a,2), poly_a)[0]);
	print_p(divide_p(pow_p( poly_a,2), poly_a)[1]);
	printf("-----------------\n");

	//test derivative_p
	print_p(poly_a);
	print_p(derivative_p(poly_a));

	//test gcd_p
	print_p(gcd_p(poly_a, pow_p( poly_a,2)));

	//test content_p
	print_f(content_p(poly_a));

	//test pseudo division
	print_p(pseudodiv_p(pow_p(poly_a,2),poly_a)[0]);
	print_p(pseudodiv_p(pow_p(poly_a,2),poly_a)[1]);

	//test pseudogcd
	print_p(pseudogcd_p(poly_a,pow_p(poly_a,2)));

	//test integrate_p
	printf("this is integral");
	print_p(integrate_p(poly_a));
        
	//test equals_p
	if(equals_p(poly_a, poly_a)) {
		printf("\ntest passed\n");

	}


return 0;
}
