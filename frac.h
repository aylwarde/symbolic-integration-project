#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "int_utils.h"

/*defining frac struct*/

typedef struct frac {
  int num;
  int denom;
} frac;

frac *init_f();
void print_f();

frac *reduce_f();
frac *add_f();
frac *multiply_f();
frac *negative_f();
frac *reciprocal_f();
frac *pow_f();

void print_f(frac *frac_a) {

  printf("%d", frac_a->num);

  if (frac_a->denom != 1 ) {
    printf(" / %d", frac_a->denom);
  }

  printf("\n");
  
}


//constructor for fractions; takes arguments for numerator & denominator and returns a pointer to a rational.
frac  *init_f(int num, int denom) {

  if( denom == 0 ){

    printf("Error:");
    return NULL;

  } else {
  
    frac *frac_a= (frac *)calloc(1,sizeof(frac));
    frac_a->num = num;
    frac_a->denom = denom;
    return frac_a;

  }
  
}


//fully reduces a fraction; i.e. returns an equal fraction such that numerator and denomenator are coprime.
frac *reduce_f(frac *frac_a){
  int div = gcd_z( frac_a->num , frac_a->denom );
  
  int newnum = frac_a->num / div;
  int newdenom = frac_a->denom / div;

  return init_f(newnum, newdenom);
}


//adds two fractions
frac *add_f(frac *frac_a, frac *frac_b) {

  int newnum, newdenom;

  newnum = ( frac_a->num * frac_b->denom ) + ( frac_a->denom * frac_b->num);
  newdenom = frac_a->denom * frac_b->denom;
  
  return init_f(newnum, newdenom);
  
}


//multiplies two fractions
frac *multiply_f(frac *frac_a, frac *frac_b) {

  int newnum, newdenom;

  newnum = frac_a->num * frac_b->num;
  newdenom = frac_a->denom * frac_b->denom;

  return init_f(newnum, newdenom);
}


//negates a fraction
frac *negative_f(frac *frac_a) {

  return init_f(-frac_a->num, frac_a->denom);
  
}


//returns the reciprocal of a fraction
frac *reciprocal_f(frac *frac_a) {

  return init_f(frac_a->denom, frac_a->num);
    
}



//exponentiates a fraction to ***INTEGER*** powers
frac *pow_f(frac *frac_a, int exp) {

  if ( exp == 0) {
    
    return init_f(1,1);
    
  } else if ( exp > 0 ) {

    return init_f(pow_z(frac_a->num,exp),pow_z(frac_a->denom,exp));
    
  } else {

    return reciprocal_f(init_f(pow_z(frac_a->num,-exp),pow_z(frac_a->denom,-exp)));
  }
  
}
