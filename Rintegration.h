#include "lazard_rioboo_trager.h"
#include "hermite_reduce.h"
#include "latex.h"

void integrate_r(rational *rpoly1,rational **g,poly **Q,logpart **Rh)
{
  
  rational **hermite, *rat;
  poly **polydiv;
  void *result;

  hermite = hermite_reduce(rpoly1);

  polydiv = divide_p(hermite[1]->num,hermite[1]->denom);

  *g = hermite[0];
  *Q = integrate_p(polydiv[0]);

  if(zero_p(polydiv[1]))
    {
      
      *Rh = NULL;

      return;
      
    }
  else
    {
      rat = init_r(polydiv[1],hermite[1]->denom);
      
      *Rh = int_rational_log_part(rat);

      return;
    }
}

//this string will include the result, but not "\int{integrand}"
STRING *integral_string(rational *rat_poly, char *var1, char *var2, char *leftbinder, char *rightbinder) {

	//perform integration
	rational *g;
	poly *Q;
	logpart *Rh;
	integrate_r(rat_poly, &g, &Q, &Rh);

	STRING *output;
	output = make_string();
	append_to_string(output, leftbinder);

	if(!zero_p(g->num)) {
		append_to_string(output, latex_rational(g, var2, "", "")->string);
		append_to_string(output, "+");
	}

	if(!zero_p(Q)) {
		append_to_string(output, latex_poly(Q, var2, "", "")->string);
		append_to_string(output, "+");
	}

	if(Rh != NULL) {
		append_to_string(output, latex_Logs(Rh, var1, var2, "", "")->string);
	}
	append_to_string(output, rightbinder);

	return output;
}

//this string will have "\int{integrand} = ...."
STRING *integrate_rational_string(rational *rat_poly, char *var1, char *var2, char *leftbinder, char *rightbinder) {
	
	STRING *output;
	output = make_string();
	append_to_string(output, integrate_rational(rat_poly, var2, leftbinder, "=")->string);
	append_to_string(output, integral_string(rat_poly, var1, var2, "", rightbinder)->string);
	return output;
}
