#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>


void resolve_case(){
  int n;
  std::cin>> n;
  std::vector<std::pair<int,int>> boats;
  int l,p;
  for(int i=0;i<n;i++){
    std::cin>> l >> p;
    boats.push_back({l,p});
  }
  std::sort(boats.begin(),boats.end(),[](std::pair<int,int> a, std::pair<int,int> b){return (a.second<b.second);});
  
  int count = 1, i=1,start = boats[0].second,end,curr_end;
  while(i<n){
    end = std::max(start+boats[i].first,boats[i].second);
    i++;
    while(i<n && boats[i].second < end){
      curr_end = std::max(start+boats[i].first,boats[i].second);
      if(curr_end < end) end = curr_end;
      i++;
    }
    count++;
    start = end;
  }
  
  std::cout<< count << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}