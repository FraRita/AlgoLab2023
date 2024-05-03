#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

void resolve_case(){
  int n,q;
  std::cin>> n >> q;

  
  std::unordered_map<std::string,int> to_n;
  std::vector<std::string> to_s;
  std::vector<int> age;
   std::string s;
  int a;
  for(int i=0;i<n;i++){
    std::cin>> s >> a;
    to_n[s] = i;
    to_s.push_back(s);
    age.push_back(a);
  }

  std::vector<std::vector<int>> parent(1,std::vector<int>(n,-1));
  std::string from,to;
  for(int i=0;i<n-1;i++){
    std::cin>> to >> from;
    parent[0][to_n[to]] = to_n[from];
  }

  int i=1;
  bool flag = true;
  while(flag){
    parent.push_back(std::vector<int>(n,-1));
    flag = false;
    for(int j=0;j<n;j++){
      if(parent[i-1][j] == -1 || parent[i-1][parent[i-1][j]] == -1) continue;
      flag = true;
      parent[i][j] = parent[i-1][parent[i-1][j]];
    }
    i++;
  }

  for(i=0;i<q;i++){
    std::cin>> s >> a;
    int prec = to_n[s], next = prec, j = 1;
    if(parent[0][next] == -1 || age[parent[0][next]] > a){
      std::cout<< s << " ";
      continue;
    }
    while(true){
      if(parent[j][next] == -1 || age[parent[j][next]] > a){
        prec = next;
        next = parent[j-1][next];
        if(parent[0][next] == -1 || age[parent[0][next]] > a) {
          next = prec;
          break;
        }
        j=0;
      }
      j++;
    }
    std::cout<< to_s[parent[j-1][next]] << " ";
  }
  std::cout<<std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}