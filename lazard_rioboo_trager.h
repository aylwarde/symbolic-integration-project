#ifndef LAZARD_RIOBOO_TRAGER_H
#define LAZARD_RIOBOO_TRAGER_H

#include "squarefree.h"
#include "euclidean_algorithms.h"
#include "bivariate_poly.h"
#include "bivariateResultant.h"
#include "rationalfns.h"

//FNs:
void evaluate();
void make_monic();
void int_rational_log_part();

//evaluate a bivariate poly in K(t)(x) at t=a where a is st polya(a)=0 for polya in K(t)
//result is bivariate poly in K(a)(x)
void evaluate(bpoly *b_poly, poly *polya) {
	int i;
	for(i=0; i<=b_poly->deg; ++i) {
		b_poly->pcoefficients[i] = divide_p(b_poly->pcoefficients[i], polya)[1];
	}
	display_bp(b_poly);
}

//for a bivariate poly in K(a)(x), with a in closure of K st polya(a)=0, make bivariate poly monic
void make_monic(bpoly *b_poly, poly *polya) {
	int i;
	for(i=0; i<=b_poly->deg; ++i) {
		//note this is always possible since leading coefficient and Q have trivial gcd
		b_poly->pcoefficients[i] = extended_euclidean_diophantine(
				b_poly->pcoefficients[i], b_poly->pcoefficients[0], polya)[0];
	}

}

//input: a rational polynomial A/D with deg(A)<deg(D) and D squarefree, an unitialized array of polys, an unitialized array of bpolys, outlen int address
//squarefree_r will be the squarefree factorization of resultant(D, A-t(dD/dx)) (with outlen terms)
void int_rational_log_part(rational *rat_poly, poly **squarefree_r, bpoly **S, int *outlen) {
	
	poly *A, *D;
	bpoly *d, *a;
	bpoly **subresultant;
	
	//copy numerator and denominator(univariate polys)
	A = copy_p(rat_poly->num);
	D = copy_p(rat_poly->denom);

	//we introduce t, an indeterminate over Q
	//define d as a polynomial over t of degree 0  
	d = initialize_bp(0);
	d->pcoefficients[0] = D; //set D to const coeff

	//construct (A-t*derivative_x(D))
	a = initialize_bp(1);
	a->pcoefficients[0] = negative_p(derivative_p(D));
	a->pcoefficients[1] = A;

	//change a, d to polynomials over x with coeffs in Q(t)
	a = variable_change(a);
	d = variable_change(d);

	//prs algorithm in two variables
	int k;
	subresultant = bsubresultant(d, a, &k);

	//squarefree factorization of resultant of d, a (resultant is in Q(t))
	squarefree_r = squarefree_p(subresultant[0]->pcoefficients[0], outlen);

	int i, j, h;
	poly **squarefree_a;
	
	//S contains corresponding log arguments for the residues that are sols of our 
	//squarefree factors of the resultant
	S = initialize_array_bp(*outlen);
	
	for(i=1; i<=*outlen; ++i) {
		//if squarefree factor non constant
		if(squarefree_r[i]->deg>0) {
			if(i==D->deg) {
				S[i] = d;
			}

			else {
				j = 0;
				//find member of subresultant prs that has degree i		
				while(j<k) {
					if(subresultant[j]->deg == i) {
						S[i] = subresultant[j];
					}
					++j;
				}

				//make S[i] primitive wrt x
				int terms;
				poly *gcd;

				//squarefree factorization of lc(S[i])
				squarefree_a = squarefree_p(S[i]->pcoefficients[0], &terms);
				
				for(h=1; h<=terms; ++h) {
					gcd = pow_p(h, gcd_p(squarefree_a[h], squarefree_r[i]));
					
					//gcd(lc(S[i], Q[i]) has trivial gcd, so so does gcd poly
					if(gcd->deg==0) {
						gcd->coefficients[0] = reciprocal_f(gcd->
								coefficients[0]);
						scale_bp(gcd, S[i]);			
					}
	
				}
				
			}
		}
		//set S[i] to zero if degree of squarefree factor is zero
		else {
			S[i] = initialize_bp(0);
		}
	}

	//evaluate log arguments at respective squarefree_r[i] and make monic, for nonzero S
	for(i=1; i<=*outlen; ++i) {
		if(!zero_bp(S[i])) {
			evaluate(S[i], squarefree_r[i]);
			make_monic(S[i], squarefree_r[i]);
		}
	}
}



#endif /*LAZARD_RIOBOO_TRAGER_H */
