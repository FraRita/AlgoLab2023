#include <iostream>
#include <vector>
#include <tuple>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t, int>> w_graph;
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
  int n,m,s,f;
  std::cin>> n >> m >> s >> f;
  
  w_graph WG(n);
  graph G(n);
  edge_adder adder(G);
  
  std::vector<std::vector<std::tuple<int,int,int>>> edges(n);
  int from,to,c,l;
  for(int i=0;i<m;i++){
    std::cin>> from >> to >> c >> l;
    if(from == to) continue;
    boost::add_edge(from,to,l,WG);
    edges[from].push_back({to,c,l});
  }
  
  std::vector<int> s_dist(n);
  boost::dijkstra_shortest_paths(WG, s, boost::distance_map(boost::make_iterator_property_map(s_dist.begin(), boost::get(boost::vertex_index, WG))));
  
  std::vector<int> f_dist(n);
  boost::dijkstra_shortest_paths(WG, f, boost::distance_map(boost::make_iterator_property_map(f_dist.begin(), boost::get(boost::vertex_index, WG))));
  
  for(int i=0;i<n;i++){
    if(s_dist[i] + f_dist[i] != s_dist[f]) continue;
    for(auto e : edges[i]){
      if(s_dist[i] == s_dist[std::get<0>(e)] + std::get<2>(e)) adder.add_edge(std::get<0>(e),i,std::get<1>(e));
      if(s_dist[std::get<0>(e)] == s_dist[i] + std::get<2>(e)) adder.add_edge(i,std::get<0>(e),std::get<1>(e));
    }
  }
  
  std::cout<< boost::push_relabel_max_flow(G,s,f) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}