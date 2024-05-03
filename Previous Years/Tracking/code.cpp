#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t, int>> graph;

void resolve_case(){
  int n,m,k,x,y;
  std::cin>> n >> m >> k >> x >> y;
  
  graph G(n*(k+1));
  int a,b,c,d;
  for(int i=0;i<m;i++){
    std::cin>> a >> b >> c >> d;
    if(d == 0) for(int j=0;j<=k;j++) boost::add_edge(j*n+a,j*n+b,c,G);
    else {
      for(int j=0;j<k;j++){
        boost::add_edge(j*n+a,(j+1)*n+b,c,G);
        boost::add_edge(j*n+b,(j+1)*n+a,c,G);
      }
      boost::add_edge(k*n+a,k*n+b,c,G);
    }
  }

  std::vector<int> dist_map(n*(k+1));  
  boost::dijkstra_shortest_paths(G, x, boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));

  std::cout<< dist_map[n*k+y] << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}