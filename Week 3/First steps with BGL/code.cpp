#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::property<boost::edge_weight_t, int>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;

void resolve_case(){
  int n,m;
  std::cin >> n >> m;
  graph g(n);
  
  for(int i=0;i<m;i++){
    int a,b,w;
    std::cin>> a >> b >> w;
    boost::add_edge(a,b,w,g);
  }
  
  std::vector<edge_desc> mst;
  boost::kruskal_minimum_spanning_tree(g, std::back_inserter(mst));
  
  int sum=0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) sum+=get(boost::edge_weight,g,*it);
  
  std::vector<int> dist_map(n);
  boost::dijkstra_shortest_paths(g, 0,
  boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
  boost::get(boost::vertex_index, g))));
  
  std::cout<< sum << " " << *(std::max_element(dist_map.begin(),dist_map.end())) << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}