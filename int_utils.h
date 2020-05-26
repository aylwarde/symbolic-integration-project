#ifndef INT_UTILS_H
#define INT_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

//#define MAX(x, y) (((x) > (y)) ? (x) : (y))
//#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void max_z();
void min_z();

void gcd_z();
void lcm_z();
void pow_z();


void max_z(mpz_t max, mpz_t a, mpz_t b) {

	if(mpz_cmp(a,b)>0) {
		mpz_set(max, a);
	}

	else {
		mpz_set(max, b);
	}
}


void min_z(mpz_t min, mpz_t a, mpz_t b) {

	if(mpz_cmp(a,b)>0) {
		mpz_set(min, b);
	}

	else {
		mpz_set(min, a);
	}
}

void gcd_z(mpz_t gcd, mpz_t a, mpz_t b) {
   	
	mpz_t a_abs; mpz_init(a_abs);
	mpz_t b_abs; mpz_init(b_abs);

	mpz_abs(a_abs, a);
	mpz_abs(b_abs, b);

 	 if(mpz_cmp(a_abs,b_abs)==0) {
    
   		 mpz_set(gcd, a_abs);
    	 } 
	
	 else {
    
    		if(mpz_cmp_si(a_abs,0)==0 || mpz_cmp_si(b_abs,0)==0) {
      			
			mpz_t max; mpz_init(max);
			max_z(max, a_abs, b_abs);

      			mpz_set(gcd, max);
      
   		 } 
		
		else {
			mpz_t r; mpz_init(r);
      			mpz_t max; mpz_init(max);
			mpz_t min; mpz_init(min);

			max_z(max, a_abs, b_abs);
			min_z(min, a_abs, b_abs);

			mpz_cdiv_r(r, max, min);

			gcd_z(gcd, min, r); 
			
    		}
  	}


}

void lcm_z(mpz_t lcm, mpz_t a, mpz_t b) {

	mpz_t a_abs; mpz_init(a_abs);
	mpz_t b_abs; mpz_init(b_abs);
	
	mpz_abs(a_abs, a);
	mpz_abs(b_abs, b);
	
	mpz_t mult; mpz_init(mult);
	mpz_t gcd; mpz_init(gcd);

	mpz_mul(mult, a_abs, b_abs);
	gcd_z(gcd, a, b);
	mpz_cdiv_q(lcm, mult, gcd);

}

void pow_z(mpz_t pow, mpz_t b, int exp) {
  	
	if (exp==0) {

		mpz_set_ui(pow, 1);
 	 } 
	
	else {	
		pow_z(pow, b, exp-1);
		mpz_mul(pow, pow, b);

	}
}

#endif /* INT_UTILS_H */
