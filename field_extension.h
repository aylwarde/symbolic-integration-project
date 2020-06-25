#ifndef FIELD_EXTENSION_H
#define FIELD_EXTENSION_H

#include "rationalfns.h"
#include "polynomials.h"

typedef struct field_extension {
	int deg;
	rational **rcoefficients;
} field_extension;

void print_fe();
void free_fe();
void strip_fe();

field_extension *initialize_fe();
field_extension **initialize_array_fe();
field_extension *copy_fe();
field_extension *negative_fe();
field_extension *add_fe();
field_extension *subtract_fe();
field_extension *multiply_fe();
field_extension **divide_fe();
field_extension *gcd_fe();

bool zero_fe();
bool equals_fe();


//initialize all coefficients to zero
field_extension *initialize_fe(int degree) {
	
	int i;
	poly *onep = one_p();
	poly *zerop = initialize_p(0);
	field_extension *result = (field_extension *)calloc(1, sizeof(field_extension));
	
	result->deg = degree;
	rational **rcoefficients = initialize_array_r(degree+1);

	for(i=0; i<=degree; ++i) {
		rcoefficients[i] = init_r(zerop, onep);
	}
	
	result->rcoefficients = rcoefficients;

	return result;
}

//initialize an array of n struct
field_extension **initialize_array_fe(int n) {

	field_extension **array = (field_extension **)calloc(n, sizeof(field_extension *));
	return array;
}

//free
void free_fe(field_extension *poly) {

	free(poly->rcoefficients);
	free(poly);
}

//check if poly is zero
bool zero_fe(field_extension *poly) {
	int i;
	bool result = true;
	for(i=0; i<=poly->deg; ++i) {
		if(!zero_p(poly->rcoefficients[i]->num)) {
				result = false;
		}
	}
	return result;
}


//strip poly of higher order terms with zero coeffs
void strip_fe(field_extension *poly) {
	int i, leading_zeroes=0, degree;

	if(zero_fe(poly)) {
		poly->deg = 0;
	}

	else {

		while(zero_p(poly->rcoefficients[leading_zeroes]->num)) {
			++leading_zeroes;
		}

		for(i=0; i<=poly->deg-leading_zeroes; ++i) {
			poly->rcoefficients[i] = copy_r(poly->rcoefficients[i+leading_zeroes]);
		}
		degree = poly->deg-leading_zeroes;
		poly->deg = degree;
	}
}

//copy a polynomial strucute
field_extension *copy_fe(field_extension *poly) {

	int i;
	field_extension *duplicate;
	duplicate = initialize_fe(poly->deg);
	
	for(i=0; i<=poly->deg; ++i) {
		duplicate->rcoefficients[i] = poly->rcoefficients[i];
	}

	return duplicate;
}

//negate polynomial
field_extension *negative_fe(field_extension *poly1) {
	int i;
	field_extension *result;

	result = initialize_fe(poly1->deg);

	for(i=0; i<=poly1->deg; ++i) {
		result->rcoefficients[i] = negative_r(poly1->rcoefficients[i]);
	}
	
	return result;
}

//add two polys
field_extension *add_fe(field_extension *poly1, field_extension *poly2) {

	int i, difference;
	field_extension *result;

	if(poly1->deg < poly2->deg) {
		result = add_fe(poly2, poly1);
	}

	else {
		difference = poly1->deg - poly2->deg;
		result = initialize_fe(poly1->deg);

		for(i=0; i<difference; ++i) {
			result->rcoefficients[i] = poly1->rcoefficients[i];
		}

		for(i=difference; i<=result->deg; ++i) {
			result->rcoefficients[i] 
			= add_r(poly1->rcoefficients[i], poly2->rcoefficients[i-difference]);
		}
	}
		strip_fe(result);
		return result;
}

//subtract two polys
field_extension *subtract_fe(field_extension *poly1, field_extension *poly2) { 
	field_extension *result;
	result = add_fe(poly1, negative_fe(poly2));
	return result;
}

//polynomial multiplication
field_extension *multiply_fe(field_extension *poly1, field_extension *poly2) { 

	int i,j;
	field_extension *result;

	result = initialize_fe(poly1->deg + poly2->deg);

	for(i=0; i<= poly1->deg; ++i) {
		for(j=0; j<= poly2->deg; ++j) {
			result->rcoefficients[i+j] 
				= add_r(result->rcoefficients[i+j], 
						multiply_r(poly1->rcoefficients[i], 
							poly2->rcoefficients[j]));
		}
	}
	strip_fe(result);
	return result;
}

bool equals_fe(field_extension *poly1, field_extension *poly2) {
	
	return zero_fe(subtract_fe(poly1, poly2));
}


//divison
field_extension **divide_fe(field_extension *poly1, field_extension *poly2) {

	field_extension **result = initialize_array_fe(2);
	
	if(zero_fe(poly2)) {
		printf("Error: division by zero polynomial\n");
		exit(0);
	}

	else {
		int d=0;
		rational *t;
		field_extension *division;

		field_extension *quotient = initialize_fe(MAX(poly1->deg - poly2->deg, 0));
		field_extension *remainder = copy_fe(poly1);

		while(!zero_fe(remainder) && (remainder->deg - poly2->deg)>=0) {

			d = remainder->deg - poly2->deg;
			t = divide_r(remainder->rcoefficients[0], poly2->rcoefficients[0]);
			division = initialize_fe(d);
			division->rcoefficients[0] = t;

			quotient = add_fe(quotient, division);
			remainder = subtract_fe(remainder, multiply_fe(poly2, division));
		}
		result[0] = quotient;
		result[1] = remainder;
	}
	return result;
}

field_extension *gcd_fe(field_extension *polynomial1, field_extension *polynomial2) {

        field_extension *r;

        while(!zero_fe(polynomial2))
        {
                r = divide_fe(polynomial1, polynomial2)[1];
                polynomial1 = copy_fe(polynomial2);
                polynomial2 = copy_fe(r);
        }
	
        return polynomial1;
}

field_extension *lcm_fe(field_extension *polynomial1, field_extension *polynomial2) {

	field_extension *lcm;
	lcm = divide_fe(multiply_fe(polynomial1, polynomial2), gcd_fe(polynomial1, polynomial2))[0];
	return lcm;
}

field_extension *bp_to_fe(bpoly *polynomial) {
	
	int i;
	field_extension *result = initialize_fe(polynomial->deg);
	poly *onep; 
	onep = one_p();
	
	for(i=0; i<=polynomial->deg; ++i) {
		result->rcoefficients[i] = init_r(polynomial->pcoefficients[i], onep);
		}
	return result;
}

field_extension *scale_fe(rational *scalar, field_extension *poly) {
	int i;
	field_extension *result = initialize_fe(poly->deg);

	for(i=0; i<= poly->deg; ++i) {
		result->rcoefficients[i] = multiply_r(scalar, poly->rcoefficients[i]);
	}

	return result;
}

rational *content_fe(field_extension *poly) {
	
	rational *gcd = gcd_array_r(poly->deg, poly->rcoefficients);
	return gcd;
}
		
	
#endif /* FIELD_EXTENSION_H */
