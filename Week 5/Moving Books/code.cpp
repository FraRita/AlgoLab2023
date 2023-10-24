#include <iostream>
#include <vector>
#include <algorithm>


void resolve_case(){
  int n,m;
  std::cin >> n >> m;
  std::vector<int> f(n),b(m);
  
  for(int i=0;i<n;i++) std::cin>> f[i];
  for(int i=0;i<m;i++) std::cin>> b[i];
  
  std::sort(f.begin(),f.end(),[](const int i, const int j){return (i>j);});
  std::sort(b.begin(),b.end(),[](const int i, const int j){return (i>j);});
  if(b[0] > f[0]) {
    std::cout << "impossible" << std::endl;
    return;
  }
  
  int max = 0,curr_max = 0,rest = 0,first = 0,last = 0;
  std::vector<int> memo(m,-1);
  memo[0] = 0;
  
  for(int i=0;i<m;i++){
    while(last < n && b[i] <= f[last]) last ++;
    rest = 0;
    int interval = last-first;
    curr_max=0;
    do{
      if(curr_max == max){
        if(memo[max] == 0){
          memo[max] = -1;
          curr_max++;
          max++;
          memo[max] = 0;
        }
        memo[max] = (memo[max]+1)%last;
      }
      else{
        if(rest==0) {
          memo[curr_max] = -1;
          curr_max++;
          if(memo[curr_max] != -1){
            first = memo[curr_max];
            interval = last - first;
          }
        }
        rest = (rest+1)%interval;
        memo[curr_max] = first + rest;
      }
      i++;
    }while(i<m && (last == n || b[i] > f[last]) );
    i--;
    first = last;
  }
  
  std::cout << 3*max-1 << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}