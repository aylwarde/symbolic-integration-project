#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int gcd();
int lcm();

int gcd(int a, int b) {
  
  int r = 0;
  int q = 0;
  
  if(a==b) {
    
    return a;
    
  } else {
    
    if(a==0 || b==0) {
      
      return MAX(a,b);
      
    } else {
      
      return gcd(MIN(a,b),MAX(a,b)%MIN(a,b));
      
    }
  }
  return -1;
}

int lcm(int a, int b) {

  return (a*b)/gcd(a,b);

}
