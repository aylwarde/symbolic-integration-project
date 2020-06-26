#ifndef HERMITE_REDUCE_H
#define HERMITE_REDUCE_H

#include "rationalfns.h"
#include "euclidean_algorithms.h"

rational **hermite_reduce();

// hermite reduction, mack's linear version

// given a rational poly=num/denom w/ num,denom coprime, return result st
// num/denom = derivative_p(result[0]) + result[1], 
// with result[0], result[1] rational polynomials and result[1] has squarefree denominator

rational **hermite_reduce(rational *rational_a) {

	poly *a, *b, *c, *d, *e, *f, *g;

	//initializing rational array structure for result
	rational **result;
	result = (rational **)calloc(2, sizeof(rational*));
	
	//make one and zero polynomials
	poly *onepoly, *zeropoly;
	zeropoly = initialize_and_zero_p(0);
	
	mpz_t one; mpz_init_set_ui(one, 1);
	onepoly = initialize_p(0);
	onepoly->coefficients[0] = init_f(one, one);
	mpz_clear(one);
	
	//initialize result[0] as zero rational polynomial
	result[0] = init_r(zeropoly, onepoly);
	free_p(zeropoly);
	free_p(onepoly);

	//copy numerator to not make changes to rational_a
	poly *num;
	num = copy_p(rational_a->num);

	//a = deflation of denominator, b = squarefree part of denominator
	a = gcd_p(rational_a->denom, derivative_p(rational_a->denom));
	b = divide_p(rational_a->denom, a)[0];

	while(a->deg>0) {
		c = gcd_p(a, derivative_p(a)); //deflation of a
		d = divide_p(a, c)[0]; //squarefree part of a
		
		//polynomial defined for tidyness
		e = negative_p(divide_p(multiply_p(b,derivative_p(a)), a)[0]);
		
		// num = e*f + d*g
		f = extended_euclidean_diophantine(e, d, num)[0];
		g = extended_euclidean_diophantine(e, d, num)[1];
		free_p(e);
		
		//new numerator in reduction step	
		num = subtract_p(g, divide_p(multiply_p(derivative_p(f), b), d)[0]);
		free_p(d);

		result[0] = add_r(result[0], init_r(f, a));
		
		free_p(f);
		free_p(g);

		//replace a by deflation of a 
		a = copy_p(c);
		free_p(c);
	}

	result[1] = init_r(num, b);
	
	//freeing up
	free_p(a);
	free_p(b);
	
	return result;
}


#endif /* HERMITE_REDUCE_H */



