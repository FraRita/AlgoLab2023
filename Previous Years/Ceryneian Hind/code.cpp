#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define CMAX 512000

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
  void add_edge(int from, int to, int capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void resolve_case(){
  int n,m;
  std::cin>> n >> m;
  
  graph G(n+2);
  edge_adder adder(G);
  int s = n+1, t=s+1;
  
  int c,sum=0;
  for(int i=0;i<n;i++){
    std::cin >> c;
    if(c>0){
      adder.add_edge(s,i,c);
      sum += c;
    }
    if(c<0) adder.add_edge(i,t,-c);
  }
  
  int u,v;
  for(int i=0;i<m;i++){
    std::cin>> u >> v;
    adder.add_edge(u,v,CMAX);
  }
  
  sum -= boost::push_relabel_max_flow(G,s,t);
  if(sum == 0) std::cout<< "impossible"  << std::endl;
  else std::cout<< sum  << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}