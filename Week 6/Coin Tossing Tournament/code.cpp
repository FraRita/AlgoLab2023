#include <iostream>
#include <vector>
#include <cstring>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;


class edge_adder {
  graph &G;
  public:
  explicit edge_adder(graph &G) : G(G) {}
  
  void add_edge(int from, int to,int cap) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = cap;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int matches[200][200];

void resolve_case(){
  int n,m;
  std::cin >> n >> m;
  std::vector<int> scores(n,0);
  for(int i=0;i<n-1;i++) memset(matches[i],0,(n-1-i)*sizeof(int));
  
  int a,b,c;
  for(int i=0;i<m;i++){
    std::cin >> a >> b >> c;
    if(c == 0){
      if(a<b) matches[a][b-a-1]++;
      else matches[b][a-b-1]++;
    }
    else{
      if(c == 1) scores[a]++;
      else scores[b] ++;
    }
  }
  
  int flow=0,total=0;
  for(int i=0;i<n;i++){
    std::cin >> c;
    if(scores[i] > c){
      std::cout << "no" << std::endl;
      while(++i<n) std::cin>> c;
      return;
    }
    scores[i] = c-scores[i];
    flow += scores[i];
    total += c;
  }
  
  if(total != m){
    std::cout << "no" << std::endl;
    return;
  }
  
  m=n*(n-1)/2;
  graph G(m+n+2);
  edge_adder adder(G);
  int s=m+n, t=m+n+1;
  int ver_id=0;
  for(int i=0;i<n-1;i++){
    for(int j=0;j<n-1-i;j++){
      if(matches[i][j] != 0){
        adder.add_edge(s,ver_id,matches[i][j]);
        adder.add_edge(ver_id,m+i,matches[i][j]);
        adder.add_edge(ver_id,m+i+j+1,matches[i][j]); 
      }
      ver_id++;
    }
  }
  for(int i=0;i<n;i++) adder.add_edge(m+i,t,scores[i]);
  
  if(flow == boost::push_relabel_max_flow(G, s, t)) std::cout << "yes" << std::endl;
  else std::cout << "no" << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}