#include "lazard_rioboo_trager.h"
#include "hermite_reduce.h"

void intergrate_r(rational *rpoly1,rational *g,poly *Q,Logs *Rh)
{
  
  rational **hermite, *rat;
  poly **polydiv;
  void *result;

  hermite = hermite_reduce(rpoly1);

  polydiv = divide_p(hermite[1]->num,hermite[1]->denom);

  g = hermite[0];
  Q = intergrate_p(polydiv[0]);

  if(zero_p(polydiv[1]))
    {
      
      Rh = NULL;

      return;
      
    }
  else
    {
      rat = init_r(polydiv[1],hermite[1]->denom);
      
      Rh = int_rational_log_part(rat);

      return;
    }
}

  

  
