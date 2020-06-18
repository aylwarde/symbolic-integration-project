#include <stdio.h>
#include "../bivariate_poly.h"
#include "../bivariateResultant.h"

//trying to repliacte the books example pg 38 bronstien
int main()
{
  bpoly *polya, *polyb;
  poly *a,*b,*c,*d,*e;
  mpz_t one ,negfour ,negone,three,negnine;
  int strlen;
  polya= initialize_bp(2);
  polyb= initialize_bp(2);
  mpz_init_set_ui(one,1);
  mpz_init_set_ui(negfour,4);
  mpz_init_set_ui(three,3);
  mpz_init_set_ui(negnine,9);

  mpz_init(negone);
  mpz_neg(negone, one);
  mpz_neg(negfour,negfour);
  mpz_neg(negnine,negnine);

  a=initialize_p(3);
  a->coefficients[0]=init_f(negone,one);
  a->coefficients[3]=init_f(negfour,one);
  polya->pcoefficients[2]=a;

  b=initialize_p(1);
  b->coefficients[0]=init_f(three,one);
  polya->pcoefficients[0]=b;

  c=initialize_p(0);
  c->coefficients[0]=init_f(negnine,one);
  polyb->pcoefficients[2]=c;

  d=initialize_p(3);
  d->coefficients[0]=init_f(one,one);
  polyb->pcoefficients[1]=d;

  e=initialize_p(0);
  e->coefficients[0]=init_f(one,one);
  polyb->pcoefficients[0]=e;

  printf("this is poly a\n");
  print_bp(polya);

  printf("this is poly b\n");
    print_bp(polyb);

    printf("this is the subresultant of A&B\n");

    print_bp(bsubresultant(polya,polyb,&strlen)[0]);
    
  
    

    return 0;

}
