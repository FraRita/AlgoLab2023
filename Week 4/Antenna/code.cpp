#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Point_2.h>
#include <iostream>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

double round_up(const K::FT& x)
{
 double a = std::ceil(CGAL::to_double(x));
 while (a < x) a += 1;
 while (a-1 >= x) a -= 1;
 return a;
}

void resolve_case(int n){
  std::vector<K::Point_2> houses;
  
  for(int i=0;i<n;i++){
    long a,b;
    std::cin >> a >> b;
    houses.push_back(K::Point_2(a,b));
  }
  
  Traits::Circle c = Min_circle (houses.begin(),houses.end(), true).circle(); 
  std::cout << std::setprecision(0) << std::fixed << round_up(CGAL::sqrt(c.squared_radius())) << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  while(n>0){
    resolve_case(n);
    std::cin >> n;
  }
}