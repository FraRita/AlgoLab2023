#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>
#include <iostream>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef Triangulation::Vertex_handle Vertex_handle;

double round_down(const K::FT& x)
{
 double a = std::floor(CGAL::to_double(x));
 while (a > x) a -= 1;
 while (a+1 <= x) a += 1;
 return a;
}

void resolve_case(int n){
  int l,b,r,top;
  std::cin >> l >> b >> r >> top;
  
  std::vector<K::Point_2> germs;
  double x,y;
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    germs.push_back(K::Point_2(x,y));
  }
  
  double best_near, best_wall;
  std::vector<double> times;
  K::Point_2 ver1,ver2;
  
  if(germs.size()==1){
    x = round_down(germs[0].x());
    y = round_down(germs[0].y());
    best_wall = std::min(x-l,r-x);
    best_wall = std::min(best_wall, top-y);
    best_wall = std::min(best_wall, y-b);
    best_wall = std::ceil(std::sqrt(best_wall-0.5));
    std::cout<< std::setprecision(0) << std::fixed << best_wall << " " << best_wall << " " << best_wall << std::endl;
    return;
  }
  
  if(germs.size() == 2){
    x = round_down(germs[0].x());
    y = round_down(germs[0].y());
    ver2 = germs[1];
    best_near = std::sqrt((x-round_down(ver2.x()))*(x-round_down(ver2.x()))+(y-round_down(ver2.y()))*(y-round_down(ver2.y())))/2;
    best_wall = std::min(x-l,r-x);
    best_wall = std::min(best_wall, top-y);
    best_wall = std::min(best_wall, y-b);
    times.push_back(std::min(best_near,best_wall));
    x = round_down(germs[1].x());
    y = round_down(germs[1].y());
    best_wall = std::min(x-l,r-x);
    best_wall = std::min(best_wall, top-y);
    best_wall = std::min(best_wall, y-b);
    times.push_back(std::min(best_near,best_wall));
    std::sort(times.begin(),times.end());
    std::cout<< std::setprecision(0) << std::fixed << std::ceil(std::sqrt(times[0]-0.5)) << " " << std::ceil(std::sqrt(times[0]-0.5)) << " " << std::ceil(std::sqrt(times[1]-0.5)) << std::endl;
    return;
  }
  
  Triangulation t;
  t.insert(germs.begin(),germs.end());
  
  Triangulation::Edge_circulator c;
  for( Vertex_handle vh : t.finite_vertex_handles()){
    ver1 = vh -> point();
    ver2 = CGAL::nearest_neighbor(t,vh) -> point();
    x = round_down(ver1.x());
    y = round_down(ver1.y());
    best_near = std::sqrt((x-round_down(ver2.x()))*(x-round_down(ver2.x()))+(y-round_down(ver2.y()))*(y-round_down(ver2.y())))/2;
    best_wall = std::min(x-l,r-x);
    best_wall = std::min(best_wall, top-y);
    best_wall = std::min(best_wall, y-b);
    times.push_back(std::min(best_near,best_wall));
  }
  
  std::sort(times.begin(),times.end());
  
  
  std::cout<< std::setprecision(0) << std::fixed << std::ceil(std::sqrt(times[0]-0.5)) << " " << std::ceil(std::sqrt(times[times.size()/2]-0.5)) << " " << std::ceil(std::sqrt(times[times.size()-1]-0.5)) << std::endl;
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