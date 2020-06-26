#ifndef RATIONALFNS_H
#define RATIONALFNS_H

#include <stdbool.h>

#include "polynomials.h"

typedef struct rational {

  poly *num, *denom;

} rational;

/* Functions defined in this header */

rational *init_r();
rational **initialize_array_r();
rational *copy_r();
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

rational **initialize_array_r(int len) {
	rational **result = (rational **)calloc(len, sizeof(rational *));
	return result;
}

int reduce_r(rational *rfa) {

  //find primitive gcd
  poly *gcd = gcd_p(rfa->num, rfa->denom);
  poly **numdiv = divide_p(rfa->num, gcd);
  poly **denomdiv = divide_p(rfa->denom, gcd);

  poly *newnum = copy_p(numdiv[0]);
  poly *newdenom = copy_p(denomdiv[0]);

  free_array_p(numdiv, 2);
  free_array_p(denomdiv, 2);
  
  //find gcd of contents
  frac *gcd_content  = gcd_f(content_p(newnum), content_p(newdenom));
  newnum = scale_p(reciprocal_f(gcd_content), newnum);
  newdenom = scale_p(reciprocal_f(gcd_content), newdenom);
  
  free_p(rfa->num);
  free_p(rfa->denom);
  free_p(gcd);
  free_f(gcd_content);
  
  rfa->num = newnum;
  rfa->denom = newdenom;

  return 0;
}

void print_r(rational *rfa) {
  
  print_p(rfa->num);
  printf("---------------------\n");
  print_p(rfa->denom);
  
}

//copy rational 
rational *copy_r(rational *rfa) {

	rational *duplicate = (rational *)calloc(1, sizeof(rational));
	duplicate->num = copy_p(rfa->num);
	duplicate->denom = copy_p(rfa->denom);
	return duplicate;
}


// you know the drill by now
rational *add_r(rational *rfa, rational *rfb) {
  
  rational *result;

  poly *newnum = add_p(multiply_p( rfa->num, rfb->denom ), multiply_p( rfb->num, rfa->denom ));
  poly *newdenom = multiply_p( rfa->denom, rfb->denom );
  
  result = init_r( newnum, newdenom );
  free_p(newnum);
  free_p(newdenom);
  return result;
}

rational *multiply_r(rational *rfa, rational *rfb) {

  rational *result;

  poly *newnum = multiply_p( rfa->num, rfb->num );
  poly *newdenom = multiply_p( rfa->denom, rfb->denom );

  result = init_r( newnum, newdenom );
  free_p(newnum);
  free_p(newdenom);
  return result;
}

rational *negative_r(rational *rfa) {

  rational *result;
  poly *newnum = negative_p( rfa->num );

  result = init_r( newnum, rfa->denom );
  free_p(newnum);
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

rational *gcd_r(rational *rat1, rational *rat2) {
	
	rational *result;
	poly *num = gcd_p(rat1->num, rat2->num);
	poly *denom =lcm_p(rat1->denom, rat2->denom);
	result = init_r(num, denom);
	free_p(num);
	free_p(denom);
	return result;
}

rational *gcd_array_r(int i, rational **rat_array) {

	rational *gcd_array;

	if(i==0) {
		return rat_array[0];
	}

	else if(i==1) {
		gcd_array = gcd_r(rat_array[1], rat_array[0]);
		return gcd_array;
	}

	else {
		return gcd_r(gcd_array_r(i-1, rat_array), rat_array[i]);
	}
}

	
#endif /* RATIONALFNS_H */
