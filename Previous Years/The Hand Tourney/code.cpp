#include <iostream>
#include <vector>
#include <tuple>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K>    Vb;
typedef CGAL::Triangulation_face_base_2<K>                    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>           Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                 Triangulation;
typedef Triangulation::Finite_edges_iterator                  EI;

int n,k;

int count_families(std::vector<std::tuple<int,int,K::FT>> &edges, K::FT s){
  std::vector<int> cluster(n,1);
  boost::disjoint_sets_with_storage<> uf(n);
  
  for(std::tuple<int,int,K::FT> &edge : edges){
    if(std::get<2>(edge) >= s) break;
    int c1 = uf.find_set(std::get<0>(edge));
    int c2 = uf.find_set(std::get<1>(edge));
    if(c1 != c2){
      uf.link(c1, c2);
      if(uf.find_set(std::get<0>(edge)) == c2) std::swap(c1,c2);
      cluster[c1] += cluster[c2];
      cluster[c2] = 0;
    }
  }
  std::sort(cluster.begin(),cluster.end(),[](int e1, int e2){return (e1>e2);});
  
  int count = 0;
  int i=0, j=n-1;
  while(cluster[j] == 0) j--;
  while(cluster[i] >= k){
    count++;
    i++;
  }
  
  int curr = cluster[i];
  while(i<j){
    curr += cluster[j];
    j--;
    if(curr >= k){
      count++;
      i++;
      curr = cluster[i];
    }
  }
  
  return count;
}

double binary(std::vector<std::tuple<int,int,K::FT>> &edges, int start, int end, int f0){
  if(start == end-1){
    if(count_families(edges,std::get<2>(edges[start])) >= f0) return std::get<2>(edges[start]);
    return -1;
  }
  int mid = (start+end)/2;
  if(count_families(edges,std::get<2>(edges[mid])) >= f0){
    double res = binary(edges,mid,end,f0);
    if(res == -1) return std::get<2>(edges[mid]);
    return res;
  }
  return binary(edges,start,mid,f0);
}

void resolve_case(){
  int f0;
  double s0;
  std::cin>> n >> k >> f0 >> s0;
  
  int x,y;
  std::vector<std::pair<K::Point_2,int>> tents;
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    tents.push_back({K::Point_2(x,y),i});
  }
  
  Triangulation t;
  t.insert(tents.begin(),tents.end());
  
  std::vector<std::tuple<int,int,K::FT>> edges;
  for (EI e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e)
    edges.push_back({e->first->vertex((e->second+1)%3)->info(), e->first->vertex((e->second+2)%3)->info(), t.segment(e).squared_length()});
  std::sort(edges.begin(), edges.end(),[](std::tuple<int,int,K::FT> &e1, std::tuple<int,int,K::FT> &e2){return (std::get<2>(e1)<std::get<2>(e2));});

  std::cout<< std::setprecision(0) << std::fixed << binary(edges,0,edges.size(),f0) << " " << count_families(edges,K::FT(s0)) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}