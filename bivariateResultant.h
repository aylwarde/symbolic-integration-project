#ifndef BIVARIATERESULTANT_H
#define BIVARIATERESULTANT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "int_utils.h"
#include "polynomials.h"
#include "bivariate_poly.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


/* Fn Defs */

bpoly **bsubresultant(bpoly *, bpoly *, int *);

/* End Fn Defs */

//returns the resultant with respect to t of the bivarite polynomial struct with coefficents Q1[x],Q2[x], ....
/* 
   returns the resultant of polya and polyb in result[0] and the subresultant PRS of polya, polyb in the 
   remainder of the array. Arguments should be such that polya->deg >= polyb->deg

   See Bronstein pp.24 for pseudocode
*/
bpoly **bsubresultant( bpoly *polya, bpoly *polyb, int *outlen) {

  /* initialising variables and arrays */
 
  bpoly **result = initialize_array_bp(MAX(polya->deg, polyb->deg)+3);
  bpoly **div;
  poly **r, **beta, *quo, *gamma, *temp;
  mpz_t one, negone;
  int k = 2, delta[MAX(polya->deg, polyb->deg)+2],j, l;
  

  r = initialize_array_p(MAX(polya->deg, polyb->deg)+2);
  beta = initialize_array_p(MAX(polya->deg, polyb->deg)+2);
  
  mpz_init_set_ui(one, 1);
  mpz_init(negone);
  mpz_neg(negone, one);

  result[1] = copy_bp(polya);
  result[2] = copy_bp(polyb);

  gamma=initialize_p(0);
  gamma->coefficients[0]=init_f(negone,one);
  delta[0] = polya->deg - polyb->deg;
  beta[0] = pow_p( gamma,delta[0]+1);

  r[0] = result[1]->pcoefficients[0];
  /* end initialisation */

  
  /* 
     terminating condition is fk=0 (as with euclidean algorithm etc.), the while loop generates the
     subresultant PRS
  */
  while ( !(zero_bp(result[k])) ) {

    r[k-1] = result[k]->pcoefficients[0]; 
    div = pseudo_divide_bp(result[k-1], result[k]);
    result[k+1] = initialize_bp(div[1]->deg);
    for (l=0; l<=div[1]->deg; ++l) {
      result[k+1]->pcoefficients[l] = divide_p(div[1]->pcoefficients[l], beta[k-2])[0];
    }
    if(delta[k-2]>1) {
      gamma = divide_p(pow_p( negative_p(r[k-1]),delta[k-2]), pow_p( gamma,delta[k-2]-1))[0];
    }
    else {
      gamma = multiply_p( pow_p( negative_p(r[k-1]),delta[k-2]), pow_p( gamma, 1-delta[k-2] ));
    }
    delta[k-1] = result[k]->deg - result[k+1]->deg;
    beta[k-1] = multiply_p( negative_p(r[k-1]), pow_p( gamma ,delta[k-1]));
    ++k;
    
  }
  int i = k-1;
  *outlen = k+1;

  /* reconstructing the resultant from the PRS */
  
  if ( result[i]->deg > 0 ) {

    result[0] = initialize_and_zero_bp(0);

    return result;
    
  } else if ( result[i-1]->deg == 1 ) {

    result[0] = copy_bp(result[i]);
    return result;
    
  } else {

    poly *s,*c;
    s = initialize_p(0);
    c = initialize_p(0);
    s->coefficients[0] = init_f(one, one);
    c->coefficients[0] = init_f(one,one);
    
    int j;

    for (j=2; j<k; ++j) {
      if ( !((result[j-1]->deg % 2) || (result[j]->deg % 2)) ) {
	s = negative_p(s);
      }

      c = multiply_p( multiply_p( c, pow_p(divide_p(beta[j-2], pow_p( r[j-2],1+delta[j-2]) )[0] ,result[j]->deg) ), pow_p( r[j-2], result[j-1]->deg - result[j+1]->deg ) );
      
    }
    result[0] = scale_bp( multiply_p(c, s), pow_bp( result[i],result[i-1]->deg ) );
    return result;
  }
}
#endif /* BIVARIATERESULTANT_H */
