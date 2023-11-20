#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor,
        boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;


class edge_adder {
  graph &G;
  public:
  explicit edge_adder(graph &G) : G(G) {}
  
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

void resolve_case(){
  int n,m,s; std::cin>> n >> m >> s;
  
  graph G(n+m+s+2);
  edge_adder adder(G);
  int source = n+m+s, sink = n+m+s+1;
  
  int in;
  
  for(int i=0;i<s;i++){
    std::cin>> in;
    adder.add_edge(n+m+i,sink,in,0);
  }
  for(int i=0;i<m;i++){
    std::cin>> in;
    adder.add_edge(n+i,n+m+in-1,1,0);
  }
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      std::cin>> in;
      adder.add_edge(i,n+j,1,100-in);
    }
    adder.add_edge(source,i,1,0);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, source, sink);
  int cost = boost::find_flow_cost(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(source,G), G); e != eend; ++e) {
    flow += c_map[*e] - rc_map[*e];
  }
  
  std::cout<< flow << " " << 100*flow-cost << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}