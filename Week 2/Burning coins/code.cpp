#include <iostream>
#include <vector>

void compute_max(std::vector<std::vector<int>> &max, std::vector<std::vector<int>> &next, std::vector<int> &coins, int i, int j){
  if(max.at(i).at(j) != -1) return;
  if(i == j-1){
    if(coins.at(i) > coins.at(j)){
      max.at(i).at(j) = coins.at(i);
      next.at(i).at(j) = 0;
    }
    else{
      max.at(i).at(j) = coins.at(j);
      next.at(i).at(j) = 1;
    }
    return;
  }
  compute_max(max,next,coins,i+1,j);
  compute_max(max,next,coins,i,j-1);
  int case_i = 0,case_j = 0;
  if(next.at(i+1).at(j) == 0 ) case_i = max.at(i+2).at(j);
  else case_i = max.at(i+1).at(j-1);
  if(next.at(i).at(j-1) == 0 ) case_j = max.at(i+1).at(j-1);
  else case_j = max.at(i).at(j-2);
  
  if(coins.at(i)+case_i > coins.at(j)+case_j){
    max.at(i).at(j) = coins.at(i)+case_i;
    next.at(i).at(j) = 0;
  }
  else{
    max.at(i).at(j) = coins.at(j)+case_j;
    next.at(i).at(j) = 1;
  }
}

void resolve_case(){
  int n;
  std::cin >> n;
  std::vector<int> coins;
  std::vector<std::vector<int>> max(n,std::vector<int>(n,-1));
  std::vector<std::vector<int>> next(n,std::vector<int>(n,-1));
  for(int i = 0;i < n; i++){
    int a;
    std::cin >> a;
    coins.push_back(a);
    max.at(i).at(i) = a;
    next.at(i).at(i) = 0;
  }
  
  compute_max(max,next,coins,0,n-1);
  std::cout << max.at(0).at(n-1) << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}