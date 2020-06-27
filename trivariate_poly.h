#ifndef TRIVARIATE_POLY_H
#define TRIVARIATE_POLY_H

#include "biv_rationalfns.h"

//AS AN IMPORTANT STRAY FROM THE USUAL:
//trivariate polynomials will be defined with coeffs that are !!rational!! bivariate polys...
//wrt to this arctan stuff we're working in K(a,b)[x] so K(a,b) a field should have quotients...
//when we get to using more general trivariate polys, then i'm not certain about how this will work,
//but that bridge and me are a good distance away yet


typedef struct trivariate_poly {
	int deg;
	biv_rational **brcoefficients;
} tpoly;

//void display_tp();
void free_tp();
void strip_tp();

tpoly *initialize_tp();
tpoly *initialize_and_zero_tp();
tpoly **initialize_array_tp();
tpoly *copy_tp();
tpoly *negative_tp();
tpoly *scale_tp();
tpoly *add_tp();
tpoly *subtract_tp();
tpoly *multiply_tp();
tpoly *one_tp();
tpoly *pow_tp();
//tpoly **pseudo_divide_tp();

bool zero_tp();
bool equals_tp();
biv_rational *content_tp();

//initialize all coefficients to zero
tpoly *initialize_tp(int degree) {
	
	tpoly *trivariate_poly = (tpoly *)calloc(1, sizeof(tpoly));

	trivariate_poly->deg = degree;
	trivariate_poly->brcoefficients = initialize_array_br(degree+1);

	return trivariate_poly;
}

//initialize all coefficients to zero
tpoly *initialize_and_zero_tp(int degree) {
	
	int  i;
	tpoly *trivariate_poly = (tpoly *)calloc(1, sizeof(tpoly));

	//make 1 bpoly
	bpoly *onebp = one_bp();

	trivariate_poly->deg = degree;
	trivariate_poly->brcoefficients = initialize_array_br(degree+1);

	for(i=0; i<=degree; ++i) {
		trivariate_poly->brcoefficients[i] = init_br(initialize_and_zero_fe(0), 
				bp_to_fe(onebp));
	}
	free_bp(onebp);

	return trivariate_poly;
}

//initialize an array of n zero trivariate polynomisls
tpoly **initialize_array_tp(int n) {

	tpoly **array_tp = (tpoly **)calloc(n, sizeof(tpoly *));
	return array_tp;
}

//copy tp
tpoly *copy_tp(tpoly *t_poly1) {

	int i;
	tpoly *duplicate = (tpoly *)calloc(1, sizeof(tpoly));
	duplicate->deg = t_poly1->deg;
	duplicate->brcoefficients = (biv_rational **)
		calloc(duplicate->deg+1, sizeof(biv_rational *));
	for(i=0; i<=t_poly1->deg; ++i) {
		duplicate->brcoefficients[i] = copy_br(t_poly1->brcoefficients[i]);
	}
	return duplicate;
}

//free trivariate poly
void free_tp(tpoly *t_poly) {

	for(int i=0; i<=t_poly->deg; ++i) {
		free_br(t_poly->brcoefficients[i]);
	}
	free(t_poly->brcoefficients);
	free(t_poly);
}

//free array of trivariate polys
void free_array_tp(tpoly **tpolyarray, int len) {
	int i;
	for(i=0; i<len; ++i) {
		free_tp(tpolyarray[i]);
	}
	free(tpolyarray);
}

//check if trivariate poly is zero
bool zero_tp(tpoly *t_poly) {
	int i;
	bool result = true;
	for(i=0; i<=t_poly->deg; ++i) {
		if(!zero_fe(t_poly->brcoefficients[i]->num)) {
			result = false;
		}
	}
	return result;
}

//strip trivariate poly of higher order terms with zero coeffs
void strip_tp(tpoly *t_poly) {
	int i, leading_zeroes=0, degree;

	if(zero_tp(t_poly)) {
		biv_rational **new_coeffs = (biv_rational **)calloc(1, sizeof(biv_rational *));
		new_coeffs[0] = copy_br(t_poly->brcoefficients[0]);

		for(i=0; i<=t_poly->deg; ++i) {
			free_br(t_poly->brcoefficients[i]);
		}
		free(t_poly->brcoefficients);
		t_poly->deg = 0;
		t_poly->brcoefficients = new_coeffs;
	}

	else {

		while(zero_fe(t_poly->brcoefficients[leading_zeroes]->num)) {
			++leading_zeroes;
		}

		degree = t_poly->deg-leading_zeroes;
		biv_rational **new_coeffs = (biv_rational **)
			calloc(degree+1, sizeof(biv_rational *));

		for(i=0; i<=degree; ++i) {
			new_coeffs[i] = copy_br(t_poly->brcoefficients[i + leading_zeroes]);
		}
		for(i=0; i<=t_poly->deg; ++i) {
			free_br(t_poly->brcoefficients[i]);
		}
		free(t_poly->brcoefficients);
		t_poly->deg = degree;
		t_poly->brcoefficients = new_coeffs;
	}
}

//negate a trivariate polynomial 
tpoly *negative_tp(tpoly *t_poly) {
	int i;
	tpoly *result;

	result = initialize_tp(t_poly->deg);

	for(i=0; i<=t_poly->deg; ++i) {
		result -> brcoefficients[i] = negative_br(t_poly->brcoefficients[i]);
	}

	return result;
}

tpoly *scale_tp(biv_rational *scalar, tpoly *poly) {
	
	int i;
	tpoly *result = initialize_tp(poly->deg);

	for(i=0; i<= poly->deg; ++i) {
		result->brcoefficients[i] = multiply_br(scalar, poly->brcoefficients[i]);
	}

	return result;
}

//add two polys
tpoly *add_tp(tpoly *t_poly1, tpoly *t_poly2) {

	int i, difference;
	tpoly *result;

	if(t_poly1->deg < t_poly2->deg) {
		result = add_tp(t_poly2, t_poly1);
	}

	else {
		difference = t_poly1->deg - t_poly2->deg;
		result = initialize_tp(t_poly1->deg);

		for(i=0; i<difference; ++i) {
			result->brcoefficients[i] = copy_br(t_poly1->brcoefficients[i]);
		}

		for(i=difference; i<=result->deg; ++i) {
			result->brcoefficients[i] = 
			add_br(t_poly1->brcoefficients[i], t_poly2->brcoefficients[i-difference]);
		}
	}
	strip_tp(result);
	return result;
}


//subtract two polys
tpoly *subtract_tp(tpoly *t_poly1, tpoly *t_poly2) {
	tpoly *result;
	result = add_tp(t_poly1, negative_tp(t_poly2));
	return result;
}

//multiplication
tpoly *multiply_tp(tpoly *t_poly1, tpoly *t_poly2) {

	int i,j;
	tpoly *result;

	result = initialize_and_zero_tp(t_poly1->deg +t_poly2->deg);

	for(i=0; i<=t_poly1->deg; ++i) {
		for(j=0; j<= t_poly2->deg; ++j) {
			result->brcoefficients[i+j] =
			       add_br(result->brcoefficients[i+j], multiply_br(t_poly1->
					brcoefficients[i], t_poly2->brcoefficients[j]));
		}
	}
	strip_tp(result);
	return result;
}

//make one t poly
tpoly *one_tp() {

	tpoly *onetp;
	bpoly *onebp = one_bp();
	onetp = initialize_tp(0);
	biv_rational *one= init_br(bp_to_fe(onebp), bp_to_fe(onebp));
	onetp->brcoefficients[0] = copy_br(one); 
	free_bp(onebp);
	free_br(one);
	return onetp;
}

//raises a trivariate polynomial to the power of an int
tpoly *pow_tp(tpoly *t_poly1, int exp) {
	if(exp==0) {
		tpoly *onetp = one_tp();

		return onetp;
	}

	else if(exp==1) {
		return t_poly1;
	}
	else {
		return multiply_tp(pow_tp(t_poly1, exp-1), t_poly1);
	}
}

//remember importantly that we are working over something like K(a,b)[x]
tpoly **divide_tp(tpoly *t_poly1, tpoly *t_poly2) {

	tpoly **result = initialize_array_tp(2);
	
	if(zero_tp(t_poly2)) {
		printf("Error: division by zero polynomial\n");
		exit(0);
	}

	else {
		int d=0;
		biv_rational *t;
		tpoly *division;

		tpoly *quotient = initialize_and_zero_tp(0);
		tpoly *remainder = copy_tp(t_poly1);

		while(!zero_tp(remainder) && (remainder->deg - t_poly2->deg)>=0) {

			d = remainder->deg - t_poly2->deg;
			t = divide_br(remainder->brcoefficients[0], t_poly2->brcoefficients[0]);
			division = initialize_and_zero_tp(d);
			division->brcoefficients[0] = copy_br(t);

			quotient = add_tp(quotient, division);
			remainder = subtract_tp(remainder, multiply_tp(t_poly2, division));
			free_br(t);
			free_tp(division);
		}
		result[0] = quotient;
		result[1] = remainder;
	}
	return result;
}

	 

//half extended euclidean algorithm in K(a,b)[x] 
tpoly **half_ext_euclid_tp(tpoly *t_poly1, tpoly *t_poly2) {
	
	tpoly **result;
	
	tpoly *a, *b;
	tpoly *q, *r, *a_1, *b_1, *r_1;

	a_1 = one_tp();
	b_1 = initialize_and_zero_tp(0);

	a = copy_tp(t_poly1);
	b = copy_tp(t_poly2);

	while(!zero_tp(b)) {

		q = divide_tp(a, b)[0];
		r = divide_tp(a, b)[1];

		free_tp(a);
		a = copy_tp(b);

		free_tp(b);
		b = copy_tp(r);

		r_1 = subtract_tp(a_1, multiply_tp(q, b_1));

		free_tp(a_1);
		a_1 = copy_tp(b_1);
		
		free_tp(b_1);
		b_1 = copy_tp(r_1);
		
		free_tp(r_1);
		free_tp(q);
		free_tp(r);
	}

	result = initialize_array_tp(2);
	result[0] = a_1;
	result[1] = a;

	free_tp(b_1);
	free_tp(b);

	return result;
}

//full extended euclidean algorithm in K(a,b)[x] 
//returns S,T,G, st G=gcd(t_poly1, t_poly2) and G = St_poly1 +Tt_poly2
tpoly **ext_euclid_tp(tpoly *t_poly1, tpoly *t_poly2) {

	tpoly **half, **div;
	tpoly **result;

	half = half_ext_euclid_tp(t_poly1, t_poly2);
	div = divide_tp(subtract_tp(half[1], multiply_tp(half[0], t_poly1)), t_poly2);

	result = initialize_array_tp(3);
	result[0] = copy_tp(half[0]);
	result[1] = copy_tp(div[0]);
	result[2] = copy_tp(half[1]);

	free_array_tp(half, 2);
	free_array_tp(div, 2);

	return result;
}

bool equals_tp(tpoly *t_poly1, tpoly *t_poly2) {

	return zero_tp(subtract_tp(t_poly1, t_poly2));
}

biv_rational *content_tp(tpoly *t_poly) {

	return gcd_array_br(t_poly->deg, t_poly->brcoefficients);
}

#endif /* TRIVARIATE_POLY_H */
