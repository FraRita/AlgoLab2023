#include <iostream>
#include <vector>

std::vector<std::pair<int,int>> intervals;
std::vector<int> first_intersec;

void do_best(std::vector<std::vector<int>> &state,int m,int i){
  if(state.at(m-1).at(i) != -1) return;
  if(m == 1){
    if(i == 0) state.at(m-1).at(i) = intervals.at(i).second-intervals.at(i).first+1;
    else{
      do_best(state,m,i-1);
      state.at(m-1).at(i) = std::max(intervals.at(i).second-intervals.at(i).first+1,state.at(m-1).at(i-1));
    } 
    return;
  }
  
  if(i == 0 || first_intersec.at(i) == 0){
    state.at(m-1).at(i) = -2;
    return;
  }
  int prec = i-1;
  if(first_intersec.at(i) != -1) prec = first_intersec.at(i)-1;
  
  do_best(state,m,i-1);
  do_best(state,m-1,prec);
  
  if(state.at(m-1).at(i-1) == -2 && state.at(m-2).at(prec) == -2){
    state.at(m-1).at(i) = -2;
    return;
  }
  
  if(state.at(m-2).at(prec) == -2){
    state.at(m-1).at(i) = state.at(m-1).at(i-1);
    return;
  }
  
  state.at(m-1).at(i) = std::max(intervals.at(i).second-intervals.at(i).first+1 + state.at(m-2).at(prec),state.at(m-1).at(i-1));
}

void resolve_case(){
  intervals.clear();
  first_intersec.clear();
  int n,m,k;
  std::cin >> n >> m >> k;
  std::vector<long> hor(1,0);
  for(int i=0;i<n;i++){
    int a;
    std::cin>> a;
    hor.push_back(hor.at(i)+a);
  }
  
  int j=1,p=0;
  for(int i=1;i<=n;i++){
    while(j<=n && hor.at(j)-hor.at(i - 1) < k) j++;
    if(j>n) break;
    if(hor.at(j)-hor.at(i - 1) == k){
      intervals.push_back(std::make_pair(i,j));
      while(p<intervals.size() - 1 && intervals.at(p).second < i) p++;
      if(p==intervals.size() - 1) first_intersec.push_back(-1);
      else{
        first_intersec.push_back(p);
      }
    } 
    else j--;
    if(j<=i) j = i + 1;
  }
  
  if(intervals.size() < m){
    std::cout << "fail" << std::endl;
    return;
  } 
  std::vector<std::vector<int>> state(m,std::vector<int>(intervals.size(),-1));
  
  do_best(state,m,intervals.size()-1);

  
  if(state.at(m-1).at(intervals.size()-1) != -2) std::cout<< state.at(m-1).at(intervals.size()-1) << std::endl;
  else std::cout<< "fail" << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}