#ifndef BIVARIATE_POLY_H
#define BIVARIATE_POLY_H

#include "polynomials.h"

// we define a bivariate polynomial as an array of polynomial structures
//eg k(t) a polynomial in t with coefficients q0(x), q1(x), etc 
//with order of coeffs from highest order term to lowest

typedef struct bivariate_poly {
	int deg;
	poly **pcoefficients;
} bpoly;

void display_bp();
void free_bp();
void strip_bp();

bpoly *initialize_bp();
bpoly *variable_change();
bpoly **initialize_array_bp();
bpoly *copy_bp();
bpoly *negative_bp();
bpoly *scale_bp();
bpoly *add_bp();
bpoly *subtract_bp();
bpoly *multiply_bp();
bpoly *pow_bp();
bpoly **pseudo_divide_bp();

poly *content_bp();

bool zero_bp();


//initialize all coefficients to zero
bpoly *initialize_bp(int degree) {
	
	int i;
	bpoly *bivariate_poly = (bpoly *)calloc(1, sizeof(bpoly));
	
	bivariate_poly->deg = degree;
	bivariate_poly->pcoefficients = initialize_array_p(degree+1);

	for(i=0; i<=degree; ++i) {
		bivariate_poly->pcoefficients[i] = initialize_p(0);
	}

	return bivariate_poly;
}

//polynomial over t with coefficients in Q(x) goes to polynomial over x with coeffs in Q(t) etc
bpoly *variable_change(bpoly *b_poly) {
	
	int i=0, j, k;
	int max = 0;
	bpoly *result;
	poly *placeholder;

	//find the maximum degree of the polynomial coefficients
	while(i<b_poly->deg+1) {
		max = MAX(max, b_poly->pcoefficients[i]->deg);
		++i;
	}
	
	//initialize bivariate poly of degree max
	result = initialize_bp(max);

	for(i=0; i<=b_poly->deg; ++i) { 
		for(j=0; j<=b_poly->pcoefficients[i]->deg; ++j) {
			placeholder = initialize_p(b_poly->deg-i);
			placeholder->coefficients[0] = b_poly->pcoefficients[i]->coefficients[j];
			k = b_poly->pcoefficients[i]->deg-j;
			
			result->pcoefficients[max-k] = 
				add_p(result->pcoefficients[max-k], placeholder);
		}
	}

	return result;
}

//initialize an array of n zero bivariate polynomials
bpoly **initialize_array_bp(int n) {

	bpoly **array_bp = (bpoly **)calloc(n, sizeof(bpoly *));
	return array_bp;
}

//displays poly(this isn't very pretty... at all)
void display_bp(bpoly *b_poly) {
	int i;

	for(i=0; i<b_poly->deg; ++i) {
	       	if(!zero_p(b_poly->pcoefficients[i])) {
			display_p(b_poly->pcoefficients[i]);
			printf("*t^%d+\n", b_poly->deg-i);
		}
	}
	display_p(b_poly->pcoefficients[b_poly->deg]);
	printf("\n");
}

//free bivariate poly
void free_bp(bpoly *b_poly) {

	free(b_poly->pcoefficients);
	free(b_poly);
}

//check if bivariate poly is zero
bool zero_bp(bpoly *b_poly) {
	int i;
	bool result = true;
	for(i=0; i<=b_poly->deg; ++i) {
		if(!zero_p(b_poly->pcoefficients[i])) {
				result = false;
		}
	}
	return result;
}

//strip bivariate poly of higher order terms with zero coeffs
void strip_bp(bpoly *b_poly) {
	int i, leading_zeroes=0, degree;

	if(zero_bp(b_poly)) {
		b_poly = initialize_bp(0);
	}

	else {

		while(zero_p(b_poly->pcoefficients[leading_zeroes])) {
			++leading_zeroes;
		}

		for(i=0; i<=b_poly->deg-leading_zeroes; ++i) {
			b_poly->pcoefficients[i] = copy_p(b_poly->pcoefficients[i+leading_zeroes]);
		}
		degree = b_poly->deg-leading_zeroes;
		b_poly->deg = degree;
	}
}

//copy a bivariate polynomial strucute
bpoly *copy_bp(bpoly *b_poly) {

	int i;
	bpoly *duplicate;
	duplicate = initialize_bp(b_poly->deg);
	
	for(i=0; i<=b_poly->deg; ++i) {
		duplicate->pcoefficients[i] = b_poly->pcoefficients[i];
	}

	return duplicate;
}

//negate bivariate polynomial
bpoly *negative_bp(bpoly *b_poly1) {
	int i;
	bpoly *result;

	result = initialize_bp(b_poly1->deg);

	for(i=0; i<=b_poly1->deg; ++i) {
		result->pcoefficients[i] = negative_p(b_poly1->pcoefficients[i]);
	}
	
	return result;
}

//multiply polynomial in t by polynomial in x (ie a scalar)
bpoly *scale_bp(poly *poly_a, bpoly *b_poly1) {
	
	int i;
	bpoly *result;
	result = initialize_bp(b_poly1->deg);

	for(i=0; i<=b_poly1->deg; ++i) {
		result->pcoefficients[i] = multiply_p(poly_a, b_poly1->pcoefficients[i]);
	}

	return result;
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
			result->pcoefficients[i] = b_poly1->pcoefficients[i];
		}

		for(i=difference; i<=result->deg; ++i) {
			result->pcoefficients[i] 
			= add_p(b_poly1->pcoefficients[i], b_poly2->pcoefficients[i-difference]);
		}
	}
		strip_bp(result);
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
			result->pcoefficients[i+j] 
				= add_p(result->pcoefficients[i+j], 
						multiply_p(b_poly1->pcoefficients[i], 
							b_poly2->pcoefficients[j]));
		}
	}
	strip_bp(result);
	return result;
}

//pseudo division, result[0]=pquo(bpoly1, bpoly2), result[1]=prem(bpoly1, bpoly2)
bpoly **pseudo_divide_bp(bpoly *b_poly1, bpoly *b_poly2) {
	poly *b;
	int N, d;
	bpoly *q, *r, *T;
	bpoly **result;

	result = initialize_array_bp(2);

	result[0] = initialize_bp(0);
	result[1] = b_poly1;

	b = b_poly2->pcoefficients[0];
	N = b_poly1->deg-b_poly2->deg +1;

	while(!zero_bp(result[1]) && result[1]->deg-b_poly2->deg>=0 ) {
		d = result[1]->deg-b_poly2->deg;
		T = initialize_bp(d);
		T->pcoefficients[0] = result[1]->pcoefficients[0];
		--N;
		result[0] = add_bp(scale_bp(b, result[0]), T);
	       	result[1] = subtract_bp(scale_bp(b, result[1]), multiply_bp(T, b_poly2));
	}

	result[0] = scale_bp(pow_p(N, b), result[0]);
	result[1] = scale_bp(pow_p(N, b), result[1]);

	return result;
}
//raises a bivariate polynomial to the power of an int
bpoly *pow_bp(bpoly *poly1,int exp)
{
  if(exp==0)
    {
      bpoly *onebp;
      poly *onep;
      mpz_t one;


      mpz_init_set_si(one,1);
      onep = initialize_p(0);
      onebp = initialize_bp(0);
      onep->coefficients[0]= init_f(one,one);
      onebp->pcoefficients[0] = onep;

      return onebp ;
    }
  else if(exp==1)
    {
      return poly1;
    }
  else
    {
      return multiply_bp(pow_bp(poly1,exp-1),poly1);
    }
}
/*
poly *content_bp(bpoly *poly)
{
  return gcd_array_p(poly->deg,poly->pcoefficients);
}
*/



#endif /* BIVARIATE_POLY_H */
