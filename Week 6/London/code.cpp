#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#define AZ 26

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

int letter_count[AZ];
int news_count[AZ][AZ];

void resolve_case(){
  int h,w;
  std::string letter;
  std::cin>> h >> w >> letter;
  
  int n = letter.length();
  memset(letter_count,0,AZ*sizeof(int));
  for(int i=0;i<n;i++) letter_count[letter[i]-'A']++;
  
  std::vector<std::vector<int>> news(h,std::vector<int>(w));
  for(int i=0;i<AZ;i++) memset(news_count[i],0,AZ*sizeof(int));
  char c;
  for(int i=0;i<h;i++){
    for(int j=0;j<w;j++){
      std::cin>> c;
      news[i][j] = c-'A';
    }
  }
  
  for(int i=0;i<h;i++){
    for(int j=w-1;j>=0;j--){
      std::cin>> c;
      news_count[news[i][j]][c-'A']++;
    }
  }
  
  int page = AZ*AZ;
  graph G(page+AZ+2);
  int s = page+AZ,t=page+AZ+1;
  edge_adder adder(G);
  for(int i=0;i<AZ;i++) adder.add_edge(page+i,t,letter_count[i]);
  
  int ver_id=0;
  for(int i=0;i<AZ;i++){
    for(int j=0;j<AZ;j++){
      adder.add_edge(s,ver_id,news_count[i][j]);
      adder.add_edge(ver_id,page+i,news_count[i][j]);
      adder.add_edge(ver_id,page+j,news_count[i][j]);
      ver_id++;
    }
  }
  
  if(n == boost::push_relabel_max_flow(G, s, t)) std::cout << "Yes" << std::endl;
  else std::cout << "No" << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while (t-- > 0) resolve_case();
}