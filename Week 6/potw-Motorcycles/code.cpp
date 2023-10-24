#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Point_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/intersections.h>
#include <iostream>
#include <vector>
#include <utility>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

void resolve_case(){
  long n;
  std::cin >> n;
  std::vector<std::pair<long, std::pair<K::Point_2,K::Ray_2>>> res;
  std::vector<std::pair<long, std::pair<K::Point_2,K::Ray_2>>> memo(n);
  
  long x,y;
  for(long i=0;i<n;i++){
    memo[i].first = i;
    std::cin >> y;
    memo[i].second.first = K::Point_2(0,y);
    std::cin>> x >> y;
    memo[i].second.second = K::Ray_2(memo[i].second.first, K::Point_2(x,y));
  }
  
  std::sort(memo.begin(),memo.end(),[](std::pair<long, std::pair<K::Point_2,K::Ray_2>> a, std::pair<long, std::pair<K::Point_2,K::Ray_2>> b){return (a.second.first.y()<b.second.first.y());});
  
  res.push_back(memo[0]);
  for(long i=1;i<n;i++){
    if(!CGAL::do_intersect(res.back().second.second,memo[i].second.second)){
      res.push_back(memo[i]);
      continue;
    }
    bool flag = true;
    do{
    const auto temp = CGAL::intersection(res.back().second.second,memo[i].second.second);
    K::Point_2 intersec = *(boost::get<K::Point_2>(&*temp));
    
    if(CGAL::has_smaller_distance_to_point(intersec,memo[i].second.first,res.back().second.first)) res.pop_back();
    else flag = false;
    }while(flag && res.size() > 0 && CGAL::do_intersect(res.back().second.second,memo[i].second.second));
    if(flag) res.push_back(memo[i]);
  }
  
  std::sort(res.begin(),res.end(),[](std::pair<long, std::pair<K::Point_2,K::Ray_2>> a, std::pair<long, std::pair<K::Point_2,K::Ray_2>> b){return (a.first<b.first);});
  for(long i=0;i<res.size();i++) std::cout<< res[i].first << " ";
  std::cout<< std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while(t-- >0){
    resolve_case();
  }
}