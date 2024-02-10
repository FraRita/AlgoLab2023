///4
#include <iostream>
#include <vector>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>

int n,k,m;
std::vector<int> fighters;

template <typename Container> 
struct container_hash {
    std::size_t operator()(Container const& c) const {
        return boost::hash_range(c.begin(), c.end());
    }
};
std::unordered_map<std::vector<int>,int,container_hash<std::vector<int>>> dp;

/*
0: current fighter
1: exit side (1 north, 0 south)
2: length diff (north-south)
3: m-1 north
4: m-1 south
5: m-2 north
6: m-2 south
*/

void best(std::vector<int> &state){
  int curr = state[0], side = state[1], diff = state[2];
  int points = 1000,idx;
  if(side == 1){
    idx = 3;
    diff++;
  }
  else{
    idx = 4;
    diff--;
  }
  if(state[idx] != -1 && state[idx] != fighters[curr]) points += 1000;
  if(state[idx+2] != -1 && state[idx+2] != state[idx] && state[idx+2] != fighters[curr]) points += 1000;
  points -= 1 << std::abs(diff);
  if(points < 0){
    dp[state] = -1;
    return;
  }
  if(curr == n-1){
    dp[state] = points;
    return;
  }
  std::vector<int> next_state = {curr+1,0,diff,state[3],state[4],state[5],state[6]};
  if(m == 3) next_state[idx+2] = next_state[idx];
  next_state[idx] = fighters[curr];
  auto south = dp.find(next_state);
  if(south == dp.end()){
    best(next_state);
    south = dp.find(next_state);
  }
  next_state[1] = 1;
  auto north = dp.find(next_state);
  if(north == dp.end()){
    best(next_state);
    north = dp.find(next_state);
  }
  if(north->second == -1 && south->second == -1){
    dp[state] = -1;
    return;
  }
  dp[state] = points + std::max(north->second,south->second);
}

void resolve_case(){
  std::cin>> n >> k >> m;
  
  dp.clear();
  fighters = std::vector<int>(n);
  for(int i=0;i<n;i++) std::cin>> fighters[i];
  
  std::vector<int> temp = {0,1,0,-1,-1,-1,-1};
  best(temp);
  std::cout<< dp[temp] << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}