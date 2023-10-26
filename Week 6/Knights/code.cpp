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
  
  void add_edge(int from, int to, int cap) {
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
  int n,m,k,c;
  std::cin>> m >> n >> k >> c;
  
  graph G (n*m*2+2);
  edge_adder adder(G);
  int s=n*m*2, t=n*m*2+1;
  
  int out;
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      adder.add_edge((i*m+j)*2,(i*m+j)*2+1,c);
      out = 0 + (i == 0) + (i == n-1) + (j == 0) + (j == m-1);
      if(out != 0) adder.add_edge((i*m+j)*2+1,t,out);
      if(i != 0) adder.add_edge((i*m+j)*2+1,((i-1)*m+j)*2,1);
      if(i != n-1) adder.add_edge((i*m+j)*2+1,((i+1)*m+j)*2,1);
      if(j != 0) adder.add_edge((i*m+j)*2+1,(i*m+j-1)*2,1);
      if(j != m-1) adder.add_edge((i*m+j)*2+1,(i*m+j+1)*2,1);
    }
  }
  
  int x,y;
  for(int i=0;i<k;i++){
    std::cin >> x >> y;
    adder.add_edge(s,(y*m+x)*2,1);
  }
  
  std::cout << boost::push_relabel_max_flow(G,s,t) << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}