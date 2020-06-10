#ifndef SUBRESULTANT_H
#define SUBRESULTANT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "int_utils.h"
#include "polynomials.h"


#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


/* Fn Defs */

poly **subresultant(poly *, poly *, int *);

/* End Fn Defs */


/* 
   returns the resultant of polya and polyb in result[0] and the subresultant PRS of polya, polyb in the 
   remainder of the array. Arguments should be such that polya->deg >= polyb->deg

   See Bronstein pp.24 for pseudocode
*/
poly **subresultant_p(poly * polya, poly *polyb, int *outlen) {

  /* initialising variables and arrays */
 
  poly **result = initialize_array_p(MAX(polya->deg, polyb->deg)+3);
  poly **div;
  frac **r, **beta, *quo, *gamma;
  mpz_t one, negone;
  int k = 2, delta[MAX(polya->deg, polyb->deg)+2];
  

  r = (frac **)calloc(MAX(polya->deg, polyb->deg)+2, sizeof(frac *));
  beta = (frac **)calloc(MAX(polya->deg, polyb->deg)+2, sizeof(frac *));
  
  mpz_init_set_ui(one, 1);
  mpz_init(negone);
  mpz_neg(negone, one);

  result[1] = copy_p(polya);
  result[2] = copy_p(polyb);

  gamma = init_f(negone, one);
  delta[0] = polya->deg - polyb->deg;
  beta[0] = pow_f(gamma, delta[0]+1);

  r[0] =result[1]->coefficients[0];

  /* end initialisation */

  
  /* 
     terminating condition is fk=0 (as with euclidean algorithm etc.), the while loop generates the
     subresultant PRS
  */
  while ( !(zero_p(result[k])) ) {
    
    r[k-1] = result[k]->coefficients[0] ;
    div = pseudodiv_p(result[k-1], result[k]);
    
    result[k+1] = scale_p(reciprocal_f(beta[k-2]), div[1]);

    if(delta[k-2]>1)
      {
	gamma = divide_f(pow_f(negative_f(r[k-1]),delta[k-2]),pow_f(gamma,delta[k-2]-1));
      }
    else
      {
       gamma = multiply_f(pow_f(negative_f(r[k-1]), delta[k-2]), pow_f(gamma, 1 - delta[k-2]));
      }
      
    delta[k-1] = result[k]->deg-result[k+1]->deg;
    beta[k-1] = multiply_f(negative_f(r[k-1]), pow_f(gamma, delta[k-1]));

    ++k;
  }

  int i = k-1;
  *outlen = k+1;

  /* reconstructing the resultant from the PRS */
  
  if ( result[i]->deg > 0 ) {

    result[0] = initialize_p(0);

    return result;
    
  } else if ( result[i-1]->deg == 1 ) {

    result[0] = copy_p(result[i]);
    return result;
    
  } else {

    frac *s, *c;
    s = init_f(one, one);
    c = init_f(one, one);
    
    int j;

    for (j=2; j<k; ++j) {
      if ( !((result[j-1]->deg % 2) || (result[j]->deg % 2)) ) {
	s = negative_f(s);
      }

      c = multiply_f( multiply_f( c, pow_f( divide_f( beta[j-2], pow_f(r[j-2], 1+delta[j-2]) ), result[j]->deg ) ), pow_f( r[j-2], result[j-1]->deg - result[j+1]->deg ) );
      
    }

    result[0] = scale_p( multiply_f(s, c), pow_p( result[i-1]->deg, result[i] ) );
    return result;
  }
}
  
#endif /* SUBRESULTANT_H */
