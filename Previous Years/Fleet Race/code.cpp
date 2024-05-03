#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

#define C 50

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
  int b,sa,p;
  std::cin>> b >> sa >> p;
  
  graph G(b+sa+2);
  edge_adder adder(G);
  int s = b+sa, t = b+sa+1;
  
  for(int i=0;i<b;i++){
    adder.add_edge(s,i,1,0);
    adder.add_edge(i,t,1,C);
  }
  for(int i=0;i<sa;i++) adder.add_edge(b+i,t,1,0);
  
  int from,to,c;
  for(int i=0;i<p;i++){
    std::cin>> from >> to >> c;
    adder.add_edge(from,b+to,1,C-c);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, s, t);
  int cost = boost::find_flow_cost(G);
  
  std::cout<< b*C-cost << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}