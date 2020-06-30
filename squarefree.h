#ifndef SQUAREFREE_H
#define SQUAREFREE_H

#include "polynomials.h"


poly **squarefree_p();


/*returns **a such that poly1= (a[0])^0(a[1])^1(a[2])^2... etc*/
//as such, we set a[0] = 1 for all non-zero poly1
//outlen is the position of the last non-zero factor in our array
poly **squarefree_p(poly *poly1, int *outlen) {
      	
        poly *s, *d, *e, *z, *b, *onepoly;
        poly **a, **div, **div1;
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
	onepoly = one_p(); 
	a[0] = copy_p(onepoly);
	free_p(onepoly);

        c = content_p(poly1);
        s = scale_p(reciprocal_f(c), poly1); //s primitive part of poly1

        d = gcd_p(s, derivative_p(s)); //deflation of s

	div = divide_p(s, d); //squarefree part of S= div[0]
        div1 = divide_p(derivative_p(s), d);
        k = 1; 

        while(!equals_p(div1[0], derivative_p(div[0])) )
        {
                z = subtract_p(div1[0], derivative_p(div[0]));
	       	b = gcd_p(div[0], z);
		
		e = copy_p(div[0]);
		free_array_p(div, 2);
                div = divide_p(e, b);
		free_p(e);
                
		free_array_p(div1, 2);
		div1 = divide_p(z, b);
		a[k] = copy_p(b);
		free_p(b);
		++k;
		free_p(z);
        } 
              
        
        a[k] = copy_p(div[0]);
        a[1] = scale_p(c, a[1]);
	*outlen = k;

	//make subsequent polynomials in array not used point to null
        for(i=k+1; i<poly1->deg; ++i)
        {
                a[i] = NULL;
        }

	free_p(s);
	free_p(d);
	
	free_f(c);
	free_array_p(div1, 2);
	free_array_p(div,2 );

        return a;
}




#endif /* FRAC_H */
