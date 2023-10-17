#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int m,n;
K::Point_2 points[2000];
bool maps[2000][2000];
int count[1999];
K::Point_2 tri[6];

bool is_in(K::Point_2 point){
  for(int i=0;i<6;i+=2){
    auto point_or = CGAL::orientation(tri[i],tri[i+1],point);
    if(point_or == CGAL::COLLINEAR) continue;
    if( point_or != CGAL::orientation(tri[i],tri[i+1],tri[(i+2)%6]) ) return false;
  }
  return true;
}

void resolve_case(){
  int n,m;
  std::cin >> m >> n;
  for(int i=0;i<n;i++) memset(maps[i],false,m*sizeof(bool));
  memset(count,0,(m-1)*sizeof(int));
  
  int a,b;
  for(int i=0;i<m;i++){
    std::cin >> a >> b;
    points[i] = K::Point_2(a,b);
  }
  
  for(int i=0;i<n;i++){
    for(int j=0;j<6;j++){
      std::cin >> a >> b;
      tri[j] = K::Point_2(a,b);
    }
    for(int j=0;j<m;j++){
      if(is_in(points[j])) maps[i][j] = true;
    }
  }
  
  int sli=0;
  int min = n;
  bool all = false;
  int i=0;
  while(i<n){
    while(sli<n && !all){
      all=true;
      for(int j=0;j<m-1;j++){
        if(maps[sli][j] && maps[sli][j+1]) count[j]++;
        if(count[j] == 0) all = false;
      }
      sli++;
    }
    
    if(sli == n && !all) break;
    
    while(i<n && all){
      for(int j=0;j<m-1;j++){
        if(maps[i][j] && maps[i][j+1]) count[j]--;
        if(count[j] == 0) all = false;
      }
      i++;
    }
    if(!all && i<n && sli-i+1 < min) min = sli-i+1;
    
    
    if(sli == n)break;
    
  }
  
  std::cout << min << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  while(n-- >0){
    resolve_case();
  }
}