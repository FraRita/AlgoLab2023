#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>  Vb;
typedef CGAL::Triangulation_face_base_2<K>                  Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>         Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>               T;

void resolve_case(){
  int n;
  long r;
  std::cin>> n >> r;
  r = r * r;
  
  std::vector<std::pair<K::Point_2,int>> planets(n);
  int x,y;
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    planets[i] = {K::Point_2(x,y),i};
  }
  
  int start =0, end =n;
  int k;
  while(true){
    k = (start+end)/2;
    
    T t;
    t.insert(planets.begin()+k,planets.end());
    
    boost::disjoint_sets_with_storage<> uf(n);
    std::vector<int> cc(n,1);
    
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      if(t.segment(e).squared_length() > r) continue;
      int c1 = uf.find_set(e->first->vertex((e->second+1)%3)->info());
      int c2 = uf.find_set(e->first->vertex((e->second+2)%3)->info());
      if (c1 != c2) {
        uf.link(c1, c2);
        cc[uf.find_set(e->first->vertex((e->second+1)%3)->info())] = cc[c1] + cc[c2];
      }
    }
    
    int all = *std::max_element(cc.begin(),cc.end());
    if(all == k) break;
    if(start == end - 1){
      k = std::min(k,all);
      break;
    }
    if(all < k) end = k;
    else start = k;
  }
   std::cout << k << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}