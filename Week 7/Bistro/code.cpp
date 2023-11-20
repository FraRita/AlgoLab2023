#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <iostream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Finite_faces_iterator Face_iterator;

void resolve_case(int n){
  std::vector<K::Point_2> rest;
  int x,y;
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    rest.push_back(K::Point_2(x,y));
  }
  
  Triangulation t;
  t.insert(rest.begin(),rest.end());
  
  int m;
  std::cin >> m;
  for(int i=0;i<m;i++){
    std::cin >> x >> y;
     K::Point_2 ver = t.nearest_vertex(K::Point_2(x,y)) -> point();
     std::cout << std::setprecision(0) << std::fixed <<CGAL::squared_distance(ver,K::Point_2(x,y)) << std::endl;
  }
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  while(n>0){
    resolve_case(n);
    std::cin >> n;
  }
}