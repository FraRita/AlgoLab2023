#include <vector>
#include <iostream>
#include <climits>

std::vector<int> costs;
std::vector<std::vector<int>> edges;
std::vector <long> not_me,only_me,also_me;

void compute_node(int me){
  if(edges[me].size() == 0){
    only_me[me] = -1;
    also_me[me] = costs[me];
    return;
  }
  long diff = LONG_MAX;
  int best_son;
  bool found = false;
  also_me[me] = costs[me];
  long son_cost;
  for(int son : edges[me]){
    compute_node(son);
    
    if(not_me[me] != -1){
      if(only_me[son] != -1) not_me[me] += only_me[son];
      else not_me[me] = -1;
    }
    
    son_cost = also_me[son];
    if(only_me[son] != -1) son_cost = std::min(son_cost,only_me[son]);
    if(not_me[son] != -1) son_cost = std::min(son_cost,not_me[son]);
    also_me[me] += son_cost;
    
    if(found) continue;
    if(only_me[son] == -1){
      best_son = son;
      found = true;
      continue;
    }
    if(also_me[son]-only_me[son] < diff){ 
      diff = also_me[son]-only_me[son];
      best_son = son;
    }
  }
  
  for(int son : edges[me]){
    if(son == best_son) only_me[me] += also_me[son];
    else{
      if(only_me[son] == -1) only_me[me] += also_me[son];
      else only_me[me] += std::min(also_me[son],only_me[son]);
    }
  }
}

void resolve_case(){
  int n;
  std::cin>> n;
  
  costs = std::vector<int> (n);
  edges = std::vector<std::vector<int>> (n);
  not_me = std::vector<long> (n,0);
  only_me = std::vector<long> (n,0);
  also_me = std::vector<long> (n,0);
  
  int s,t;
  for(int i=1;i<n;i++){
    std::cin>> s >> t;
    edges[s].push_back(t);
  }
  
  for(int i=0;i<n;i++){
    std::cin>> costs[i];
  }
  
  compute_node(0);
  
  std::cout << std::min(only_me[0],also_me[0]) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}