#include <iostream>
#include <vector>
#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

void resolve_case(){
  int n,c,f;
  std::cin >> n >> c >> f;
  std::map<std::string,std::vector<int>> stud;
  std::vector<std::vector<int>> common(n,std::vector<int>(n,0));
  graph g(n);
  
  for(int i=0;i<n;i++){
    for(int j=0;j<c;j++){
      std::string key;
      std::cin>> key;
      stud[key].push_back(i);
    }
  }
  
  for(auto key : stud){
    std::vector<int> curr_vec = key.second;
    for(int i=0;i<curr_vec.size()-1;i++){
      for(int j=i+1;j<curr_vec.size();j++){
        common.at(curr_vec.at(i)).at(curr_vec.at(j))++;
      }
    }
  }
  
  for(int i=0;i<n-1;i++){
    for(int j=i+1;j<n;j++){
      if(common.at(i).at(j) > f) boost::add_edge(i,j,g);
    }
  }
  
  std::vector<vertex_desc> mate_map(n);
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
  boost::edmonds_maximum_cardinality_matching(g,
  boost::make_iterator_property_map(mate_map.begin(),
  boost::get(boost::vertex_index, g)));
  int matching_size = boost::matching_size(g,
  boost::make_iterator_property_map(mate_map.begin(),
  boost::get(boost::vertex_index, g)));
 
  if(matching_size == n/2) std::cout<< "not optimal" << std::endl;
  else std::cout<< "optimal" << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}