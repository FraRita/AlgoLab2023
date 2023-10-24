#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int n,m,a,H;
long P,W,b;
std::pair<long,std::vector<bool>> memo[101][1025];

int resolve_a(long p, int max_num){
  if(memo[max_num][H].first < p ) return -1;
  while(memo[max_num][H].first >= p) max_num--;
  max_num++;
  return max_num;
}

void fill_memo(std::vector<std::pair<int,int>> &type_a, int i, int j){
  for(int k=0;k<n;k++){
    if(j-type_a[k].second >= 0 && !memo[i-1][j-type_a[k].second].second[k] &&  memo[i-1][j-type_a[k].second].first != 0 && memo[i-1][j-type_a[k].second].first + type_a[k].first > memo[i][j].first){
      memo[i][j].first = memo[i-1][j-type_a[k].second].first + type_a[k].first;
      memo[i][j].second = memo[i-1][j-type_a[k].second].second;
      memo[i][j].second[k] = true;
    }
  }
  if(j<1024 && memo[i][j].first < memo[i][j+1].first) memo[i][j] = memo[i][j+1];
}


void resolve_case(){
  std::cin>> n >> m >> a >> b >> P >> H >> W;
  std::vector<std::pair<int,int>> type_a(n);
  std::vector<int> type_b(m);
  
  for(int i=0;i<n+1;i++){
    for(int j=0;j<1025;j++){
       memo[i][j] = {0,std::vector<bool>(n,false)};
    }
  }
  
  for(int i=0;i<n;i++){
    std::cin >> type_a[i].first >> type_a[i].second;
    if(type_a[i].first > memo[1][type_a[i].second].first){
      memo[1][type_a[i].second].first = type_a[i].first;
      memo[1][type_a[i].second].second = std::vector<bool>(n,false);
      memo[1][type_a[i].second].second[i] = true;
    }
  }
  
  std::pair<long,std::vector<bool>> curr_best = {0,std::vector<bool>(n,false)};
  for(int j=1024;j>=0;j--){
    if(memo[1][j].first > curr_best.first) curr_best = memo[1][j];
    else memo[1][j] = curr_best;
  }
  
  for(int i=0;i<m;i++) std::cin >> type_b[i];
  
  std::sort(type_b.begin(),type_b.end(),[](int x, int y){return (x>y);});
  
  long count_b = 0, count_a = -1;
  long sum = 0;
  while(count_b < m && sum < W){
    sum+=type_b[count_b];
    count_b++;
  }
  
  if(count_b == m && sum < W){
    std::cout << "-1" << std::endl;
    return;
  }
  
  
  for(int i=2;i<=n;i++){
    for(int j=1024; j>=0;j--){
      fill_memo(type_a,i,j);
    }
  }
  
  if(a == 0){
    count_a = resolve_a(P+count_b*b, n);
    if( count_a != -1 ) std::cout << count_b+count_a << std::endl;
    else std::cout<< "-1" << std::endl;
    return;
  }
  
  while(count_b <= m){
    count_a = resolve_a(P+count_b*b, std::min((sum-W)/a,(long)n));
    if( count_a != -1 ){
      std::cout << count_b+count_a << std::endl;
      return;
    }
    if(count_b == m) break;
    sum+=type_b[count_b];
    count_b++;
  }
  
  std::cout<< "-1" << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}