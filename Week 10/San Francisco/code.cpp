#include <iostream>
#include <vector>
#include <utility>

std::vector<std::vector<std::pair<int,int>>> edges;
std::vector<std::vector<long>> dp;

void resolve_case(){
  int n,m,k;
  long x;
  std::cin >> n >> m >> x >> k;
  
  int u,v,p;
  edges = std::vector<std::vector<std::pair<int,int>>> (n);
  dp = std::vector<std::vector<long>>(k+1,std::vector<long>(n,-1));
  for(int i=0;i<m;i++){
    std::cin >> u >> v >>p;
    edges[u].push_back({v,p});
    dp[1][u] = std::max(dp[1][u],(long)p);
  }
  
  for(int i=0;i<n;i++){
    for(int j=0;j<edges[i].size();j++){
      if(dp[1][edges[i][j].first] == -1) edges[i][j].first = 0;
    }
  }
  
  for(int h=2;h<=k;h++){
    for(int i=0;i<n;i++){
      for(int j=0;j<edges[i].size();j++){
        dp[h][i] = std::max(dp[h][i],edges[i][j].second+dp[h-1][edges[i][j].first]);
      }
    }
  }
  
  int res = 1;
  while(res <= k && dp[res][0] < x) res++;
  if(res > k) std::cout << "Impossible" << std::endl;
  else std::cout << res << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}