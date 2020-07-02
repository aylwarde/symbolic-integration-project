#ifndef LOGTOATANTRI_H
#define LOGTOATANTRI_H

#include "complex_polynomial.h"
#include "trivariate_poly.h"


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
	tpoly *a, *b, **div, **ext;
	int k=0;

	if(t_poly1->deg < t_poly2->deg) {
		a = negative_tp(t_poly2);
		b = copy_tp(t_poly1);
	}
	
	else {
		a = copy_tp(t_poly1);
		b = copy_tp(t_poly2);
	}

	div = divide_tp(a, b);
	
	while(!zero_tp(div[1])) {
	free_array_tp(div, 2);	
	ext = ext_euclid_tp(b, negative_tp(a));
	result[k]= initialize_t(add_tp(multiply_tp(a, ext[0]), multiply_tp(b, ext[1])), ext[2]);
	++k;
	if(ext[0]->deg < ext[1]->deg) {
		free_tp(a);
		a = negative_tp(ext[1]);
		free_tp(b);
		b = copy_tp(ext[0]);
	}
	else {
		free_tp(a);
		free_tp(b);
		a = copy_tp(ext[0]);
		b = copy_tp(ext[1]);
	}
	div = divide_tp(a, b);
	free_array_tp(ext, 3);
	}
	
	result[k] = initialize_t(a, b);
	*outlen = k+1;
	free_tp(a);
	free_tp(b);
	free_array_tp(div, 2);

	return result;
}


#endif



