#include <iostream>
#include <vector>
#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;


void resolve_case(){
  int n,e,s;
  long start,end;
  std::cin >> n >> e >> s >> start >> end;
  graph g(n);
  std::vector<std::map<edge_desc,long>> weights(s);
  std::map<edge_desc,long> best_weights;
  
  for(int i=0;i<e;i++){
    int a,b;
    std::cin >> a >> b;
    boost::add_edge(a,b,g);
    edge_desc desc = boost::edge(a,b,g).first;
    long c;
    for(int j=0;j<s;j++){
      std::cin>>c;
      weights.at(j)[desc] = c;
    }
  }
  
  std::vector<int> hives;
  for(int j=0;j<s;j++){
      int h;
      std::cin>>h;
      hives.push_back(h);
  }
  
  std::vector<std::vector<edge_desc>> networks(s);
  
  for(int i=0;i<s;i++){
      boost::kruskal_minimum_spanning_tree(g,std::back_inserter(networks.at(i)),boost::weight_map(boost::make_assoc_property_map(weights.at(i))));
  }
  
  for(auto edge : weights.at(0)){
    edge_desc e = edge.first;
    long min = 100000*n+1;
    for(int i = 0;i<s;i++){
      if(std::find(networks.at(i).begin(),networks.at(i).end(),e) != networks.at(i).end() && weights.at(i).at(e) < min) min = weights.at(i).at(e);
    }
    best_weights[e] = min;
  }
  
  std::vector<long> dist_map(n);
  boost::dijkstra_shortest_paths(g, start,
  boost::distance_map(boost::make_iterator_property_map(dist_map.begin(),
  boost::get(boost::vertex_index, g))).weight_map(boost::make_assoc_property_map(best_weights)));
  
  std::cout<< dist_map.at(end) << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}