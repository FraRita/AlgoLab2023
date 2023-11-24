#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <utility>
#include <queue>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef std::pair<K::Point_2,Index>                            IPoint;

void resolve_case(){
  int n,m;
  long r;
  std::cin >> n >> m >> r;
  
  std::vector<IPoint> stations;
  int x,y;
  for(Index i=0;i<n;i++){
    std::cin>> x >> y;
    stations.push_back({K::Point_2(x,y),i});
  }
  
  Triangulation t;
  t.insert(stations.begin(),stations.end());
  
  boost::disjoint_sets_with_storage<> uf(n);
  std::vector<std::vector<int>> edges(n);
  for(auto e=t.finite_edges_begin();e!=t.finite_edges_end();e++){
    if(t.segment(e).squared_length() <= r*r){
      Index i1 = e->first->vertex((e->second+1)%3)->info();
      Index i2 = e->first->vertex((e->second+2)%3)->info();
      edges[i1].push_back(i2);
      edges[i2].push_back(i1);
      Index c1 = uf.find_set(i1);
      Index c2 = uf.find_set(i2);
      if(c1 != c2) uf.link(c1,c2);
    }
  }
  
  bool network = true;
  for(int i=0;i<n;i++){
    for(int v : edges[i]){
      for(int p : edges[v]){
        if(p != i && CGAL::squared_distance(stations[i].first,stations[p].first) <= r*r){
          network = false;
          break;
        }
      }
      if(!network) break;
    }
    if(!network) break;
  }
  
  
  
  std::vector<int> f(n, -1);
  std::queue<int> Q;
  for(Index i=0;i<n;i++){
    if(f[i] == -1){
      Q.push(i);
      f[i] = 0;
      while (!Q.empty()) {
        const int u = Q.front();
        Q.pop();
        for (int v : edges[u]) {
          if (f[v] == f[u]){
            network = false;
            break;
          }
          if(f[v] == -1){
            f[v] = (f[u]+1)%2;
            Q.push(v);
          }
        }
        if(!network) break;
      }
    }
    if(!network) break;
  }
  
  if(!network){
    for(int i=0;i<m;i++){
      std::cin>> x >> y;
      std::cin>> x >> y;
      std::cout<< "n";
    }
  }
  else{
    int x1,y1;
    K::Point_2 p1,p2;
    Triangulation::Vertex_handle v1,v2;
    for(int i=0;i<m;i++){
      std::cin>> x >> y >> x1 >> y1;
      p1 = K::Point_2(x,y);
      p2 = K::Point_2(x1,y1);
      if(CGAL::squared_distance(p1,p2) <= r*r){
        std::cout<< "y";
        continue;
      }
      v1 = t.nearest_vertex(p1);
      v2 = t.nearest_vertex(p2);
      if(CGAL::squared_distance(v1->point(),p1) <= r*r && CGAL::squared_distance(v2->point(),p2) <= r*r && uf.find_set(v1->info()) == uf.find_set(v2->info())) std::cout<< "y";
      else std::cout<< "n";
    }
  }
  std::cout<<std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while(t-- > 0){
    resolve_case();
  }
}