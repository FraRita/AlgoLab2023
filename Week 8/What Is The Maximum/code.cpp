#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <utility>

typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

bool resolve_case(){
  int p,a,b;
  std::cin>> p;
  if(p == 0) return false;

  std::cin >> a >> b;
  Solution s;
  if(p == 1){
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    int x = 0;
    int y = 1;
    lp.set_a(x,0,1); lp.set_a(y,0,1); lp.set_b(0,4);
    lp.set_a(x,1,4); lp.set_a(y,1,2); lp.set_b(1,a*b);
    lp.set_a(x,2,-1); lp.set_a(y,2,1); lp.set_b(2,1);
    lp.set_c(x,a); lp.set_c(y,-b); lp.set_c0(0);
    s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
  } 
  else{
    Program lp (CGAL::SMALLER, false, 0, true, 0);
    int x = 0;
    int y = 1;
    int z = 2;
    lp.set_a(x,0,-1); lp.set_a(y,0,-1); lp.set_a(z,0,0); lp.set_b(0,4);
    lp.set_a(x,1,-4); lp.set_a(y,1,-2); lp.set_a(z,1,-1); lp.set_b(1,a*b);
    lp.set_a(x,2,1); lp.set_a(y,2,-1); lp.set_a(z,2,0); lp.set_b(2,1);
    lp.set_c(x,a); lp.set_c(y,b); lp.set_c(z,1); lp.set_c0(0);
    s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
  }
  
  if(s.is_infeasible()){
    std::cout << "no" << std::endl;
    return true;
  }
  if(s.is_unbounded()){
    std::cout << "unbounded" << std::endl;
    return true;
  }
  
  if(p == 1) std::cout<< std::floor(s.objective_value().numerator().to_double() / s.objective_value().denominator().to_double() * (-1)) << std::endl;
  else std::cout<< std::ceil(s.objective_value().numerator().to_double() / s.objective_value().denominator().to_double()) << std::endl;
  
  return true;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  while (resolve_case());
}