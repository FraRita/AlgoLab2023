///3
#include <iostream>
#include <utility>
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
  int l,p;
  std::cin>> l >> p;
  
  graph G (l+2);
  edge_adder adder(G);
  int s=l, t=l+1;
  
  long sum_d = 0;
  int g,d;
  std::vector<std::pair<int,int>> loc(l);
  for(int i=0;i<l;i++){
    std::cin >> g >> d;
    loc[i] = std::make_pair(g,d);
    sum_d += d;
  }
  
  int f,tar,c1,c2;
  for(int i=0;i<p;i++){
    std::cin >> f >> tar >> c1 >> c2;
    if(loc[f].first - c1 < 0){
      loc[f].second += c1 - loc[f].first;
      sum_d += c1 - loc[f].first;
      loc[f].first = 0;
    }
    else loc[f].first -= c1;
    if(loc[tar].second - c1 < 0){
      loc[tar].first += c1 - loc[tar].second;
      sum_d -= loc[tar].second;
      loc[tar].second = 0;
    }
    else{
      loc[tar].second -= c1;
      sum_d -= c1;
    }
    adder.add_edge(f,tar,c2-c1);
  }
  
  for(int i=0;i<l;i++){
    adder.add_edge(s,i,loc[i].first);
    adder.add_edge(i,t,loc[i].second);
  }
  

  if(boost::push_relabel_max_flow(G,s,t) == sum_d) std::cout << "yes" << std::endl;
  else std::cout << "no" << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}