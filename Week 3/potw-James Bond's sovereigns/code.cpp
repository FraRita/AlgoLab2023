#include <iostream>
#include <vector>
#include <cstring>

int n,m,k;
int coins[2000];
int memo[2000][2000];

int compute_max(int i, int j, int curr){
  if(memo[i][j] != -1) return memo[i][j];
  if(i==j){
    if(curr==k) memo[i][j] = coins[i];
    else memo[i][j] = 0;
    return memo[i][j];
  }
  
  int left = compute_max(i+1,j,(curr+1)%m);
  int right = compute_max(i,j-1,(curr+1)%m);
  
  if(curr == k){
    memo[i][j] = std::max(coins[i]+left,coins[j]+right);
  }
  else{
    memo[i][j] = std::min(left,right);
  }
  
  return memo[i][j];
}

void resolve_case(){
  std::cin >> n >> m >> k;
  for(int i=0;i<n;i++) memset(&memo[i][0],-1,n*sizeof(int));
  for(int i = 0;i < n; i++){
    std::cin >> coins[i];
  }
  
  std::cout << compute_max(0,n-1,0) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}