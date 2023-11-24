///2
#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <utility>
#include <vector>

typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void resolve_case(){
  int n,m;
  long s;
  std::cin>> n >> m >> s;

  Solution sol;
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  const int a = 0, fc = 1, sc = 2; // a x + y + fc = 0  and x - a y + sc = 0
  const int d = 3;
  
  int x,y;
  long sum_y = 0,sum_x = 0;
  std::vector<std::pair<int,int>> houses;
  for(int i=0;i<n;i++){
    std::cin >> x >> y;
    lp.set_a(a,i,-y); lp.set_a(sc,i,1); lp.set_b(i,-x);
    houses.push_back({x,y});
    sum_y += y;
    sum_x += x;
  }
  
  int row = n;
  for(int i=0;i<m;i++){
    std::cin >> x >> y;
    lp.set_a(a,row+i,y); lp.set_a(sc,row+i,-1); lp.set_b(row+i,x);
    houses.push_back({x,y});
    sum_y -= y;
    sum_x -= x;
  }
  
  sol = CGAL::solve_linear_program(lp, ET());
  if(sol.is_infeasible()){
    std::cout<< "Yuck!" << std::endl;
    return;
  }
  
  if(s != -1){
    lp.set_a(a,n+m,sum_y); lp.set_a(sc,n+m,m-n); lp.set_b(n+m,s+sum_x);
  
    sol = CGAL::solve_linear_program(lp, ET());
    if(sol.is_infeasible()){
      std::cout<< "Bankrupt!" << std::endl;
      return;
    }
    row = n+m+1;
  }
  else row = n+m;
  
  lp.set_c(d,1); lp.set_c0(0);
  int len = n+m;
  for(int i=0;i<len;i++){
    x = houses[i].first;
    y = houses[i].second;
    lp.set_a(a,row+i,-x); lp.set_a(fc,row+i,-1);lp.set_a(d,row+i,-1);lp.set_b(row+i,y);
    lp.set_a(a,row+len+i,x); lp.set_a(fc,row+len+i,1);lp.set_a(d,row+len+i,-1);lp.set_b(row+len+i,-y);
  }
  
  sol = CGAL::solve_linear_program(lp, ET());
  
  std::cout<< std::setprecision(0) << std::fixed << std::ceil(CGAL::to_double(sol.objective_value())) << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while (t-- > 0) resolve_case();
}