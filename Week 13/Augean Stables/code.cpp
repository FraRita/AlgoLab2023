#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#define MAX_HOURS 24

typedef int IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void resolve_case(){
  int n;
  std::cin>> n;
  
  Program lp (CGAL::LARGER, false, 0, false, 0);
  const int h1 = 0, h2 = 1, h3 = 2;
  
  std::vector<int> k(n),l(n),m(n);
  int f,c;
  for(int i=0;i<n;i++){
    std::cin>> f >> c >> k[i] >> l[i] >> m[i];
    lp.set_b(i,f-c);
  }
  lp.set_a(h1,n,-1);lp.set_b(n,-1);
  lp.set_a(h2,n+1,-1);lp.set_b(n+1,-1);
  lp.set_a(h3,n+2,-1);lp.set_b(n+2,-1);
  
  
  std::vector <int> a(MAX_HOURS+1),p(MAX_HOURS+1);
  a[0] = 0;
  p[0] = 0;
  for(int i=1;i<=MAX_HOURS;i++){
    std::cin >> a[i];
    a[i] += a[i-1];
  }
  for(int i=1;i<=MAX_HOURS;i++){
    std::cin >> p[i];
    p[i] += p[i-1];
  }
  
  int aa = a[MAX_HOURS]*a[MAX_HOURS];
  int pp = p[MAX_HOURS]*p[MAX_HOURS];
  int ap = a[MAX_HOURS]*p[MAX_HOURS];
  for(int j=0;j<n;j++){
    lp.set_a(h1,j,k[j]+aa); lp.set_a(h2,j,l[j]+pp); lp.set_a(h3,j,m[j]+ap);
  }
  Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
  if(s.is_infeasible()){
    std::cout<< "Impossible!" << std::endl;
    return;
  }
  
  std::vector<int> dp_a(MAX_HOURS+1,-1),dp_b(MAX_HOURS+1,-1);
  int start=0, end=2*MAX_HOURS-1, mid, res = 2*MAX_HOURS;
  
  while(true){
    mid = (start+end)/2;
    bool flag = false;
    int lim = std::min(mid,MAX_HOURS);
    for(int i=std::max(0,mid-MAX_HOURS);i<=lim && !flag;i++){
      if(dp_a[i] >= mid-i || dp_b[mid-i] >= i) continue;
      aa = a[i]*a[i];
      pp = p[mid-i]*p[mid-i];
      ap = a[i]*p[mid-i];
      for(int j=0;j<n;j++){
        lp.set_a(h1,j,k[j]+aa); lp.set_a(h2,j,l[j]+pp); lp.set_a(h3,j,m[j]+ap);
      }
      
      Solution s = CGAL::solve_nonnegative_linear_program(lp, ET());
      if(s.is_infeasible()){
        dp_a[i] = mid-i;
        dp_b[mid-i] = i;
      }
      else{
        flag = true;
        res = mid;
      }
    }
    if(start == end-1){
      std::cout<< res << std::endl;
      return;
    }
    if(flag) end = mid;
    else start = mid;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}