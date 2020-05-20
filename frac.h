#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "int_utils.h"

/*defining frac struct*/

typedef struct frac {
  long num;
  long denom;
} frac;


/* Functions defined in this header */
frac *init_f();

void print_f();
void free_f();

frac *reduce_f();
frac *add_f();
frac *multiply_f();
frac *negative_f();
frac *reciprocal_f();
frac *pow_f();
frac *divide_f();
frac *subtract_f();
frac *gcd_f();
frac *lcm_f();
frac *gcd_array_f();
frac *lcm_array_f();

frac **copy_array_f();

bool zero_f();
bool equal_f();
/* End of function defs*/


void print_f(frac *frac_a) {

  printf("%ld", frac_a->num);

  if (frac_a->denom != 1 ) {
    printf("/%ld", frac_a->denom);
  }

  //printf("\n");
  
}


//free a frac
void free_f(frac *frac_a) {

  free(frac_a);
  
}


//constructor for fractions; takes arguments for numerator & denominator and returns a polonger to a rational.
frac  *init_f(long num, long denom) {

  if( denom == 0 ){

    printf("Error:\n");
    return NULL;

  } else {
  
    frac *frac_a = (frac *)calloc(1,sizeof(frac));
    frac_a->num = num;
    frac_a->denom = denom;
    return frac_a;

  }
  
}


//fully reduces a fraction; i.e. returns an equal fraction such that numerator and denomenator are coprime.
frac *reduce_f(frac *frac_a){
  long div = gcd_z( frac_a->num , frac_a->denom );
  
  long newnum = frac_a->num / div;
  long newdenom = frac_a->denom / div;

  return init_f(newnum, newdenom);
}


//adds two fractions
frac *add_f(frac *frac_a, frac *frac_b) {

  long newnum, newdenom;

  newnum = ( frac_a->num * frac_b->denom ) + ( frac_a->denom * frac_b->num);
  newdenom = frac_a->denom * frac_b->denom;
  
  return init_f(newnum, newdenom);
  
}


//multiplies two fractions
frac *multiply_f(frac *frac_a, frac *frac_b) {

  long newnum, newdenom;

  newnum = frac_a->num * frac_b->num;
  newdenom = frac_a->denom * frac_b->denom;

  return init_f(newnum, newdenom);
}


//negates a fraction
frac *negative_f(frac *frac_a) {

  return init_f(-frac_a->num, frac_a->denom);
  
}


//returns the reciprocal of a fraction - make sure that the return value is not NULL as this may occur if you
//provide a zero input and it will break any subsequent code
frac *reciprocal_f(frac *frac_a) {

  return init_f(frac_a->denom, frac_a->num);
    
}



//exponentiates a fraction to ***INTEGER*** powers
frac *pow_f(frac *frac_a, long exp) {

  if ( exp == 0) {
    
    return init_f(1,1);
    
  } else if ( exp > 0 ) {

    return init_f(pow_z(frac_a->num,exp),pow_z(frac_a->denom,exp));
    
  } else {

    return reciprocal_f(init_f(pow_z(frac_a->num,-exp),pow_z(frac_a->denom,-exp)));
  }
  
}



//subtract frac_b from frac_a
frac *subtract_f(frac *frac_a, frac *frac_b) {

  return  add_f(frac_a, negative_f(frac_b));
  
}


//divide frac_a by frac_b
frac *divide_f(frac *frac_a, frac *frac_b) {

  return multiply_f(frac_a, reciprocal_f(frac_b));
  
}


//returns true if rational zero, false otherwise
bool zero_f(frac *frac_a)
{
	bool result = false;

	if(frac_a->num==0)
	{
		result = true;
	}
	
	return result;
}


//returns true if two rationals are equal, false otw.
bool equals_f(frac *frac_a, frac *frac_b) {

  return zero_f(subtract_f(frac_a, frac_b));
    
}



//copy array of fractions
frac **copy_array_f(frac **src, int len) {
  
  frac **result = (frac **)malloc(len*sizeof(frac));
  int i;
  
  for (i=0;i<len;++i) {
    
    result[i]=src[i];
    
  }
  
  return result;
  
}

//gcd of two fractions
frac *gcd_f(frac *frac_a, frac *frac_b) {

	return init_f(gcd_z(frac_a->num, frac_b->num), lcm_z(frac_a->denom, frac_b->denom));
}

//lcm of two fractions
frac *lcm_f(frac *frac_a, frac *frac_b) {
	
	return init_f(lcm_z(frac_a->num, frac_b->num), gcd_z(frac_a->denom, frac_b->denom));
}

//gcd of an array of fractions
frac *gcd_array_f(int i, frac **frac_array) {

  frac *gcd_array;
  if(i==1)
    {
     gcd_array = gcd_f(frac_array[1], frac_array[0]);
     return gcd_array;
    }
  else
    {
     return gcd_f(gcd_array_f(i-1, frac_array), frac_array[i]);
    }
}

//gcd of an array of fractions
frac *lcm_array_f(int i, frac **frac_array) {

  frac *lcm_array;
  if(i==1)
    {
     lcm_array = lcm_f(frac_array[1], frac_array[0]);
     return lcm_array;
    }
  else
    {
     return lcm_f(lcm_array_f(i-1, frac_array), frac_array[i]);
    }
}

