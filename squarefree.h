#ifndef SQUAREFREE_H
#define SQUAREFREE_H

#include "polynomials.h"


poly **squarefree_p();


/*returns **a such that poly1= (a[0])^0(a[1])^1(a[2])^2... etc*/
//as such, we set a[0] = 1 for all non-zero poly1
poly **squarefree_p(poly *poly1) {
      	
        poly *s, *d, *e, *y, *z, *b, *onepoly;
        poly **a;
        int k, i;
        frac *c;
        
	//the squarefree factorization will have at most the highest multiplicity factor of the 
	//degree of poly1, so we initialize an array of deg+1 to add a[0]
        a = initialize_array_p(poly1->deg+1);
	
	//exit if poly1 is zero
	if(zero_p(poly1)) {
		exit(1);
	}

	//set a[0] =1
	onepoly = initialize_p(0);
	mpz_t one; mpz_init_set_ui(one, 1);
	onepoly->coefficients[0] = init_f(one, one);
	mpz_clear(one);
	a[0] = onepoly;

        c = content_p(poly1);
        s = scale_p(reciprocal_f(c), poly1); //s primitive part of poly1

        d = gcd_p(s, derivative_p(s)); //deflation of s

	e = divide_p(s, d)[0]; //squarefree part of S
        y = divide_p(derivative_p(s), d)[0];
        k = 1; 

        while(!zero_p(subtract_p(y, derivative_p(e))) )
        {
                z = subtract_p(y, derivative_p(e));
	       	b = gcd_p(e, z);
                e = divide_p(e, b)[0];
                y = divide_p(z, b)[0];
		a[k] = copy_p(b);
		free_p(b);
		++k;
		free_p(z);
        } 
              
        
        a[k] = e;
        a[1] = scale_p(c, a[1]);

	//free subsequent polynomials in array not used
        for(i=k+1; i<poly1->deg; ++i)
        {
                free_p(a[i]);
                a[i] = NULL;
        }

	free_p(y);
	free_p(s);
	free_p(d);

        return a;
}




#endif /* FRAC_H */
