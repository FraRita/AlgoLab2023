#include <iostream>
#include <vector>

void resolve_case(){
  int n,k;
  int new_n = -1;
  std::cin >> n >> k;
  std::vector<int> sums(1,0);
  for(int i=1;i<=n;i++){
    int a;
    std::cin >> a;
    if(a==k){
      new_n = i-1;
      i++;
      while(i<=n){
        std::cin >> a;
        i++;
      }
      break;
    }
    sums.push_back(sums.at(i-1) + a);
  }
  
  if(new_n != -1) n=new_n;
  int best_i,best_j;
  int curr_i=1,curr_j=1;
  int current_diff = k;
  
  for(curr_i=1;curr_i<=n;curr_i++){
    
    if(k-sums.at(curr_j)+sums.at(curr_i-1) >= 0){
      while(curr_j<=n && k-sums.at(curr_j)+sums.at(curr_i-1) > 0) curr_j++;
    
    }
    else{
       while(curr_j>curr_i && k-sums.at(curr_j)+sums.at(curr_i-1) < 0) curr_j--;
       curr_j++;
    }
    
    if(curr_j>n){
        curr_j--;
        if(std::abs(k-sums.at(curr_j)+sums.at(curr_i-1)) < current_diff){
          best_i = curr_i;
          best_j = curr_j;
        }
        break;
      }
    
    if(k-sums.at(curr_j)+sums.at(curr_i-1) == 0){
        std::cout << curr_i-1 << " " << curr_j-1 << std::endl;
        return;
    }
    
    if(std::abs(k-sums.at(curr_j)+sums.at(curr_i-1)) >= k-sums.at(curr_j-1)+sums.at(curr_i-1)){
      if(k-sums.at(curr_j-1)+sums.at(curr_i-1) < current_diff){
        best_i = curr_i;
        best_j = curr_j-1;
        current_diff = k-sums.at(curr_j-1)+sums.at(curr_i-1);
      }
    }
    else{
      if(std::abs(k-sums.at(curr_j)+sums.at(curr_i-1)) < current_diff){
        best_i = curr_i;
        best_j = curr_j;
        current_diff = std::abs(k-sums.at(curr_j)+sums.at(curr_i-1));
      }
    }
    
  }
  if(new_n == -1) std::cout << best_i-1 << " " << best_j-1 << std::endl;
  else std::cout << n << " " << n << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}