#include "polynomials.h"

poly **squarefree_p();


/*returns **a such that poly1= (a[0])^1(a[1])^2(a[2])^3... etc*/
poly **squarefree_p(poly *poly1) {
      	
        poly *s, *d, *e, *y, *z, *b;
        poly **a;
        int k, i;
        frac *c;
        
        a = initialize_array_p(poly1->deg);

        c = content_p(poly1);
        s = scale_p(reciprocal_f(c), poly1);

        d = gcd_p(s, derivative_p(s));

	e = divide_p(s, d)[0];
        y = divide_p(derivative_p(s), d)[0];
        k = 0; 

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
        a[0] = scale_p(c, a[0]);

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





