#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int gcd_z();
int lcm_z();
int pow_z();

int gcd_z(int a, int b) {
  
  int r = 0;
  int q = 0;
  
  if(a==b) {
    
    return a;
    
  } else {
    
    if(a==0 || b==0) {
      
      return MAX(a,b);
      
    } else {
      
      return gcd_z(MIN(a,b),MAX(a,b)%MIN(a,b));
      
    }
  }
  return -1;
}

int lcm_z(int a, int b) {

  return (a*b)/gcd_z(a,b);

}

int pow_z(int b, int exp) {

  if (exp == 0) {

    return 1;
    
  } else if ( exp % 2) {

    return b * pow_z(b, exp - 1);
    
  } else {

    int temp = pow_z(b, exp / 2);
    return temp * temp;
  }
}
