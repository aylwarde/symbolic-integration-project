#include "../Rintergration.h"


int main()
{
  poly *polya ,*polyb , Q;
  rational g,*temp;
  Logs Rh;
  polya = initialize_p(3);
  assign_coeffs_p(polya);

  polyb = initialize_p(3);
  assign_coeffs_p(polyb);

  temp = init_r(polya,polyb);

  intergrate_r(temp, &g ,&Q ,&Rh);
  return 0;
  
}

  
