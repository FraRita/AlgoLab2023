#include <iostream>
#include <vector>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS,boost::no_property,boost::property<boost::edge_weight_t,int>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;


void resolve_case(){
  int n,m,k,T;
  std::cin >> n >> m >> k >> T;
  graph g(n+T);
  bool teleport[n] = {false};
  
  int u,v,w;
  for(int i=0;i<T;i++){
    std::cin>> u;
    teleport[u] = true;
  }
  
  for(int i=0;i<m;i++){
    std::cin >> u >> v >> w;
    boost::add_edge(v,u,w,g);
  }
  
  std::vector<int> scc_map(n+T);
  int nscc = boost::strong_components(g,
  boost::make_iterator_property_map(scc_map.begin(),
  boost::get(boost::vertex_index, g)));
  
  std::vector<std::vector<int>> scc_rev(nscc);
  for(int i=0;i<n;i++) if(teleport[i]) scc_rev[scc_map[i]].push_back(i);
  
  int t=-1;
  for(int i=0;i<nscc;i++){
    w = scc_rev[i].size()-1;
    if(w>0){
      t++;
      for(int j=0;j<=w;j++){
          boost::add_edge(scc_rev[i][j],n+t,w,g);
          boost::add_edge(n+t,scc_rev[i][j],0,g);
      }
    }
  }
  
  std::vector<int> dist_map(n+T);
  boost::dijkstra_shortest_paths(g, n-1,
  boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
  boost::get(boost::vertex_index, g))));
  
  int min = *std::min_element(dist_map.begin(),dist_map.begin()+k);
  
  if(min > 1000000) std::cout << "no" << std::endl;
  else std::cout<< min << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}