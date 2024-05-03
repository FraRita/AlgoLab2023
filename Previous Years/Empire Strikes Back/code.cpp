#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> Triangulation;
typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void resolve_case(){
  int a,s,b,e;
  std::cin>> a >> s >> b >> e;
  
  std::vector<std::pair<K::Point_2,int>> asteroids(a);
  int x,y,d;
  for(int i=0;i<a;i++){
    std::cin>> x >> y >> d;
    asteroids[i] = {K::Point_2(x,y),d};
  }
  
  std::vector<std::pair<K::Point_2,double>> shots(s);
  for(int i=0;i<s;i++){
    std::cin>> x >> y;
    shots[i].first = K::Point_2(x,y);
  }
  
  std::vector<K::Point_2> hunters(b);
  for(int i=0;i<b;i++){
    std::cin>> x >> y;
    hunters[i] = K::Point_2(x,y);
  }
  
  Triangulation t;
  t.insert(hunters.begin(),hunters.end());
  
  for(int i=0;i<s;i++) shots[i].second = CGAL::squared_distance(shots[i].first,t.nearest_vertex(shots[i].first) -> point());
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  for(int i=0;i<a;i++){
    for(int j=0;j<s;j++){
      double dist =  CGAL::squared_distance(asteroids[i].first,shots[j].first);
      if(dist < shots[j].second) lp.set_a(j,i,-1.0/CGAL::max(1.0,dist));
    }
    lp.set_b(i,-asteroids[i].second);
  }
  
  for(int i=0;i<s;i++) lp.set_a(i,a,1);
  lp.set_b(a,e);
  
  Solution sol = CGAL::solve_nonnegative_linear_program(lp, ET());
  
  if(sol.is_infeasible()) std::cout<< "n" << std::endl;
  else std::cout<< "y" << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}