#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS,boost::no_property,boost::property<boost::edge_weight_t, long>> weighted_graph;
typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

const long MAX_TIME = 50000000;
int a,s,c;

long binary_search(int start,int end, std::vector<std::tuple<int,int,long>> &times){
  if(start == end-1){
    graph G(a+c*s);
    for(int i=0;i<=start;i++) boost::add_edge(std::get<0>(times[i]),std::get<1>(times[i]),G);
    
    std::vector<vertex_desc> mate_map(a+c*s);
    boost::edmonds_maximum_cardinality_matching(G,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, G)));
    int matching_size = boost::matching_size(G,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, G)));
    if(matching_size < a) return -1;
    return std::get<2>(times[start]);
  }
  int mid = (start+end)/2;
  graph G(a+c*s);
  for(int i=0;i<=mid;i++) boost::add_edge(std::get<0>(times[i]),std::get<1>(times[i]),G);
  
  std::vector<vertex_desc> mate_map(a+c*s);
  boost::edmonds_maximum_cardinality_matching(G,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,boost::make_iterator_property_map(mate_map.begin(),boost::get(boost::vertex_index, G)));
  if(matching_size < a) return binary_search(mid,end,times);
  long res = binary_search(start,mid,times);
  if(res == -1) return std::get<2>(times[mid]);
  return res;
}

void resolve_case(){
  int n,m;
  long d;
  std::cin>> n >> m >> a >> s >> c >> d;
  
  weighted_graph G(n);
  std::vector<int> agents(a),shelters(s);
  std::vector<std::tuple<int,int,long>> times;
  
  char w;
  int x,y,z;
  for(int i=0;i<m;i++){
    std::cin>> w >> x >> y >> z;
    boost::add_edge(x,y,z,G);
    if(w == 'L') boost::add_edge(y,x,z,G);
  }
  
  for(int i=0;i<a;i++) std::cin>> agents[i];
  for(int i=0;i<s;i++) std::cin>> shelters[i];
  
  for(int i=0;i<a;i++){
    std::vector<long> dist_map(n);
    boost::dijkstra_shortest_paths(G, agents[i], boost::distance_map(boost::make_iterator_property_map(dist_map.begin(), boost::get(boost::vertex_index, G))));
    for(int j=0;j<s;j++) if(dist_map[shelters[j]] >= 0 && dist_map[shelters[j]] <= MAX_TIME){
      times.push_back({i,a+j,dist_map[shelters[j]]});
      if(c == 2) times.push_back({i,a+s+j,d+dist_map[shelters[j]]});
    }
  }
  std::sort(times.begin(),times.end(),[](std::tuple<int,int,long> &e1, std::tuple<int,int,long> &e2){return (std::get<2>(e1)<std::get<2>(e2));});
  
  std::cout<< d+binary_search(0,times.size(),times) << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}