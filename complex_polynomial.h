#ifndef COMPLEX_POLYNOMIAL_H
#define COMPLEX_POLYNOMIAL_H


#include "polynomials.h"
#include "bivariate_poly.h"
#include "trivariate_poly.h"

//computes n!
int factorial(int n) {
	if(n==0) {
		return 1;
	}

	if(n<0) {
		printf("Error: enter non-negative integer\n");
		exit(1);
	}
	else {

		int i, result=n;
		for(i=n-1; i>=1; --i) {
			result = result*i;
		}
		return result;
	}
}

//compute n choose k
int choose(int n, int k) {
	int result;
	if(n<k){
		printf("Error: require input1>=input2");
		exit(1);
	}
	result = factorial(n)/(factorial(k)*factorial(n-k)); //div exact
	return result;
}

//change choose to frac
frac *choose_f(int n , int k) {

	int choose_int;
	choose_int = choose(n,k);

	mpz_t num; mpz_init_set_si(num, choose_int);
	mpz_t one; mpz_init_set_si(one, 1);

	frac *result;
	result = init_f(num, one);
	mpz_clears(one, num, NULL);

	return result;
}



//takes z^n and computes (x+iy)^n, then returns the real and imaginary parts in an array of bpolys
//we'll return polynomials in Q[x][y]
bpoly **monomial_complexify(int n) {
	bpoly **result, *add;
	result = initialize_array_bp(2);
	
	result[0] = initialize_bp(n); //real part
	result[1] = initialize_bp(n); //imaginary part
	
	int k;
	for(k=0; k<=n; ++k) {
		if(k%4==0) {
			add = initialize_bp(k);
			add->pcoefficients[0] = initialize_p(n-k);
			add->pcoefficients[0]->coefficients[0] = choose_f(n,k);
			result[0] = add_bp(add, result[0]);
		}

		else if(k%4==1) {
			add = initialize_bp(k);
			add->pcoefficients[0] = initialize_p(n-k);
			add->pcoefficients[0]->coefficients[0] = choose_f(n,k);
			result[1] = add_bp(add, result[1]);
		}

		else if(k%4==2) {
			add = initialize_bp(k);
			add->pcoefficients[0] = initialize_p(n-k);
			add->pcoefficients[0]->coefficients[0] = negative_f(choose_f(n,k));
			result[0] = add_bp(add, result[0]);
		}

		else if(k%4==3) {
			add = initialize_bp(k);
			add->pcoefficients[0] = initialize_p(n-k);
			add->pcoefficients[0]->coefficients[0] = negative_f(choose_f(n,k));
			result[1] = add_bp(add, result[1]);
		}
	
		free(add);
	}	

	return result;
}

//sends R(z)->P(x,y) +iQ(x,y) with P=result[0], Q=result[1].. retured as P[x][y], Q[x][y]
bpoly **poly_complexify(poly *poly1) {

	bpoly **result;
	result = initialize_array_bp(2);
	result[0] = initialize_bp(0);
	result[1] = initialize_bp(0);
	int i;

	for(i=0; i<=poly1->deg; ++i) {
		poly *scale;
		scale = initialize_p(0);
		scale->coefficients[0] = poly1->coefficients[i];
		result[0] = add_bp(result[0], scale_bp(scale, monomial_complexify(poly1->deg-i)[0]));
		result[1] = add_bp(result[1], scale_bp(scale, monomial_complexify(poly1->deg-i)[1]));
	}

	return result;
}

//sends S(a,x)->A(u,v,x)+iB(u,v,x) for a=u+iv, A=result[0]=A[u][v][x], B=result[1]=B[u][v][x] 
tpoly **bpoly_complexify(bpoly *bpoly1) {

	tpoly **result;
	result = initialize_array_tp(2);
	result[0] = initialize_tp(bpoly1->deg);
	result[1] = initialize_tp(bpoly1->deg); //same degree in x
	int i;

	for(i=0; i<=bpoly1->deg; ++i ) {
		result[0]->brcoefficients[i] = bpoly_to_br
			(poly_complexify(bpoly1->pcoefficients[i])[0]);
		result[1]->brcoefficients[i] = bpoly_to_br(
				poly_complexify(bpoly1->pcoefficients[i])[1]);
	}

	return result;
}

#endif /* COMPLEX_POLYNOMIAL_H */
