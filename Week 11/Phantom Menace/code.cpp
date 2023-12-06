#include <iostream>
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
  
  void add_edge(int from, int to) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = 1;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void resolve_case(){
  int n,m,s,d;
  std::cin>> n >> m >> s >> d;
  
  graph G(2*n+2);
  int source = 2*n, sink= 2*n+1;
  edge_adder adder(G);

  for(int i=0;i<n;i++){
    adder.add_edge(i,n+i);
  }
  
  int a,b;
  for(int i=0;i<m;i++){
    std::cin >> a >> b;
    adder.add_edge(n+a,b);
  }
  
  for(int i=0;i<s;i++){
    std::cin >> a;
    adder.add_edge(source,a);
  }
  
  for(int i=0;i<d;i++){
    std::cin >> a;
    adder.add_edge(n+a,sink);
  }
  
  std::cout << boost::push_relabel_max_flow(G, source, sink) << std::endl;;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}