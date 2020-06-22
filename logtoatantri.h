#ifndef LOGTOATANTRI_H
#define LOGTOATANTRI_H

#include "complex_polynomial.h"
#include "trivariate_poly.h"
#include "latex.h"

typedef struct tan_tri {
	tpoly *num, *denom;
} tan_tri;

tan_tri *initialize_t(tpoly *num, tpoly *denom) {

	tan_tri *result = (tan_tri *)calloc(1, sizeof(tan_tri));
	result->num = copy_tp(num);
	result->denom = copy_tp(denom);
	return result;
}
	
tan_tri **initialize_array_t(int n) {
	int i;
	tan_tri **result = (tan_tri **)calloc(n, sizeof(tan_tri *));
	for(i=0; i<n; ++i) {
		result[i] = (tan_tri *)calloc(1, sizeof(tan_tri));
	}
	return result;
}

tan_tri **logtoatantri(tpoly *t_poly1, tpoly *t_poly2, int *outlen) {

	tan_tri **result = initialize_array_t(10);
	tpoly *a, *b, *r, *d, *c, *g, *hold;
	int k=0;

	if(t_poly1->deg < t_poly2->deg) {
		a = negative_tp(t_poly2);
		b = copy_tp(t_poly1);
	}
	
	else {
		a = copy_tp(t_poly1);
		b = copy_tp(t_poly2);
	}

	r = divide_tp(a, b)[1];
	
	while(!zero_tp(r)) {
	
	d = ext_euclid_tp(b, negative_tp(a))[0];
	c = ext_euclid_tp(b, negative_tp(a))[1];
	g = ext_euclid_tp(b, negative_tp(a))[2];
	result[k]->num = add_tp(multiply_tp(a, d), multiply_tp(b, c));
	result[k]->denom = copy_tp(g);
	++k;
	if(d->deg < c->deg) {
		a = negative_tp(c);
		b = d;
	}
	else {
		a = d;
		b = c;
	}
	r = divide_tp(a, b)[1];
	}

	result[k]->num = copy_tp(d);
	result[k]->denom = copy_tp(c);
	*outlen = k+1;

	return result;
}


#endif



