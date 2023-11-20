#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <utility>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

bool resolve_case(){
  int n,m;
  std::cin>> n >> m;
  if(n == 0 && m == 0) return false;

  Solution s;
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  int min,max;
  for(int i=0;i<n;i++){
    std::cin>>min >> max;
    lp.set_b(i,-min);
    lp.set_b(n+i,max);
  }
  
  int p,c;
  for(int i=0;i<m;i++){
    std::cin>> p;
    lp.set_c(i,p);
    for(int j=0;j<n;j++){
      std::cin >> c;
      lp.set_a(i,j,-c);
      lp.set_a(i,n+j,c);
    }
  }
  lp.set_c0(0);
  
  s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
  
  if(s.is_infeasible()) std::cout<< "No such diet." << std::endl;
  else std::cout<< std::floor(CGAL::to_double(s.objective_value())) << std::endl;
  
  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  while (resolve_case());
}