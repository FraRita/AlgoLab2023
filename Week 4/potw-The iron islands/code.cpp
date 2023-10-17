#include <iostream>
#include <vector>
#include <utility>

void resolve_case(){
  int n,k,w;
  std::cin>> n >> k >> w;
  std::vector<int> weights;
  std::vector<std::pair<int,int>> memo_ww(k,{-1,-1});
  std::vector<std::pair<int,int>> memo_dist(k,{-1,-1});
  int max = -1;
  for(int i=0;i<n;i++){
    int a;
    std::cin>> a;
    weights.push_back(a);
  }
  
  std::vector<std::vector<int>> ww(w,std::vector<int> {0,weights.at(0)});
  if(weights.at(0) == k) max = 1;
  
  for(int i=0;i<w;i++){
    int isl,slide;
    std::cin>> isl >> slide;
    slide = 1;
    for(int j=1;j<isl;j++){
      int new_isl;
      std::cin>> new_isl;
      ww.at(i).push_back(ww.at(i).at(j)+weights.at(new_isl));
      
      while(ww.at(i).at(j+1)-ww.at(i).at(slide-1) > k && slide < j+1) slide++;
      if(ww.at(i).at(j+1)-ww.at(i).at(slide-1) == k && j-slide+2 > max) max = j-slide+2;
      
      int value = ww.at(i).at(j+1)-weights.at(0);
      if(value < k){
        if(memo_ww.at(value).first == -1 ){
          memo_ww.at(value).first = i;
          memo_dist.at(value).first = j;
        }else{
          if(memo_dist.at(value).first < j){
            memo_ww.at(value).second = memo_ww.at(value).first;
            memo_dist.at(value).second = memo_dist.at(value).first;
            memo_ww.at(value).first = i;
            memo_dist.at(value).first = j;
          }
          else{
            if(memo_ww.at(value).second == -1 || memo_dist.at(value).second < j){
              memo_ww.at(value).second = i;
              memo_dist.at(value).second = j;
            }
          }
        }
      }
      
      value = k-ww.at(i).at(j+1);
      if(value > 0 && memo_ww.at(value).first != -1){
        if(memo_ww.at(value).first != i) {
          if(memo_dist.at(value).first+j+1 > max) max = memo_dist.at(value).first+j+1;
        }
        else{
          if(memo_ww.at(value).second != -1 && memo_dist.at(value).second+j+1 > max) max = memo_dist.at(value).second+j+1;
        }
      }
      
    }
  }
  
  if(max == -1) std::cout<< 0 << std::endl;
  else std::cout<< max << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}