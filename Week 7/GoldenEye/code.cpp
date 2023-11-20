#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef std::tuple<Index,Index,K::FT>                          Edge;
typedef std::pair<K::Point_2,Index>                            IPoint;

boost::disjoint_sets_with_storage<> uf;

void create_union_find(std::vector<Edge> &edges, K::FT max_dist, int n_components){
  uf = boost::disjoint_sets_with_storage<> (n_components);
  for (auto e = edges.begin(); e != edges.end(); e++) {
    if(std::get<2>(*e) > max_dist) break;
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
      if (--n_components == 1) break;
    }
  }
}

K::FT binary_all(std::vector<Edge> &missions, std::vector<Edge> &edges, int start, int end, int n){
  if(start == end-1){
    create_union_find(edges,std::get<2>(edges[start]),n);
    for(Edge mission : missions){
      if( uf.find_set(std::get<0>(mission)) != uf.find_set(std::get<1>(mission)) ||  std::get<2>(mission) > std::get<2>(edges[start]) ) return -1;
    }
    return std::get<2>(edges[start]);
  }
  int mid = (end+start)/2;
  bool flag = true;
  create_union_find(edges,std::get<2>(edges[mid]),n);
  for(Edge mission : missions){
    if( uf.find_set(std::get<0>(mission)) != uf.find_set(std::get<1>(mission)) ||  std::get<2>(mission) > std::get<2>(edges[mid]) ){
      flag = false;
      break;
    }
  }
  if(flag){
    K::FT next = binary_all(missions,edges,start,mid,n);
    if(next == -1) return std::get<2>(edges[mid]);
    return next;
  }
  return binary_all(missions,edges,mid,end,n);
}

K::FT binary_same(std::vector<Edge> &missions, std::vector<bool> &doable, std::vector<Edge> &edges, int start, int end, int n){
  if(start == end-1){
    create_union_find(edges,std::get<2>(edges[start]),n);
    for(int i=0;i<missions.size();i++){
      if( ( uf.find_set(std::get<0>(missions[i])) == uf.find_set(std::get<1>(missions[i])) && std::get<2>(missions[i]) <= std::get<2>(edges[start]) ) != doable[i] ) return -1;
    }
    return std::get<2>(edges[start]);
  }
  int mid = (end+start)/2;
  bool flag = true, value, go_up;
  create_union_find(edges,std::get<2>(edges[mid]),n);
  for(int i=0;i<missions.size();i++){
    value = (uf.find_set(std::get<0>(missions[i])) == uf.find_set(std::get<1>(missions[i])) && std::get<2>(missions[i]) <= std::get<2>(edges[mid]));
    if( value != doable[i] ){
      flag = false;
      if(value) go_up = false;
      else go_up = true;
      break;
    }
  }
  if(flag){
    K::FT next = binary_same(missions,doable,edges,start,mid,n);
    if(next == -1) return std::get<2>(edges[mid]);
    return next;
  }
  if(go_up) return binary_same(missions,doable,edges,mid,end,n);
  return binary_same(missions,doable,edges,start,mid,n);
}

void resolve_case(){
  int n,m;
  double p;
  std::cin >> n >> m >> p;
  
  std::vector<IPoint> jammers;
  int x,y;
  for(Index i=0;i<n;i++){
    std::cin>> x >> y;
    jammers.push_back({K::Point_2(x,y),i});
  }
  
  Triangulation t;
  t.insert(jammers.begin(),jammers.end());
  
  std::vector<Edge> missions;
  K::Point_2 point;
  Triangulation::Vertex_handle vh1,vh2;
  K::FT dist1,dist2;
  for(int i=0;i<m;i++){
    std::cin>> x >> y;
    point = K::Point_2(x,y);
    vh1 = t.nearest_vertex(point);
    dist1 = CGAL::squared_distance(point,vh1 -> point());
    std::cin>> x >> y;
    point = K::Point_2(x,y);
    vh2 = t.nearest_vertex(point);
    dist2 = CGAL::squared_distance(point,vh2 -> point());
    missions.push_back({vh1->info(),vh2->info(),CGAL::max(dist1,dist2)*4});
  }
  
  std::vector<Edge> edges;
  Index i1,i2;
  for(auto e=t.finite_edges_begin();e!=t.finite_edges_end();e++){
    i1 = e->first->vertex((e->second+1)%3)->info();
    i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.push_back({i1, i2, t.segment(e).squared_length()});
  }
  std::sort(edges.begin(),edges.end(),[](Edge &e1,Edge &e2){return (std::get<2>(e1)<std::get<2>(e2));});
  
  create_union_find(edges,p,n);
  std::vector<bool> doable(m,false);
  K::FT max_y = -1,max_n = -1;
  for(int i=0;i<m;i++){
    if( uf.find_set(std::get<0>(missions[i])) == uf.find_set(std::get<1>(missions[i])) && std::get<2>(missions[i]) <= p ){
      doable[i] = true;
      std::cout << "y";
      max_y = CGAL::max(max_y,std::get<2>(missions[i]));
    }
    else{
      std::cout << "n";
    }
    max_n = CGAL::max(max_n,std::get<2>(missions[i]));
  }
  std::cout<<std::endl;
  
  K::FT all = binary_all(missions,edges,0,edges.size(),n);
  if(all == -1) std::cout<< std::setprecision(0) << std::fixed << max_n << std::endl;
  else{
    create_union_find(edges,max_n,n);
    bool flag = true;
    for(int i=0;i<missions.size();i++){
      if(uf.find_set(std::get<0>(missions[i])) != uf.find_set(std::get<1>(missions[i])) || std::get<2>(missions[i]) > max_n){
        std::cout << std::setprecision(0) << std::fixed << all << std::endl;
        flag = false;
        break;
      }
    }
    if(flag) std::cout<< std::setprecision(0) << std::fixed << max_n << std::endl;
  }
  
  if(max_y == -1){
    std::cout << 0 << std::endl;
    return;
  } 
  K::FT same = binary_same(missions,doable,edges,0,edges.size(),n);
  create_union_find(edges,max_y,n);
  for(int i=0;i<missions.size();i++){
    if( ( uf.find_set(std::get<0>(missions[i])) == uf.find_set(std::get<1>(missions[i])) && std::get<2>(missions[i]) <= max_y ) != doable[i] ){
      std::cout << std::setprecision(0) << std::fixed << same << std::endl;
      return;
    }
  }
  std::cout<< std::setprecision(0) << std::fixed << max_y << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while(t-- > 0){
    resolve_case();
  }
}