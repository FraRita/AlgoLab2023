#include <iostream>
#include <vector>

void do_worst(std::vector<std::vector<int>> &transitions, std::vector<int> &best, std::vector<int> &worst, int start);

void do_best(std::vector<std::vector<int>> &transitions,std::vector<int> &best, std::vector<int> &worst, int start){
  if(best.at(start) != -1) return;
  for(int i=0; i<transitions.at(start).size();i++){
    int next = transitions.at(start).at(i);
    do_worst(transitions,best,worst,next);
    if(best.at(start) == -1 || worst.at(next) + 1 < best.at(start)){
      best.at(start) = worst.at(next) + 1;
    }
  }
}

void do_worst(std::vector<std::vector<int>> &transitions, std::vector<int> &best, std::vector<int> &worst, int start){
  if(worst.at(start) != -1) return;
  for(int i=0; i<transitions.at(start).size();i++){
    int next = transitions.at(start).at(i);
    do_best(transitions,best,worst,next);
    if(worst.at(start) == -1 || best.at(next) + 1 > worst.at(start)){
      worst.at(start) = best.at(next) + 1;
    }
  }
}


void resolve_case(){
  int n,m,r,b;
  std::cin >> n >> m >> r >> b;
  r--;
  b--;
  std::vector<std::vector<int>> transitions(n);
  while(m-- > 0){
    int start,end;
    std::cin >> start >> end;
    transitions.at(start-1).push_back(end-1);
  }
  std::vector<int> best(n,-1),worst(n,-1);
  best.at(n-1) = 0;
  worst.at(n-1) = 0;
  
  do_best(transitions,best,worst,r);
  do_best(transitions,best,worst,b);
  
  if(best.at(r)<best.at(b) || (best.at(r)==best.at(b) && best.at(r)%2 == 1)) std::cout<< 0 << std::endl;
  else std::cout<< 1 << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}