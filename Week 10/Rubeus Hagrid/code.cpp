#include <iostream>
#include <vector>
#include <utility>
#include <queue>

std::vector<std::vector<int>> edges;
std::vector<std::pair<long,long>> times;
std::vector<int> rooms;
std::vector<long> gold;

double maximize(int node){
  if(rooms[node] == 1){
    return (double)(rooms[node])/(2*times[node].first+times[node].second);
  }
  std::priority_queue<std::pair<double,int>> sons;
  for(int edge : edges[node]){
    sons.push({maximize(edge),edge});
  }
  
  rooms[node] = 1;
  int son;
  while(sons.size() > 0){
    son = sons.top().second;
    sons.pop();
    times[node].second += times[son].first;
    gold[node] += gold[son]-times[node].second*rooms[son];
    rooms[node] += rooms[son];
    times[node].second += times[son].second+times[son].first;
  }
  
  return (double)(rooms[node])/(2*times[node].first+times[node].second);
}

void resolve_case(){
  int n;
  std::cin>> n;
  
  edges = std::vector<std::vector<int>>(n+1);
  gold = std::vector<long>(n+1);
  times = std::vector<std::pair<long,long>> (n+1,{0,0});
  rooms = std::vector<int> (n+1,0);
  gold[0] = 0;
  for(int i=1;i<=n;i++) std::cin>> gold[i];
  
  int u,v;
  long l;
  for(int i=0;i<n;i++){
    std::cin>> u >> v >> l;
    edges[u].push_back(v);
    times[v].first = l;
  }
  
  
  for(int i=0;i<n;i++){
    if(edges[i].size() == 0){
      rooms[i] = 1;
    }
  }
  
  maximize(0);
  
  std::cout<< gold[0] << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}