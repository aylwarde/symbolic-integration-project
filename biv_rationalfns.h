#ifndef BIV_RATIONALFNS_H
#define BIV_RATIONALFNS_H

#include "polynomials.h"
#include "bivariate_poly.h"

typedef struct biv_rational {

	bpoly *num, *denom;
} biv_rational;

/*Functions defined in this header */

biv_rational *init_br();
biv_rational *bpoly_to_br();
biv_rational **initialize_array_br();

void free_br();

//int reduce_br();

biv_rational *copy_br();
biv_rational *add_br();
biv_rational *multiply_br();
biv_rational *negative_br();
biv_rational *reciprocal_br();
biv_rational *pow_br();
biv_rational *subtract_br();
biv_rational *divide_br();

/* End of function defs */

void free_br(biv_rational *brat) {

	free_bp(brat->num);
	free_bp(brat->denom);
	free(brat);
}


//initialize a biv rational struct from a numerator and denominator bivariate poly (denom non zero)
biv_rational *init_br(bpoly *num, bpoly *denom) {

	if(zero_bp(denom)) {
		printf("Error: division by zero bivariate poly\n");
		return NULL;
	}

	else {
		biv_rational *result = (biv_rational *)calloc(1, sizeof(biv_rational));
		result->num = num;
		result->denom = denom;
		return result;
	}
}
//returns rational num/1
biv_rational *bpoly_to_br(bpoly *num) {

	//make one bpoly
	bpoly *onebp = one_bp();

	biv_rational *result;
	result = init_br(num, onebp);
	return result;
}


//initialize an array of biv rationals of length n
biv_rational **initialize_array_br(int n) {
	biv_rational **array_br = (biv_rational **)calloc(n, sizeof(biv_rational *));
	return array_br;
}

//copy
biv_rational *copy_br(biv_rational *brat1) {
	int i;
	biv_rational *duplicate;
	duplicate = init_br(brat1->num, brat1->denom);

	return duplicate;
}

///addition
biv_rational *add_br(biv_rational *brat1, biv_rational *brat2) {

	biv_rational *result;

	bpoly *newnum = add_bp(multiply_bp(brat1->num, brat2->denom), 
			multiply_bp(brat2->num, brat1->denom));
	bpoly *newdenom = multiply_bp(brat1->denom, brat2->denom);

	result = init_br(newnum, newdenom);
	return result;
}

//multiplication 
biv_rational *multiply_br(biv_rational *brat1, biv_rational *brat2) {
	
	biv_rational *result;
	
	bpoly *newnum = multiply_bp(brat1->num, brat2->num);
	bpoly *newdenom = multiply_bp(brat1->denom, brat2->denom);

	result = init_br(newnum, newdenom);
	return result;
}

//negate
biv_rational *negative_br(biv_rational *brat1) {

	biv_rational *result;

	bpoly *newnum = negative_bp(brat1->num);
	result = init_br(newnum, brat1->denom);
	return result;
}

//reciprocal
biv_rational *reciprocal_br(biv_rational *brat1) {
	
	biv_rational *result = init_br(brat1->denom, brat1->num);
	return result;
}

//subtraction
biv_rational *subtract_br(biv_rational *brat1, biv_rational *brat2) {

	biv_rational *neg = negative_br(brat2); 
	biv_rational *result = add_br(brat1, neg);
	free_br(neg);
	return result;
}

//division of two rationals 
biv_rational *divide_br(biv_rational *brat1, biv_rational *brat2) {

	biv_rational *recip = reciprocal_br(brat2);
	biv_rational *result = multiply_br(brat1, recip);
	free_br(recip);
	return result;
}

#endif /* BIV_RATIONALFNS_H */
