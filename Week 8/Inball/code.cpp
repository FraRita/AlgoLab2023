#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <utility>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

bool resolve_case(){
  int n;
  std::cin>> n;
  if(n == 0) return false;
  
  int d;
  std::cin>> d;

  Solution s;
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  int a,norm;
  for(int i=0;i<n;i++){
    norm = 0;
    for(int j=0;j<d;j++){
      std::cin>> a;
      norm += a*a;
      lp.set_a(j,i,a);
      lp.set_a(j,n+i,a);
    }
    std::cin >> a;
    lp.set_b(i,a);
    lp.set_b(n+i,a);
    lp.set_a(d,n+i,std::sqrt(norm));
  }
  
  for(int i=0;i<d;i++) lp.set_c(i,0);
  lp.set_c(d,-1);
  lp.set_c0(0);
  
  s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
  
  if(s.is_infeasible()){
    std::cout<< "none" << std::endl;
    return true;
  }
  if(s.is_unbounded()){
    std::cout<< "inf" << std::endl;
    return true;
  }
  std::cout<< std::floor(CGAL::to_double(s.objective_value() * (-1))) << std::endl;
  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  while (resolve_case());
}