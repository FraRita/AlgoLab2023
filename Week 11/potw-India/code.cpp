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

graph G;
edge_desc s_edge;
int c,a,b;

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
  
  void add_source(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = 0;
    w_map[rev_e] = 0;
    s_edge = e;
  }
};

int binary(int start,int end){
  int mid = (start+end)/2;
  auto c_map = boost::get(boost::edge_capacity, G);
  c_map[s_edge] = mid;
  boost::successive_shortest_path_nonnegative_weights(G, c, a);
  long cost = boost::find_flow_cost(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  long flow = c_map[s_edge] - rc_map[s_edge];
  if(start == end-1){
    if(cost <= b) return flow;
    return -1;
  }
  if(cost <= b){
    int res = binary(mid,end);
    if(res == -1) return flow;
    return res;
  }
  return binary(start,mid);
}

void resolve_case(){
  int g,k; 
  std::cin>> c >> g >> b >> k >> a;
  
  G = graph (c+1);
  edge_adder adder(G);
  adder.add_source(c,k,b);
  
  int x,y,d,e;
  for(int i=0;i<g;i++){
    std::cin>> x >> y >> d >> e;
    adder.add_edge(x,y,e,d);
  }
  
  std::cout<< binary(0,b+1) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}