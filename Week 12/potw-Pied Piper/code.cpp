#include <iostream>
#include <vector>

void resolve_case(){
  int n,m;
  std::cin>> n >> m;

  std::vector<std::vector<std::pair<int,int>>> edges(n), r_edges(n);
  int from,to,r;
  for(int i=0;i<m;i++){
    std::cin>> from >> to >> r;
    if(from > to) r_edges[to].push_back({from,r});
    else edges[from].push_back({to,r});
  }
  
  std::vector<std::vector<int>> dp(n,std::vector<int>(n,-1));
  for(auto e : edges[0]) dp[e.first][0] = std::max(dp[e.first][0],e.second);
  for(auto e : r_edges[0]) dp[0][e.first] = std::max(dp[0][e.first],e.second);
  
  for(int k=1;k<2*n-2;k++){
    for(int i=0;i<=std::min(n-1,k);i++){
      int j=k-i;
      if(j > n-1 || i==j || dp[i][j] == -1) continue;
      if(edges[i].size() > 0) for(auto e : edges[i]) if(e.first >= j) dp[e.first][j] = std::max(dp[e.first][j],dp[i][j]+e.second);
      if(r_edges[j].size() > 0) for(auto e : r_edges[j]) if(e.first >= i) dp[i][e.first] = std::max(dp[i][e.first],dp[i][j]+e.second);
    }
  }
  
  std::cout<< dp[n-1][n-1] << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}