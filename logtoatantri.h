#ifndef LOGTOATANTRI_H
#define LOGTOATANTRI_H

#include "complex_polynomial.h"
#include "trivariate_poly.h"
#include "latex.h"

typedef struct atan_tri {
	tpoly *num, *denom;
} atan_tri;

int reduce_t();

atan_tri *initialize_t(tpoly *num, tpoly *denom) {

	atan_tri *result = (atan_tri *)calloc(1, sizeof(atan_tri));
	result->num = copy_tp(num);
	result->denom = copy_tp(denom);

	if(reduce_t(result) == -1) {
		printf("error\n");
		return NULL;
	}

	return result;
}

int reduce_t(atan_tri *input) {

	tpoly *newnum = scale_tp(reciprocal_br(content_tp(input->denom)), input->num);
	tpoly *newdenom = scale_tp(reciprocal_br(content_tp(input->denom)), input->denom);
	free(input->num);
	free(input->denom);

	input->num = newnum;
	input->denom = newdenom;

	return 0;
}
	
atan_tri **initialize_array_t(int n) {
	int i;
	atan_tri **result = (atan_tri **)calloc(n, sizeof(atan_tri *));
	return result;
}

atan_tri **logtoatantri(tpoly *t_poly1, tpoly *t_poly2, int *outlen) {

	atan_tri **result = initialize_array_t(MAX(t_poly1->deg, t_poly2->deg)+3);
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
	result[k]= initialize_t(add_tp(multiply_tp(a, d), multiply_tp(b, c)), copy_tp(g));
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
	
	result[k] = initialize_t(copy_tp(a), copy_tp(b));
	*outlen = k+1;

	return result;
}


#endif


