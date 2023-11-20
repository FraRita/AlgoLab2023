///4
#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <utility>
#include <vector>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

void resolve_case(){
  int n,m,h,w;
  std::cin>> n >> m >> h >> w;
  
  std::vector<std::pair<int,int>> new_ones(n), old_ones(m);
  
  for(int i=0;i<n;i++){
    int x,y;
    std::cin>> x >> y;
    new_ones[i] = std::make_pair(x,y);
  }
  
  for(int i=0;i<m;i++){
    int x,y;
    std::cin>> x >> y;
    old_ones[i] = std::make_pair(x,y);
  }

  Solution s;
  Program lp (CGAL::SMALLER, true, 1, false, 0);
  lp.set_c0(0);
  for(int i=0;i<n;i++) lp.set_c(i,(-2) * (w+h)); 
  
  long x,y,x1,y1;
  double dist_x, dist_y;
  int row = 0;
  for(int i=0;i<n-1;i++){
    x = new_ones[i].first;
    y = new_ones[i].second;
    for(int j=i+1;j<n;j++){
      x1 = new_ones[j].first;
      y1 = new_ones[j].second;
      dist_y = std::abs(y-y1)*w;
      dist_x = std::abs(x-x1)*h;
      if(dist_y > dist_x){
        lp.set_a(i,row,h);
        lp.set_a(j,row,h);
        lp.set_b(row,std::abs(y-y1)*2);
      }
      else{
        lp.set_a(i,row,w);
        lp.set_a(j,row,w);
        lp.set_b(row,std::abs(x-x1)*2);
      }
      row++;
    }
  }
  
  long min_dist_y,min_dist_x;
  for(int i=0;i<n;i++){
    x = new_ones[i].first;
    y = new_ones[i].second;
    min_dist_y = -1;
    min_dist_x = -1;
    for(int j=0;j<m;j++){
      x1 = old_ones[j].first;
      y1 = old_ones[j].second;
      dist_y = std::abs(y-y1)*w;
      dist_x = std::abs(x-x1)*h;
      if(dist_y > dist_x){
        if(min_dist_y == -1) min_dist_y = std::abs(y-y1);
        else min_dist_y = std::min(min_dist_y,std::abs(y-y1));
      }
      else{
        if(min_dist_x == -1) min_dist_x = std::abs(x-x1);
        else min_dist_x = std::min(min_dist_x,std::abs(x-x1));
      }
    }
    if(min_dist_y != -1){
      lp.set_a(i,row,h);
      lp.set_b(row,min_dist_y*2-h);
      row++;
    }
    if(min_dist_x != -1){
      lp.set_a(i,row,w);
      lp.set_b(row,min_dist_x*2-w);
      row++;
    }
  }
  
  s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
  
  std::cout<< std::setprecision(0) << std::fixed << std::ceil(s.objective_value().numerator().to_double() / s.objective_value().denominator().to_double() * (-1)) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while(t-- > 0) resolve_case();
}