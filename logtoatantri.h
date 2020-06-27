#ifndef LOGTOATANTRI_H
#define LOGTOATANTRI_H

#include "complex_polynomial.h"
#include "trivariate_poly.h"
#include "latex.h"

typedef struct atan_tri {
	tpoly *num, *denom;
} atan_tri;

atan_tri *initialize_t();
int reduce_t();
atan_tri **initialize_array_t();
atan_tri **logtoatantri();

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

  biv_rational *denom_ct = copy_br(content_tp(input->denom));
  biv_rational *recip_denom_ct = reciprocal_br(denom_ct);
  tpoly *newnum = scale_tp(recip_denom_ct, input->num);
  tpoly *newdenom = scale_tp(recip_denom_ct, input->denom);

  free_br(denom_ct);
  free_br(recip_denom_ct); 
  
  free_tp(input->num);
  free_tp(input->denom);

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
	tpoly *a, *b, *r, *d, *c, *g;
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
	free_tp(r);	
	d = ext_euclid_tp(b, negative_tp(a))[0];
	c = ext_euclid_tp(b, negative_tp(a))[1];
	g = ext_euclid_tp(b, negative_tp(a))[2];
	result[k]= initialize_t(add_tp(multiply_tp(a, d), multiply_tp(b, c)), g);
	free_tp(g);
	++k;
	if(d->deg < c->deg) {
		free_tp(a);
		a = negative_tp(c);
		free_tp(b);
		b = copy_tp(d);
	}
	else {
		free_tp(a);
		free_tp(b);
		a = copy_tp(d);
		b = copy_tp(c);
	}
	r = divide_tp(a, b)[1];
	free_tp(d);
	free_tp(c);
	}
	
	result[k] = initialize_t(a, b);
	*outlen = k+1;
	free_tp(a);
	free_tp(b);
	free_tp(r);

	return result;
}


#endif



