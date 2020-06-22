#include "../LogToArctan.h"


//implementing the exmple from the brosntien book pg75

int main()
{
  poly *a , *b;
  frac *c, *d, *e, *f;
  mpz_t q,y,z,g;


  a=initialize_p(3);
  b=initialize_p(2);

  mpz_init_set_ui(q,1);
  mpz_init_set_ui(g,1);
  mpz_init_set_ui(y,3);
  mpz_init_set_ui(z,2);

  mpz_neg(z,z);
  mpz_neg(y,y);

  a->coefficients[0]=init_f(q,g);
  b->coefficients[0]=init_f(q,g);

  a->coefficients[2]=init_f(y,q);
  b->coefficients[2]=init_f(z,q);

  
  print_p(a);
  print_p(b);

  print_arc(log_to_arc(a,b));
}
