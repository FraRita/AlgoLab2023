#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor,
        boost::property <boost::edge_weight_t, int> > > > > graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it; 

class edge_adder {
graph &G;

public:
explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, int capacity, int cost) {
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
  int e,w,m,d,p,l;
  std::cin>> e >> w >> m >> d >> p >> l;
  
  graph G(2*(e+w+2));
  edge_adder adder(G);
  int s = 2*(e+w), s1 = s+1, t = s1+1, t1 = t+1;
  adder.add_edge(s,s1,p-(l*e),0);
  adder.add_edge(t1,t,p-(l*w),0);
  
  for(int i=0;i<e;i++){
    adder.add_edge(e+w+i,i,p,0);
    adder.add_edge(s,i,l,0);
    adder.add_edge(s1,e+w+i,p,0);
  }
  for(int i=0;i<w;i++){
    adder.add_edge(e+i,e*2+w+i,p,0);
    adder.add_edge(e+i,t,l,0);
    adder.add_edge(e*2+w+i,t1,p,0);
  }
  
  int u,v,r;
  for(int i=0;i<m;i++){
    std::cin>> u >> v >> r;
    adder.add_edge(u,e+v,1,r);
  }
  for(int i=0;i<d;i++){
    std::cin>> u >> v >> r;
    adder.add_edge(e+w+u,e*2+w+v,1,r);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, s, t);
  int cost = boost::find_flow_cost(G), flow = 0;
  auto c_map = boost::get(boost::edge_capacity, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  out_edge_it ebeg, eend;
  for(boost::tie(ebeg, eend) = boost::out_edges(boost::vertex(s,G), G); ebeg != eend; ++ebeg) flow += c_map[*ebeg] - rc_map[*ebeg];
  
  if(flow != p) std::cout<< "No schedule!" << std::endl;
  else std::cout<< cost << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}