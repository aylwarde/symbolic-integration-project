#ifndef RATIONALFNS_H
#define RATIONALFNS_H

#include <stdbool.h>

#include "polynomials.h"

typedef struct rational {

  poly *num, *denom;

} rational;

/* Functions defined in this header */

rational *init_r();

void print_r();
void free_r();

int reduce_r();

rational *add_r();
rational *multiply_r();
rational *negative_r();
rational *reciprocal_r();
rational *pow_r();
rational *subtract_r();
rational *divide_r();

/* End of function defs */

void free_r(rational *rfa) {

  free_p(rfa->num);
  free_p(rfa->denom);

  free(rfa);
  
}

// initialise a rational function struct instance from a numerator polynomial and a denominator polynomial.
// Denominator MUST NOT be identically zero.
rational *init_r(poly *num, poly *denom) {

  if ( zero_p(denom) ) {

    printf("Error\n");
    return NULL;
    
  } else {
      
    rational * result = (rational *)calloc(1, sizeof(rational));
      
    result->num = copy_p(num);
    result->denom = copy_p(denom);

    if ( reduce_r(result) == -1) {

      printf("ERROR\n");
      return NULL;
    
    }

    return result;
    
  }
}

int reduce_r(rational *rfa) {

  poly *gcd = gcd_p(rfa->num, rfa->denom);
  poly *newnum = divide_p(rfa->num, gcd)[0];
  poly *newdenom = divide_p(rfa->denom, gcd)[0];
  
  rfa->num = newnum;
  rfa->denom = newdenom;

  return 0;
}

void print_r(rational *rfa) {
  
  print_p(rfa->num);
  printf("---------------------\n");
  print_p(rfa->denom);
  
}


// you know the drill by now
rational *add_r(rational *rfa, rational *rfb) {
  
  rational *result;

  poly *newnum = add_p(multiply_p( rfa->num, rfb->denom ), multiply_p( rfb->num, rfa->denom ));
  poly *newdenom = multiply_p( rfa->denom, rfb->denom );
  
  result = init_r( newnum, newdenom );
  return result;
}

rational *multiply_r(rational *rfa, rational *rfb) {

  rational *result;

  poly *newnum = multiply_p( rfa->num, rfb->num );
  poly *newdenom = multiply_p( rfa->denom, rfb->denom );

  result = init_r( newnum, newdenom );
  return result;
}

rational *negative_r(rational *rfa) {

  rational *result;
  poly *newnum = negative_p( rfa->num );

  result = init_r( newnum, rfa->denom );
  return result;
}

rational *reciprocal_r(rational *rfa) {

  rational *result = init_r( rfa->denom, rfa->num );
  return result;
}

rational *pow_r(int exp, rational *rfa) {
  if (exp < 0) {

    return pow_r(-exp, reciprocal_r(rfa));
    
  }

  return init_r(pow_p(rfa->num,exp), pow_p(rfa->denom,exp));
}

rational *subtract_r(rational *rfa, rational *rfb) {
  
  rational *neg = negative_r(rfb);
  rational *result = add_r(rfa, neg);
  free_r(neg);
  return result;
}

rational *divide_r(rational *rfa, rational *rfb) {

  rational *recip = reciprocal_r(rfb);
  rational * result = multiply_r(rfa, recip);
  free_r(recip);
  return result;
}

#endif /* RATIONALFNS_H */
