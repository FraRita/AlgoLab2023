#include <iostream>
#include <vector>

void resolve_case(){
  int n;
  std::cin >> n;
  int zero = 1000100;
  std::vector<int> par_plus (2*zero+102,0);
  std::vector<int> par_minus (2*zero+102,0);
  int min,max;
  std::cin >> min;
  max=min;
  par_plus.at(zero+min-100)++;
  par_minus.at(zero+min+101)--;
  for(int i=1;i<n;i++){
    int a;
    std::cin >> a;
    par_plus.at(zero+a-100)++;
    par_minus.at(zero+a+101)--;
    if(a<min) min = a;
    if(a>max) max = a;
  }

  std::vector<int> bars(max-min+201,0);
  int offset = min+zero-100;
  int incr = 0;
  int max_par = 0 ;
  std::vector<int> max_start;
  std::vector<int> max_end;
  for(int i=0;i<bars.size();i++){
    incr += par_plus.at(offset+i) + par_minus.at(offset+i);
    bars.at(i) += incr;
    if(bars.at(i) > max_par){
      max_par = bars.at(i);
      max_start.clear();
      max_end.clear();
    }
    if(par_plus.at(offset+i)>0 && max_par == bars.at(i)) max_start.push_back(i);
    if(par_minus.at(offset+i)<0 && bars.at(i-1) ==  max_par) max_end.push_back(i);
  }
  
  int nearest_par = 101;
  std::vector<int> best_positions;
  for(int i=0;i<max_start.size();i++){
    if((max_end.at(i) - max_start.at(i))%2 == 1){
      int near_par = 100 - (max_end.at(i) - max_start.at(i) - 1)/2;
      if(near_par < nearest_par){
        nearest_par = near_par;
        best_positions.clear();
      }
      if(near_par == nearest_par) best_positions.push_back((max_end.at(i) + max_start.at(i) - 1)/2);
    }
    else{
      int near_par = 101 - (max_end.at(i) - max_start.at(i))/2;
      if(near_par < nearest_par){
        nearest_par = near_par;
        best_positions.clear();
      }
      if(near_par == nearest_par){
        best_positions.push_back((max_end.at(i) + max_start.at(i))/2 -1);
        best_positions.push_back((max_end.at(i) + max_start.at(i))/2);
      }
    }
  }
  
  std::cout << max_par << " " << nearest_par << std::endl;
  for(int i=0;i<best_positions.size();i++){
    std::cout<< best_positions.at(i)-100+min << " ";
  }
  std::cout << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}