#include <iostream>

void resolve_case(){
  int n;
  std::cin >> n;
  int sum=0,even=1,odd=0;
  while(n-- > 0){
    int a;
    std::cin >> a;
    sum += a;
    if(sum%2 == 0) even++;
    else odd++;
  }
  
  std::cout << even*(even-1)/2 + odd*(odd-1)/2 << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}