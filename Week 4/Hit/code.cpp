#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/intersections.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void resolve_case(int n){
  long a,b,c,d;
  std::cin >> a >> b >> c >> d;
  K::Point_2 p1(a,b), p2(c,d);
  K::Ray_2 ray(p1,p2);
  
  long p,q,r,s;
  
  while(n-- > 0){
    std::cin>> p >> q >> r >> s;
    K::Point_2 p3(p,q), p4(r,s);
    K::Segment_2 seg(p3,p4);
    if(CGAL::do_intersect(ray,seg)){
      std::cout<< "yes" << std::endl;
      while(n-- > 0) std::cin>> p >> q >> r >> s;
      return;
    }
  }
  
  std::cout<< "no" << std::endl;
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