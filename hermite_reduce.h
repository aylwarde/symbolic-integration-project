#ifndef HERMITE_REDUCE_H
#define HERMITE_REDUCE_H

#include "rationalfns.h"
#include "euclidean_algorithms.h"

rational **hermite_reduce();

//hermite reduction, linear version
rational **hermite_reduce(rational *rational_a) {

	poly *onepoly, *zero, *num;
	poly *a, *b, *c, *d, *e, *f, *g;
	rational **result;
	mpz_t one; mpz_init_set_ui(one, 1);

	result = (rational **)calloc(2, sizeof(rational*));
	
	//make one and zero polynomials
	zero = initialize_p(0);
	onepoly = initialize_p(0);
	onepoly->coefficients[0] = init_f(one, one);
	result[0] = init_r(zero, onepoly);

	num = copy_p(rational_a->num);
	a = gcd_p(rational_a->denom, derivative_p(rational_a->denom));
	b = divide_p(rational_a->denom, a)[0];

	while(a->deg>0) {
		c = gcd_p(a, derivative_p(a));
		d = divide_p(a, c)[0];
		e = negative_p(divide_p(multiply_p(b,derivative_p(a)), a)[0]);
		f = extended_euclidean_diophantine(e, d, num)[0];
		g = extended_euclidean_diophantine(e, d, num)[1];
		num = subtract_p(g, divide_p(multiply_p(derivative_p(f), b), d)[0]);
		result[0] = add_r(result[0], init_r(f, a));
		a = copy_p(c);
	}

	result[1] = init_r(num, b);
	reduce_r(result[1]);
	return result;
}


#endif /* HERMITE_REDUCE_H */



