#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Quadratic_program<long> Program;
typedef CGAL::Quadratic_program_solution<CGAL::Gmpz> Solution;

void resolve_case(){
  long xs,ys,n;
  std::cin>> xs >> ys >> n;
  
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  const int x = 0, y = 1, t = 2;
  lp.set_l(t,true,0);
  lp.set_c(t,-1);
  
  long a,b,c,v;
  for(int i=0;i<n;i++){
    std::cin>> a >> b >> c >> v;
    if( a*xs+b*ys+c > 0){
      a = -a;
      b = -b;
      c = -c;
    }
    lp.set_a(x,i,a); lp.set_a(y,i,b);lp.set_a(t,i,v*std::sqrt(a*a+b*b)); lp.set_b(i,-c);
  }
  
  Solution s = CGAL::solve_linear_program(lp, CGAL::Gmpz());
  
  std::cout << std::setprecision(0) << std::fixed << std::floor(CGAL::to_double(s.objective_value() * (-1))) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}