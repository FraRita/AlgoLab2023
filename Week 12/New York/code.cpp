#include <iostream>
#include <vector>
#include <set>

int m,k;
std::vector<int> temp,path;
std::vector<std::vector<int>> edges;
std::vector<bool> res;

void dfs(int curr, std::multiset<int> &set){
  if(path.size() < m-1){
    set.insert(temp[curr]);
    path.push_back(curr);
    for(int v : edges[curr]) dfs(v,set);
    path.pop_back();
    set.erase(set.find(temp[curr]));
    return;
  }
  set.insert(temp[curr]);
  path.push_back(curr);
  int first = path[path.size()-m];
  if(*set.rbegin()-*set.begin() <= k) res[first] = true;
  set.erase(set.find(temp[first]));
  for(int v : edges[curr]) dfs(v,set);
  set.insert(temp[first]);
  path.pop_back();
  set.erase(set.find(temp[curr]));
}

void resolve_case(){
  int n;
  std::cin>> n >> m >> k;
  
  temp = std::vector<int>(n);
  edges = std::vector<std::vector<int>>(n);
  path = std::vector<int>(0);
  res = std::vector<bool>(n,false);
  std::multiset<int> set;
  
  int u,v;
  for(int i=0;i<n;i++) std::cin >> temp[i];
  for(int i=1;i<n;i++){
    std::cin >> u >> v;
    edges[u].push_back(v);
  }
  
  dfs(0,set);
  
  bool flag = false;
  for(int i=0;i<n;i++){
    if(res[i]){
      std::cout << i << " ";
      flag = true;
    }
  }
  
  if(!flag) std::cout<< "Abort mission";
  std::cout << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}