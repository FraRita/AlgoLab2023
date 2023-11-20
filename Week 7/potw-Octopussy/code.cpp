#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int n;

int de(std::vector<bool> &deact,int pos){
  if(deact[pos] == true) return 0;
  deact[pos] = true;
  if(pos >= (n-1)/2) return 1;
  return (1+de(deact,2*pos+1)+de(deact,2*pos+2));
}

void resolve_case(){
  std::cin>>n;
  std::vector<bool> deact(n,false);
  std::vector<std::pair<int,int>> bombs;
  int t;
  for(int i=0;i<n;i++){
    std::cin>>t;
    bombs.push_back({t,i});
  }
  
  std::sort(bombs.begin(),bombs.end(),[](std::pair<int,int> a, std::pair<int,int> b){return (a.first<b.first);});
  
  t=0;
  for(int i=0;i<n;i++){
    t += de(deact,bombs[i].second);
    if(t > bombs[i].first){
      std::cout << "no" << std::endl;
      return;
    }
  }
  
  std::cout << "yes" << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}