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
  int w,h;
  std::cin>> w >> h;
  
  graph G(w*h+2);
  int s = w*h,t=w*h+1;
  edge_adder adder(G);
  
  int ver_id = 0, exp_flow = 0;
  char c;
  for(int i=0;i<h;i++){
    for(int j=0;j<w;j++){
      std::cin>> c;
      if(c == '.'){
        if((i+j)%2 == 0){
          adder.add_edge(s,ver_id);
          if(j>0) adder.add_edge(ver_id,ver_id-1);
          if(j<w-1) adder.add_edge(ver_id,ver_id+1);
          if(i>0) adder.add_edge(ver_id,ver_id-w);
          if(i<h-1) adder.add_edge(ver_id,ver_id+w);
        }
        else adder.add_edge(ver_id,t);
        exp_flow++;
      }
      ver_id++;
    }
  }
  
  if(exp_flow%2 == 1){
    std::cout << "no" << std::endl;
    return;
  }
  
  exp_flow = exp_flow/2;
  
  if(exp_flow == boost::push_relabel_max_flow(G, s, t)) std::cout << "yes" << std::endl;
  else std::cout << "no" << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}