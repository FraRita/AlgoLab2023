#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <vector>
#include <iostream>

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

void resolve_case(){
  int n,m,c;
  std::cin >> n >> m >> c;
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  std::vector<P> wh(n),st(m);
  std::vector<int> alc(n),rewards(n*m);
  
  int x,y,l,a;
  for(int i=0;i<n;i++){
    std::cin>> x >> y >> l >> alc[i];
    wh[i] = P(x,y);
    lp.set_b(i,l);
  }
  
  for(int j=0;j<m;j++){
    std::cin>> x >> y >> l >> a;
    st[j] = P(x,y);
    lp.set_b(n+j,l);
    lp.set_b(n+m+j,-l);
    lp.set_b(n+2*m+j,a*100);
  }
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      int curr = m*i+j;
      std::cin>> rewards[curr];
      rewards[curr] = rewards[curr]*100;
    }
  }
  
  long r;
  P center;
  std::vector<std::vector<bool>> wh_lines(c,std::vector<bool> (n,false)), st_lines(c,std::vector<bool> (m,false));
  std::vector<int> not_empty;
  bool pushed;
  for(int k=0;k<c;k++){
    std::cin>> x >> y >> r;
    r *= r;
    center = P(x,y);
    pushed = false;
    for(int i=0;i<n;i++) if(CGAL::squared_distance(center, wh[i]) <= r){
      wh_lines[k][i] = true;
      if(!pushed){
        not_empty.push_back(k);
        pushed = true;
      }
    }
    for(int j=0;j<m;j++) if(CGAL::squared_distance(center, st[j]) <= r){
      st_lines[k][j] = true;
      if(!pushed){
        not_empty.push_back(k);
        pushed = true;
      }
    }
  }
  
  for(int k : not_empty){
    for(int i=0;i<n;i++){
      for(int j=0;j<m;j++){
        if(wh_lines[k][i] != st_lines[k][j]) rewards[m*i+j]--;
      }
    }
  }
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      int curr = m*i+j;
      lp.set_c(curr,-1*rewards[curr]);
      lp.set_a(curr,i,1);
      lp.set_a(curr,n+j,1);
      lp.set_a(curr,n+m+j,-1);
      lp.set_a(curr,n+2*m+j,alc[i]);
    }
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if(s.is_infeasible()) std::cout<< "RIOT!" << std::endl;
  else{
    double sol = std::floor(to_double(s.objective_value()) / (-100));
    if(sol == 0) std::cout << 0 << std::endl;
    else std::cout<< std::setprecision(0) << std::fixed << sol << std::endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}