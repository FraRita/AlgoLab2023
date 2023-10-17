#include <iostream>

void resolve_case(){
  int n;
  std::cin >> n;
  int fallen = 1;
  
  for(int i = 0; i<n; i++){
    int h;
    std::cin >> h;
    if(i+1 <= fallen){
      if(i+h > fallen) fallen = i+h;
    }
  }
  
  if(fallen >= n) fallen = n;
  
  std::cout << fallen  <<std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}