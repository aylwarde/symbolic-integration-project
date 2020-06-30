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

	poly *a, *c, **d, *e, **div, **div1, **div2, **ext;
	rational *temp;

	//initializing rational array structure for result
	rational **result;
	result = (rational **)calloc(2, sizeof(rational*));
	
	//make one and zero polynomials
	poly *onepoly, *zeropoly;
	zeropoly = initialize_and_zero_p(0);
	onepoly = one_p();
	
	//initialize result[0] as zero rational polynomial
	result[0] = init_r(zeropoly, onepoly);
	free_p(zeropoly);
	free_p(onepoly);

	//copy numerator to not make changes to rational_a
	poly *num;
	num = copy_p(rational_a->num);

	//a = deflation of denominator, b = squarefree part of denominator
	a = gcd_p(rational_a->denom, derivative_p(rational_a->denom));
	div = divide_p(rational_a->denom, a);

	while(a->deg>0) {
		c = gcd_p(a, derivative_p(a)); //deflation of a
		div1 = divide_p(a, c); // div[1] = squarefree part of a
		
		//polynomials defined for tidyness
		d = divide_p(multiply_p(div[0],derivative_p(a)), a);
		e = negative_p(d[0]);
		
		// num = e*ext[0] + d*ext[1]
		ext = extended_euclidean_diophantine(e, div1[0], num);
		free_p(e);
		free_array_p(d, 2);
		
		//new numerator in reduction step	
		free_p(num);
		div2 = divide_p(multiply_p(derivative_p(ext[0]), div[0]), div1[0]);
		num = subtract_p(ext[1], div2[0]);
		free_array_p(div2, 2);

		temp = copy_r(result[0]);
		free_r(result[0]);
		result[0] = add_r(temp, init_r(ext[0], a));
		free_r(temp);
		
		free_array_p(ext, 2);

		//replace a by deflation of a 
		free_p(a);
		a = copy_p(c);
		free_p(c);
		free_array_p(div1, 2);
	}

	result[1] = init_r(num, div[0]);
	
	//freeing up
	free_p(a);
	free_array_p(div, 2);
	
	return result;
}


#endif /* HERMITE_REDUCE_H */



