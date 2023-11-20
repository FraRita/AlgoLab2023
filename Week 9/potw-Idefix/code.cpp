#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <utility>
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

int bones_count(std::vector<std::vector<K::FT>> &bones, std::vector<Edge> &edges, long max_l, int n){
  std::vector<int> sums(n,0);
  for(int i=0;i<n;i++){
    for(int j=0;j<bones[i].size();j++){
      if(bones[i][j] <= max_l) sums[i]++;
    }
  }
  
  uf = boost::disjoint_sets_with_storage<> (n);
  for (auto e = edges.begin(); e != edges.end(); e++) {
    if(std::get<2>(*e) > max_l) break;
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    if (c1 != c2) {
      uf.link(c1, c2);
      if(uf.find_set(std::get<0>(*e)) == c2) std::swap(c1,c2);
      sums[c1] += sums[c2];
      if (--n == 1) break;
    }
  }
  
  return *(std::max_element(sums.begin(),sums.end()));
}

long binary(std::vector<K::FT> &distances, int start, int end, std::vector<std::vector<K::FT>> &bones, std::vector<Edge> &edges, int n, int k){
  if(start == end-1){
    if(bones_count(bones,edges,distances[start],n) >= k) return distances[start];
    return -1;
  }
  int mid = (start+end)/2;
  if(bones_count(bones,edges,distances[mid],n) >= k){
    long ret = binary(distances,start,mid,bones,edges,n,k);
    if(ret == -1) return distances[mid];
    return ret;
  }
  return binary(distances,mid,end,bones,edges,n,k);
}

void resolve_case(){
  int n,m,k;
  long s;
  std::cin >> n >> m >> s >> k;
  
  std::vector<IPoint> trees;
  std::vector<std::vector<K::FT>> bones(n,std::vector<K::FT>(0));
  std::vector<K::FT> distances(0);
  int x,y;
  for(Index i=0;i<n;i++){
    std::cin>> x >> y;
    trees.push_back({K::Point_2(x,y),i});
  }
  
  Triangulation t;
  t.insert(trees.begin(),trees.end());
  
  K::Point_2 point;
  K::FT dist;
  for(int i=0;i<m;i++){
    std::cin >> x >> y;
    point = K::Point_2(x,y);
    Triangulation::Vertex_handle vh = t.nearest_vertex(point);
    dist = 4 * CGAL::squared_distance(vh->point(),point);
    bones[vh -> info()].push_back(dist);
    distances.push_back(dist);
  }
  
  std::vector<Edge> edges;
  for(auto e=t.finite_edges_begin();e!=t.finite_edges_end();e++){
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    if (i1 > i2) std::swap(i1, i2);
    edges.push_back({i1, i2, t.segment(e).squared_length()});
    distances.push_back(t.segment(e).squared_length());
  }
  
  std::sort(edges.begin(),edges.end(),[](Edge &e1,Edge &e2){return (std::get<2>(e1)<std::get<2>(e2));});
  std::sort(distances.begin(),distances.end(),[](K::FT e1, K::FT e2){return (e1<e2);});
  
  std::cout<< bones_count(bones,edges,s,n) << " " << binary(distances,0,distances.size(),bones,edges,n,k) << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while(t-- > 0){
    resolve_case();
  }
}