#include <iostream>

void resolve_case(){
  int n;
  std::cin >> n;
  int sum = 0;
  
  while(n-- > 0){
    int a;
    std::cin >> a;
    sum += a;
  }
  
  std::cout << sum << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}