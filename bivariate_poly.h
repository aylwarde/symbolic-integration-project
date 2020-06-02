#ifndef BIVARIATE_POLY_H
#define BIVARIATE_POLY_H

#include "polynomials.h"

// we define a bivariate polynomial as an array of polynomial structures
//eg k(t) a polynomial in t with coefficients q0(x), q1(x), etc 


typedef struct bivariate_poly {
	int deg;
	poly **coefficients;
} bpoly;

bpoly *initialize_bp();
void print_bp();
void strip_bp();
//bpoly *copy_bp();
bpoly *negative_bp();
bpoly *scale_bp();
bpoly *add_bp();
bpoly *subtract_bp();
bpoly *multiply_bp();
//bpoly *pow_bp();
//bpoly divide_pb;


//initialize all coefficients to zero
bpoly *initialize_bp(int degree) {
	
	int i;
	bpoly *bivariate_poly = (bpoly *)calloc(1, sizeof(bpoly));
	
	bivariate_poly->deg = degree;
	bivariate_poly->coefficients = initialize_array_p(degree+1);

	for(i=0; i<=degree; ++i) {
		bivariate_poly->coefficients[i] = initialize_p(0);
	}

	return bivariate_poly;
}

//consider polynomial over the variable the coeffs are defined over (ie. swap dominant variable)
bpoly *perspective_change(bpoly *b_poly) {
	int i=0, j, max=0, k;
	bpoly *perspective;
	poly *placeholder;

	while(i<b_poly->deg+1) {
		max = MAX(max, b_poly->coefficients[i]->deg);
		++i;
	}
	
	perspective = initialize_bp(max);

	for(i=0; i<=b_poly->deg; ++i) { 
		for(j=0; j<=b_poly->coefficients[i]->deg; ++j) {
			placeholder = initialize_p(b_poly->deg-i);
			placeholder->coefficients[0] = b_poly->coefficients[i]->coefficients[j];
			k = b_poly->coefficients[i]->deg-j;
			perspective->coefficients[max-k] = add_p(perspective->coefficients[max-k], placeholder);
		}
	}

	return perspective;
}

//check if poly is zero
bool zero_bp(bpoly *b_poly) {
	int i;
	bool result = true;
	for(i=0; i<=b_poly->deg; ++i) {
		if(!zero_p(b_poly->coefficients[i])) {
				result = false;
		}
	}
	return result;
}

//strip poly of higher order terms with zero coeffs
void strip_bp(bpoly *b_poly) {
	int i, leading_zeroes=0, degree;

	while(zero_p(b_poly->coefficients[leading_zeroes])) {
			++leading_zeroes;
	}

	for(i=0; i<=b_poly->deg-leading_zeroes; ++i) {
		b_poly->coefficients[i] = copy_p(b_poly->coefficients[i+leading_zeroes]);
	}
	degree = b_poly->deg-leading_zeroes;
	b_poly->deg = degree;
}

//add two polys
bpoly *add_bp(bpoly *b_poly1, bpoly *b_poly2) {

	int i, difference;
	bpoly *result;

	if(b_poly1->deg < b_poly2->deg) {
		result = add_bp(b_poly2, b_poly1);
	}

	else {
		difference = b_poly1->deg - b_poly2->deg;
		result = initialize_bp(b_poly1->deg);

		for(i=0; i<difference; ++i) {
			result->coefficients[i] = b_poly1->coefficients[i];
		}

		for(i=difference; i<=result->deg; ++i) {
			result->coefficients[i] = add_p(b_poly1->coefficients[i], b_poly2->coefficients[i-difference]);
		}
	}
		strip_bp(result);
		return result;
}

//negate polynomial
bpoly *negative_bp(bpoly *b_poly1) {
	int i;
	bpoly *result;

	result = initialize_bp(b_poly1->deg);

	for(i=0; i<=b_poly1->deg; ++i) {
		result->coefficients[i] = negative_p(b_poly1->coefficients[i]);
	}
	
	return result;
}

//subtract two polys
bpoly *subtract_bp(bpoly *b_poly1, bpoly *b_poly2) { 
	bpoly *result;
	b_poly2 = negative_bp(b_poly2);
	result = add_bp(b_poly1, b_poly2);
	return result;
}

//polynomial multiplication
bpoly *multiply_bp(bpoly *b_poly1, bpoly *b_poly2) { 

	int i,j;
	bpoly *result;

	result = initialize_bp(b_poly1->deg + b_poly2->deg);

	for(i=0; i<= b_poly1->deg; ++i) {
		for(j=0; j<= b_poly2->deg; ++j) {
			result->coefficients[i+j] = add_p(result->coefficients[i+j], multiply_p(b_poly1->coefficients[i], b_poly2->coefficients[j]));
		}
	}
	strip_bp(result);
	return result;
}

//multiply polynomial in t by polynomial in x (ie a scalar)
bpoly *scale_bp(poly *poly_a, bpoly *b_poly1) {
	
	int i;
	bpoly *result;
	result = initialize_bp(b_poly1->deg);

	for(i=0; i<=b_poly1->deg; ++i) {
		result->coefficients[i] = multiply_p(poly_a, b_poly1->coefficients[i]);
	}

	return result;
}


//pseudo division
bpoly **poly_pseudo_divide(bpoly *b_poly1, bpoly *b_poly2) {
	poly *b;
	int N, d;
	bpoly *q, *r, *T;
	bpoly **result;

	result = (bpoly **)calloc(2, sizeof(bpoly *));

	result[0] = initialize_bp(0);

	result[1] = b_poly1;

	b = b_poly2->coefficients[0];
	N = b_poly1->deg-b_poly2->deg +1;

	while(!zero_bp(result[1]) && result[1]->deg-b_poly2->deg>=0 ) {
		d = result[1]->deg-b_poly2->deg;
		T = initialize_bp(d);
		T->coefficients[0] = result[1]->coefficients[0];
		--N;
		result[0] = add_bp(scale_bp(b, result[0]), T);
	       	result[1] = subtract_bp(scale_bp(b, result[1]), multiply_bp(T, b_poly2));
	}

	result[0] = scale_bp(pow_p(N, b), result[0]);
	result[1] = scale_bp(pow_p(N, b), result[1]);

	return result;
}



	







#endif /* BIVARIATE_POLY_H */
