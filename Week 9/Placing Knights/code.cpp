///1
#include <iostream>
#include <vector>
#include <queue>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
        boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;
typedef traits::vertex_descriptor vertex_desc;

typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

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

int n;
bool is_valid(int x, int y){
  return (x>= 0 && x<n && y>=0 && y<n);
}

void resolve_case(){
  std::cin>> n;
  
  graph G(n*n+2);
  edge_adder adder(G);
  int s = n*n, t = n*n+1;
  
  std::vector<int> present(n*n);
  std::vector<bool> left(n*n,false);
  for(int i=0;i<n;i++){
    std::cin>> present[i];
    left[i] = (i%2 == 0);
    if(present[i]){
      if(left[i]) adder.add_edge(s,i);
      else adder.add_edge(i,t);
    }
    else if(!left[i]) adder.add_edge(s,i);
  }
  
  std::vector<int> moves_x(4),moves_y(4);
  moves_x[0] = -2; moves_x[1] = -1; moves_x [2] = 1; moves_x[3] = 2;
  moves_y[0] = -1; moves_y[1] = -2; moves_y [2] = -2; moves_y[3] = -1;
  int x,y,id;
  for(int i=1;i<n;i++){
    for(int j=0;j<n;j++){
      id = n*i+j;
      std::cin>> present[id];
      for(int k=0;k<4;k++){
        y = i+moves_y[k];
        x = j+moves_x[k];
        if(is_valid(x,y)){
          left[id] = !left[n*y+x];
          break;
        }
      }
      if(present[id]){
        if(left[id]) adder.add_edge(s,id);
        else adder.add_edge(id,t);
        for(int k=0;k<4;k++){
          y = i+moves_y[k];
          x = j+moves_x[k];
          if(is_valid(x,y) && present[n*y+x]){
            if(left[id]) adder.add_edge(id,n*y+x);
            else adder.add_edge(n*y+x,id);
          }
        }
      }
      else if(!left[id]) adder.add_edge(s,id);
    }
  }
  
  boost::push_relabel_max_flow(G,s,t);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  std::vector<bool> vis(n*n, false);
  std::queue<int> Q;
  vis[s] = true;
  Q.push(s);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }
  
  int res = 0;
  for(int i=0;i<n*n;i++){
    if((left[i] && vis[i]) || (!left[i] && !vis[i])) res++;
  }
  
  std::cout << res << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}