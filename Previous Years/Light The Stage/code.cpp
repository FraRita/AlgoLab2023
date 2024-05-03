#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;

int m,n;
long h;
std::vector<std::pair<K::Point_2,int>> participants;
std::vector<K::Point_2> lamps;
std::vector<int> elim;

void binary_search(int start, int end){
  int mid = (start+end)/2;

  std::vector<K::Point_2> temp(mid+1);
  for(int i=0;i<=mid;i++) temp[i] = lamps[i];
  Triangulation t;
  t.insert(temp.begin(),temp.end());

  bool flag = false;
  for(int i=0;i<m;i++){
    if(elim[i] < mid) continue;
    if(CGAL::squared_distance(participants[i].first, t.nearest_vertex(participants[i].first) -> point()) < (h+participants[i].second)*(h+participants[i].second) ) elim[i] = mid;
    else flag = true;
  }

  if(start == end-1) return;
  
  if(flag) binary_search(mid,end);
  else binary_search(start,mid);
}

void resolve_case(){
  std::cin >> m >> n;

  participants = std::vector<std::pair<K::Point_2,int>>(m);
  int x,y;
  for(int i=0;i<m;i++){
    std::cin>> x >> y >> participants[i].second;
    participants[i].first = K::Point_2(x,y);
  }

  std::cin>> h;

  lamps = std::vector<K::Point_2>(n);
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    lamps[i] = K::Point_2(x,y);
  }

  elim = std::vector<int>(m,n-1);
  
  Triangulation t;
  t.insert(lamps.begin(),lamps.end());
  bool flag = false;
  for(int i=0;i<m;i++){
    if(CGAL::squared_distance(participants[i].first, t.nearest_vertex(participants[i].first) -> point()) >= (h+participants[i].second)*(h+participants[i].second) ){
      flag = true;
      elim[i] = n;
    }
  }
  if(!flag) binary_search(0,n);

  int max = *std::max_element(elim.begin(),elim.end());
  for(int i=0;i<m;i++) if(elim[i] == max) std::cout<< i << " ";
  std::cout<< std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}