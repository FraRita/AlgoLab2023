#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int m;

void compact(std::vector<int> &danger, std::vector<std::vector<long>> &minerals, std::vector<std::vector<int>> &edges, int curr, int prev_dan, std::vector<int> &id, std::vector<int> &new_danger, std::vector<std::vector<long>> &new_minerals, std::vector<std::vector<int>> &new_edges){
  if(danger[curr] >= 0){
    new_danger[id[curr]] = danger[curr];
    new_edges[prev_dan].push_back(id[curr]);
    prev_dan = id[curr];
  }
  for(int v : edges[curr]){
    compact(danger,minerals,edges,v,prev_dan,id,new_danger,new_minerals,new_edges);
    if(danger[v] < 0){
      for(int i=0;i<m;i++) minerals[curr][i] += minerals[v][i];
    }
  }
  if(danger[curr] >= 0){
    for(int i=0;i<m;i++ ) new_minerals[id[curr]][i] = minerals[curr][i];
  }
}

int do_lp(Program &lp, std::vector<int> &danger, std::vector<std::vector<long>> &minerals, std::vector<std::vector<int>> &edges,int curr, int line ){
  for(int v : edges[curr]) line = do_lp(lp,danger,minerals,edges,v,line);
  for(int i=0;i<m;i++) lp.set_a(curr*m+i,line,1);
  lp.set_b(line,danger[curr]);
  line++;
  
  for(int i=0;i<m;i++){
    for(int v : edges[curr]) lp.set_a(v*m+i,line,-1);
    lp.set_a(curr*m+i,line,2);
    lp.set_b(line,2*minerals[curr][i]);
    line++;
  }

  return line;
}

void resolve_case(){
  int n;
  std::cin >> n >> m;
  
  std::vector<int> danger(n);
  std::vector<std::vector<long>> minerals(n,std::vector<long> (m));
  std::vector<std::vector<int>> edges(n);
  std::vector<int> needs(m);
  std::vector<std::pair<int,int>> shop(m);
  std::vector<int> id(n);
  int curr_id = 0;
  
  for(int i=0;i<n;i++){
    std::cin>> danger[i];
    for(int j=0;j<m;j++){
      std::cin>> minerals[i][j];
    }
    if(danger[i] >= 0){
      id[i] = curr_id;
      curr_id++;
    }
  }
  
  int u,v;
  for(int i=0;i<n-1;i++){
    std::cin >> u >> v;
    edges[v].push_back(u);
  }
  
  for(int i=0;i<m;i++){
    std::cin>> needs[i] >> shop[i].first >> shop[i].second;
  }
  
  std::vector<int> new_danger(curr_id);
  std::vector<std::vector<long>> new_minerals(curr_id,std::vector<long> (m));
  std::vector<std::vector<int>> new_edges(curr_id+1);
  compact(danger,minerals,edges,0,curr_id,id,new_danger,new_minerals,new_edges);
  
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  int line = 0;
  for(int v : new_edges[curr_id]) line = do_lp(lp,new_danger,new_minerals,new_edges,v,line);
  for(int i=0;i<m;i++){
    for(int v : new_edges[curr_id]) lp.set_a(v*m+i,line,-1);
    lp.set_a(curr_id*m+i,line,2);
    lp.set_b(line,2*minerals[0][i]);
    line++;
  }
  
  for(int i=0;i<m;i++){
    lp.set_a(curr_id*m+i,line,1);
    lp.set_a(curr_id*m+m+i,line,1);
    lp.set_b(line, needs[i]);
    line++;
    lp.set_a(curr_id*m+i,line,-1);
    lp.set_a(curr_id*m+m+i,line,-1);
    lp.set_b(line, -needs[i]);
    line++;
    lp.set_a(curr_id*m+m+i,line,1);
    lp.set_b(line, shop[i].first);
    line++;
  }
  
  for(int i=0;i<m;i++){
    lp.set_c(curr_id*m+m+i,shop[i].second);
  }
  
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()) std::cout << "Impossible!" << std::endl;
  else std::cout << std::setprecision(0) << std::fixed << std::floor(CGAL::to_double(s.objective_value())) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}