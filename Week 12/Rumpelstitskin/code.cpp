#include <iostream>
#include <vector>
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
typedef boost::graph_traits<graph>::edge_iterator edge_it;

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

int MAX_PREF = (1<<10);

void resolve_case(){
  int n,pg,ph,eg,eh,fg,fh,sg,sh;
  std::cin>> n >> pg >> ph >> eg >> eh >> fg >> fh >> sg >> sh;
  
  graph G(2*n+pg+ph+2);
  edge_adder adder(G);
  int s = 2*n+pg+ph, t = 2*n+pg+ph+1;
  
  int u,v,c,pref;
  for(int i=0;i<n;i++){
    std::cin >> pref;
    adder.add_edge(i,n+i,1,MAX_PREF-pref);
  }

  for(int i=0;i<eg;i++){
    std::cin>> u >> v >> c;
    adder.add_edge(2*n+u,2*n+v,c,0);
  }
  for(int i=0;i<eh;i++){
    std::cin>> u >> v >> c;
    adder.add_edge(2*n+pg+v,2*n+pg+u,c,0);
  }
  for(int i=0;i<fg;i++){
    std::cin>> u >> v >> c;
    adder.add_edge(2*n+u,v,c,0);
  }
  for(int i=0;i<fh;i++){
    std::cin>> u >> v >> c;
    adder.add_edge(n+v,2*n+pg+u,c,0);
  }
  adder.add_edge(s,2*n,sg,0);
  adder.add_edge(2*n+pg,t,sh,0);
  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  boost::successive_shortest_path_nonnegative_weights(G, s, t);
  long cost = boost::find_flow_cost(G);
  int flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(t,G), G); e != eend; ++e){
    flow += rc_map[*e] - c_map[*e];
  }
  
  std::cout<< flow << " " << MAX_PREF*flow-cost << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0)resolve_case();
}