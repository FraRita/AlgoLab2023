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

void resolve_case(){
  int n,m,k,l;
  std::cin >> n >> m >> k >> l;
  int s = 2*n, t = 2*n+1;
  
  graph G(2*n+2);
  edge_adder adder(G);
  
  int u,v;
  for(int i=0;i<k;i++){
    std::cin>> v;
    adder.add_edge(s,v,1);
    adder.add_edge(n+v,t,1);
  }
  
  for(int i=0;i<l;i++){
    std::cin>> v;
    adder.add_edge(v,n+v,1);
  }
  
  for(int i=0;i<m;i++){
    std::cin>> u >> v;
    adder.add_edge(u,v,k);
    adder.add_edge(n+u,n+v,1);
  }
  
  std::cout<< boost::push_relabel_max_flow(G,s,t) << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}