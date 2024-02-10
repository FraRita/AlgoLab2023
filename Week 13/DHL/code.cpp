#include <iostream>
#include <vector>

void resolve_case(){
  int n;
  std::cin>> n;
  
  std::vector<int> a(n),b(n);
  for(int i=n-1;i>=0;i--){
    std::cin>> a[i];
    a[i]--;
  }
  for(int i=n-1;i>=0;i--){
    std::cin>> b[i];
    b[i]--;
  }
  
  std::vector<std::vector<int>> dp(n,std::vector<int> (n,0));
  
  for(int j=1;j<n;j++) dp[0][j] = dp[0][j-1] + a[0] * b[j-1];
  for(int i=1;i<n;i++) dp[i][0] = dp[i-1][0] + a[i-1] * b[0];
  
  for(int i=1;i<n;i++){
    for(int j=1;j<n;j++){
      dp[i][j] = std::min(std::min(dp[i-1][j]+a[i-1]*b[j],dp[i][j-1]+a[i]*b[j-1]),dp[i-1][j-1]+a[i-1]*b[j-1]);
    }
  }
  
  std::cout << dp[n-1][n-1] + a[n-1]*b[n-1]<< std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}