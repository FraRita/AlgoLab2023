#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/intersections.h>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cstdlib>

typedef CGAL::Exact_predicates_exact_constructions_kernel K1;

double round_down(const K1::FT& x)
{
 double a = std::floor(CGAL::to_double(x));
 while (a > x) a -= 1;
 while (a+1 <= x) a += 1;
 return a;
}

void resolve_case(int n){
  long a,b,c,d;
  std::cin >> a >> b >> c >> d;
  K1::Point_2 start(a,b);
  K1::Ray_2 ray1(K1::Point_2 (a,b), K1::Point_2 (c,d));
  
  long p,q,r,s;
  K1::Point_2 best_intersec;
  std::vector<std::pair<K1::Point_2,K1::Point_2>> segments;
  bool flag = false, right = false;
  if(c > a) right = true;
  
  for(int i=0;i<n;i++){
    std::cin>> p >> q >> r >> s;
    segments.push_back({K1::Point_2 (p,q), K1::Point_2 (r,s)});
  }
  std::random_shuffle(segments.begin(), segments.end());
  
  while(n-- > 0 && !flag){
    K1::Segment_2 seg(segments[n].first,segments[n].second);
    if(CGAL::do_intersect(ray1,seg)){
      flag = true;
      const auto res = CGAL::intersection(ray1,seg);
      K1::Point_2 intersec;
      if(const K1::Point_2 *point = boost::get<K1::Point_2>(&*res)){
        best_intersec = *point;
      }
      else{
        if(right) best_intersec = (segments[n].first.x() < segments[n].second.x()  ? segments[n].first : segments[n].second);
        else best_intersec = (segments[n].first.x() < segments[n].second.x()  ? segments[n].first : segments[n].second);
      }
    }
  }
  
  if(!flag){
    std::cout<< "no" << std::endl;
    return;
  } 
  if(start == best_intersec){
    std::cout << std::setprecision(0) << std::fixed << round_down(best_intersec.x()) << " " << round_down(best_intersec.y()) << std::endl;
    return;
  }
  
  K1::Segment_2 best_seg(start,best_intersec);
  n++;
  while(n-- > 0){
    if((right && segments[n].first.x() > best_intersec.x() && segments[n].second.x() > best_intersec.x()) || (!right && segments[n].first.x() < best_intersec.x() && segments[n].second.x() < best_intersec.x())) continue;
    K1::Segment_2 seg(segments[n].first,segments[n].second);
    if(CGAL::do_intersect(best_seg,seg)){
      const auto res = CGAL::intersection(best_seg,seg);
      K1::Point_2 intersec;
      if(const K1::Point_2 *point = boost::get<K1::Point_2>(&*res)){
        intersec = *point;
      }
      else{
        if(right) intersec = (segments[n].first.x() < segments[n].second.x()  ? segments[n].first : segments[n].second);
        else intersec = (segments[n].first.x() < segments[n].second.x()  ? segments[n].first : segments[n].second);
      }
      
      if( (right && intersec.x() < best_intersec.x()) || (!right && intersec.x() > best_intersec.x()) ){
        best_intersec = intersec;
        if(start == best_intersec){
          std::cout << std::setprecision(0) << std::fixed << round_down(best_intersec.x()) << " " << round_down(best_intersec.y()) << std::endl;
          return;
        }
        best_seg = K1::Segment_2 (start,best_intersec);
        
      }
    }
  }
  
  std::cout << std::setprecision(0) << std::fixed << round_down(best_intersec.x()) << " " << round_down(best_intersec.y()) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  std::srand ( unsigned ( std::time(0) ) );
  int n;
  std::cin >> n;
  while(n>0){
    resolve_case(n);
    std::cin >> n;
  }
}