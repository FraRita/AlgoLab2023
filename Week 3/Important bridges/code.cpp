#include <iostream>
#include <vector>
#include <utility>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;


void resolve_case(){
  int n,m;
  std::cin >> n >> m;
  graph country(n);
  
  for(int i=0;i<m;i++){
    int a,b;
    std::cin>> a >> b;
    boost::add_edge(a,b,country);
  }
  
  std::map<edge_desc,int> bcc_edges;
  boost::biconnected_components(country,boost::make_assoc_property_map(bcc_edges));
  
  std::vector<std::vector<edge_desc>> components(m,std::vector<edge_desc>(0)); 
  for(auto elem : bcc_edges){
    components.at(elem.second).push_back(elem.first);
  }
  
  std::vector<std::pair<int,int>> crit;
  for(int i=0;i<components.size();i++){
    if(components.at(i).size() == 1){
      int a = boost::source(components.at(i).at(0),country);
      int b = boost::target(components.at(i).at(0),country);
      if(a<b) crit.push_back({a,b});
      else crit.push_back({b,a});
    }
  }
  
  std::sort(crit.begin(),crit.end());
  
  std::cout<< crit.size() << "\n";
  for(int i = 0;i<crit.size();i++){
    std::cout<< crit.at(i).first << " " << crit.at(i).second << std::endl;
  }

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}