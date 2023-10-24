#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

int n,m;
long D,T;
std::vector<std::pair<long,long>> moves(30), curr_moves(30);

bool check(){
  int mid=n/2, first_half = 1<<(mid), second_half = first_half;
  if(n%2 == 1) second_half = second_half<<1;
  std::vector<std::pair<long,long>> l1(first_half,{0,0}),l2(second_half,{0,0});
  int temp,j;
  for(int i=1;i<first_half;i++){
    temp = i;
    j = 0;
    while(temp != 0){
      if(temp%2 == 1){
        l1[i].first += curr_moves[j].first;
        l1[i].second += curr_moves[j].second;
        l2[i].first += curr_moves[mid+j].first;
        l2[i].second += curr_moves[mid+j].second;
      }
      temp = temp/2;
      j++;
    }
    if( (l1[i].first >= D && l1[i].second < T) || (l2[i].first >= D && l2[i].second < T) ) return true;
    l1[i].second = T - l1[i].second;
  }
  
  if(n%2 == 1){
    for(int i=first_half;i<second_half;i++){
      temp = i;
      j = 0;
      while(temp != 0){
        if(temp%2 == 1){
          l2[i].first += curr_moves[mid+j].first;
          l2[i].second += curr_moves[mid+j].second;
        }
        temp = temp/2;
        j++;
      }
      if(l2[i].first >= D && l2[i].second < T) return true;
    }
  }
  
  std::sort(l1.begin(),l1.end(),[](std::pair<long,long> a, std::pair<long,long> b){return (a.second < b.second);});
  std::sort(l2.begin(),l2.end(),[](std::pair<long,long> a, std::pair<long,long> b){return (a.second < b.second);});
  
  j=1;
  long max_dist = 0;
  for(int i=1;i<first_half;i++){
    while(j<second_half && l2[j].second < l1[i].second){
      if(l2[j].first > max_dist) max_dist = l2[j].first;
      j++;
    }
    if( max_dist != 0 && l1[i].first + max_dist >= D )return true;
  }
  
  return false;
  
}

int find_first(std::vector<long> &gulps, int start, int end){
  if(start == end-1){
    for(int i=0;i<n;i++) curr_moves[i].first = moves[i].first + gulps[start];
    if(check()) return start;
    return -1;
  }
  
  int mid;
  mid = (start+end)/2;
  for(int i=0;i<n;i++) curr_moves[i].first = moves[i].first + gulps[mid];
  if(check()){
    int res = find_first(gulps,start,mid);
    if(res == -1) return mid;
    return res;
  }
  
  if(mid == end-1) return -1;
  return find_first(gulps,mid+1,end);
}

void resolve_case(){
  std::cin>> n >> m >> D >> T;
  for(int i=0;i<n;i++){
    std::cin>> moves[i].first >> moves[i].second;
    curr_moves[i].second = moves[i].second;
  }
  std::vector<long> gulps(1,0);
  long g;
  for(int i=0;i<m;i++){
    std::cin>> g;
    gulps.push_back(g);
  }
  
  int res = find_first(gulps,0,m+1);
  if(res == -1) std::cout<< "Panoramix captured" << std::endl;
  else std::cout<< res << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}