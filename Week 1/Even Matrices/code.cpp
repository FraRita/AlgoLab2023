#include <iostream>
#include <vector>

void resolve_case(){
  int n;
  std::cin >> n;
  std::vector<std::vector<int>> mat(n,std::vector<int>(n));
  
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      int num;
      std::cin >> num;
      mat.at(i).at(j) = num + (i>0 ? mat.at(i-1).at(j) : 0)
                            + (j>0 ? mat.at(i).at(j-1) : 0)
                            - ( (i>0 && j>0) ?  mat.at(i-1).at(j-1) : 0 );
    }
  }
  
  int even_quad=0;
  
  for(int dec=0;dec<n;dec++){
    for(int i=dec;i<n;i++){
      int even_row = 1;
      for(int j=0;j<n;j++){
        if(mat.at(i).at(j)%2 == 0) even_row++;
      }
      even_quad += even_row*(even_row-1)/2 + (n-even_row+1)*(n-even_row)/2;
    }
    
    for(int i=dec+1;i<n;i++){
      for(int j=0;j<n;j++){
        mat.at(i).at(j) -= mat.at(dec).at(j);
      }
    }
  }
  
  std::cout << even_quad << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}