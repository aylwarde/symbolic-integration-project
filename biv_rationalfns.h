#ifndef BIV_RATIONALFNS_H
#define BIV_RATIONALFNS_H

#include "polynomials.h"
#include "field_extension.h"

typedef struct biv_rational {

	field_extension *num, *denom;
} biv_rational;

/*Functions defined in this header */

biv_rational *init_br();
biv_rational *bpoly_to_br();
biv_rational **initialize_array_br();

void free_br();
int reduce_br();

biv_rational *copy_br();
biv_rational *add_br();
biv_rational *multiply_br();
biv_rational *negative_br();
biv_rational *reciprocal_br();
biv_rational *pow_br();
biv_rational *subtract_br();
biv_rational *divide_br();
biv_rational *gcd_br();
biv_rational *gcd_array_br();

/* End of function defs */

void free_br(biv_rational *brat) {

	free_fe(brat->num);
	free_fe(brat->denom);
	free(brat);
}


//initialize a biv rational struct from a numerator and denominator bivariate poly (denom non zero)
biv_rational *init_br(field_extension *num, field_extension *denom) {

	if(zero_fe(denom)) {
		printf("Error, division by zero bivariate poly\n");
		return NULL;
	}

	else {
	
		biv_rational *result = (biv_rational *)calloc(1, sizeof(biv_rational));
		result->num = copy_fe(num);
		result->denom = copy_fe(denom);

		if( reduce_br(result) == -1) {
			printf("ERROR\n");
			return NULL;
		}

		return result;
	}
}

//returns rational num/1
biv_rational *bpoly_to_br(bpoly *num) {

	//make one bpoly
	bpoly *onebp = one_bp();

	biv_rational *result;
	result = init_br(bp_to_fe(num), bp_to_fe(onebp));
	return result;
}


//initialize an array of biv rationals of length n
biv_rational **initialize_array_br(int n) {
	biv_rational **array_br = (biv_rational **)calloc(n, sizeof(biv_rational *));
	return array_br;
}

int reduce_br(biv_rational *brat) {
	
	field_extension *gcd = gcd_fe(brat->num, brat->denom);
	field_extension *newnum = divide_fe(brat->num, gcd)[0]; //exact division
	field_extension *newdenom = divide_fe(brat->denom, gcd)[0]; //exact division

	newnum = scale_fe(reciprocal_r(content_fe(newdenom)), newnum); 
	newdenom = scale_fe(reciprocal_r(content_fe(newdenom)), newdenom); 
/*	
	if(newdenom->deg==0) {
		bpoly *onebp = one_bp();
		newnum = scale_fe(newdenom->rcoefficients[0], newnum);
		newdenom = bp_to_fe(onebp);
	}
*/
	// cleaning up
	field_extension *oldnum = brat->num, *olddenom = brat->denom;
	free_fe(oldnum);
	free_fe(olddenom);
	free_fe(gcd);

	brat->num = newnum; 
	brat->denom = newdenom;

	return 0;
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

	field_extension *newnum = add_fe(multiply_fe(brat1->num, brat2->denom), 
			multiply_fe(brat2->num, brat1->denom));
	field_extension *newdenom = multiply_fe(brat1->denom, brat2->denom);

	result = init_br(newnum, newdenom);
	return result;
}

//multiplication 
biv_rational *multiply_br(biv_rational *brat1, biv_rational *brat2) {
	
	biv_rational *result;
	
	field_extension *newnum = multiply_fe(brat1->num, brat2->num);
	field_extension *newdenom = multiply_fe(brat1->denom, brat2->denom);

	result = init_br(newnum, newdenom);
	return result;
}

//negate
biv_rational *negative_br(biv_rational *brat1) {

	biv_rational *result;

	field_extension *newnum = negative_fe(brat1->num);
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
	return result;
}

//division of two rationals 
biv_rational *divide_br(biv_rational *brat1, biv_rational *brat2) {

	biv_rational *recip = reciprocal_br(brat2);
	biv_rational *result = multiply_br(brat1, recip);
	return result;
}

biv_rational *gcd_br(biv_rational *brat1, biv_rational *brat2) {
	
	biv_rational *result;
	field_extension *num = gcd_fe(brat1->num, brat2->num);
	field_extension *denom = lcm_fe(brat1->denom, brat2->denom);
	result = init_br(num, denom);
	free_fe(num);
	free_fe(denom);
	return result;
}

biv_rational *gcd_array_br(int i, biv_rational **brat_array) {

	biv_rational *gcd_array;

	if(i==0) {
		return brat_array[0];
	}

	else if(i==1) {
		gcd_array = gcd_br(brat_array[1], brat_array[0]);
		return gcd_array;
	}

	else {
		return gcd_br(gcd_array_br(i-1, brat_array), brat_array[i]);
	}
}


#endif /* BIV_RATIONALFNS_H */
