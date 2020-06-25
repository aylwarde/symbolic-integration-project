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
//tpoly *variable_change();
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
	
	int  i;
	tpoly *trivariate_poly = (tpoly *)calloc(1, sizeof(tpoly));

	//make 1 bpoly
	bpoly *onebp = one_bp();

	trivariate_poly->deg = degree;
	trivariate_poly->brcoefficients = initialize_array_br(degree+1);

	for(i=0; i<=degree; ++i) {
		trivariate_poly->brcoefficients[i] = init_br(initialize_fe(0), bp_to_fe(onebp));
	}

	return trivariate_poly;
}

//initialize an array of n zero trivariate polynomisls
tpoly **initialize_array_tp(int n) {

	tpoly **array_tp = (tpoly **)calloc(n, sizeof(tpoly *));
	return array_tp;
}

tpoly *copy_tp(tpoly *t_poly1) {

	int i;
	tpoly *duplicate;
	duplicate = initialize_tp(t_poly1->deg);

	for(i=0; i<=t_poly1->deg; ++i) {
		duplicate->brcoefficients[i] = copy_br(t_poly1->brcoefficients[i]);
	}
	return duplicate;
}

//free trivariate poly
void free_tp(tpoly *t_poly) {

	free(t_poly->brcoefficients);
	free(t_poly);
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
		t_poly->deg = 0;
	}

	else {
		while(zero_fe(t_poly->brcoefficients[leading_zeroes]->num)) {
			++leading_zeroes;
		}

		for(i=0; i<=t_poly->deg-leading_zeroes; ++i) {
			t_poly->brcoefficients[i] = copy_br(
					t_poly->brcoefficients[i + leading_zeroes]);
		}
		degree = t_poly->deg-leading_zeroes;
		t_poly->deg = degree;
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

tpoly *multiply_tp(tpoly *t_poly1, tpoly *t_poly2) {

	int i,j;
	tpoly *result;

	result = initialize_tp(t_poly1->deg +t_poly2->deg);

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

tpoly *one_tp() {

	tpoly *onetp;
	bpoly *onebp = one_bp();
	onetp = initialize_tp(0);
	onetp->brcoefficients[0] = init_br(bp_to_fe(onebp), bp_to_fe(onebp));
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

		tpoly *quotient = initialize_tp(MAX(t_poly1->deg - t_poly2->deg, 0));
		tpoly *remainder = copy_tp(t_poly1);

		while(!zero_tp(remainder) && (remainder->deg - t_poly2->deg)>=0) {

			d = remainder->deg - t_poly2->deg;
			t = divide_br(remainder->brcoefficients[0], t_poly2->brcoefficients[0]);
			division = initialize_tp(d);
			division->brcoefficients[0] = t;

			quotient = add_tp(quotient, division);
			remainder = subtract_tp(remainder, multiply_tp(t_poly2, division));
			//free_br(t);
			//free_tp(division);
		}
		result[0] = quotient;
		result[1] = remainder;
	}
	return result;
}

	 

//half extended euclidean algorithm in K(a,b)[x] 
tpoly **half_ext_euclid_tp(tpoly *t_poly1, tpoly *t_poly2) {
	
	tpoly **result;
	bpoly *onebp = one_bp();

	tpoly *q, *r, *a_1, *b_1, *r_1;

	a_1 = initialize_tp(0);
	a_1->brcoefficients[0] = init_br(bp_to_fe(onebp), bp_to_fe(onebp));
	b_1 = initialize_tp(0);

	while(!zero_tp(t_poly2)) {

		q = divide_tp(t_poly1, t_poly2)[0];
		r = divide_tp(t_poly1, t_poly2)[1];

		t_poly1 = copy_tp(t_poly2);
		t_poly2 = copy_tp(r);

		r_1 = subtract_tp(a_1, multiply_tp(q, b_1));

		a_1 = copy_tp(b_1);
		b_1 = copy_tp(r_1);
	}

	result = initialize_array_tp(2);
	result[0] = a_1;
	result[1] = t_poly1;

	return result;
}

//full extended euclidean algorithm in K(a,b)[x] 
//returns S,T,G, st G=gcd(t_poly1, t_poly2) and G = St_poly1 +Tt_poly2
tpoly **ext_euclid_tp(tpoly *t_poly1, tpoly *t_poly2) {

	tpoly *s, *g, *t, *r;
	tpoly **result;

	s = half_ext_euclid_tp(t_poly1, t_poly2)[0];
	g = half_ext_euclid_tp(t_poly1, t_poly2)[1];
	t = divide_tp(subtract_tp(g, multiply_tp(s, t_poly1)), t_poly2)[0];
	r = divide_tp(subtract_tp(g, multiply_tp(s, t_poly1)), t_poly2)[1]; //r must be zero

	result = initialize_array_tp(3);
	result[0] = s;
	result[1] = t;
	result[2] = g;

	return result;
}

bool equals_tp(tpoly *t_poly1, tpoly *t_poly2) {

	return zero_tp(subtract_tp(t_poly1, t_poly2));
}

biv_rational *content_tp(tpoly *t_poly) {

	return gcd_array_br(t_poly->deg, t_poly->brcoefficients);
}

#endif /* TRIVARIATE_POLY_H */
