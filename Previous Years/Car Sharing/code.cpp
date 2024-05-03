#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
#define P 100

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, int capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0;
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;
    w_map[rev_e] = -cost;
  }
};

int find(std::vector<std::tuple<int,int,int,int>> &bookings, int start, int end, int t){
  if(start == end-1){
    if(std::get<0>(bookings[start]) < t) return -1;
    return start;
  }
  int mid = (start+end)/2;
  if(std::get<0>(bookings[mid]) < t) return find(bookings,mid,end,t);
  int res = find(bookings,start,mid,t);
  if(res == -1) return mid;
  return res;
}

void resolve_case(){
  int n,S;
  std::cin>> n >> S;
  
  graph G(n+S+2);
  edge_adder adder(G);
  int s = n+S, t = n+S+1; 
  
  int l,tot_cars = 0;
  for(int i=0;i<S;i++){
    std::cin>> l;
    tot_cars += l;
    adder.add_edge(s,n+i,l,0);
  }

  std::vector<std::vector<std::tuple<int,int,int,int>>> bookings(S);
  int from,to,t1,t2,p,max_t = 0;
  for(int i=0;i<n;i++){
    std::cin>> from >> to >> t1 >> t2 >> p;
    max_t = std::max(max_t,t2);
    bookings[from-1].push_back({t1,t2,to-1,p});
  }
  
  std::vector<int> offset(S+1,0);
  for(int i=0;i<S;i++){
    if(bookings[i].size() == 0){
      offset[i+1] = offset[i];
      adder.add_edge(n+i,t,tot_cars,max_t*P);
      continue;
    }
    std::sort(bookings[i].begin(),bookings[i].end(),[](std::tuple<int,int,int,int> &e1, std::tuple<int,int,int,int> &e2){return (std::get<0>(e1)<std::get<0>(e2));});
    offset[i+1] = offset[i] + bookings[i].size();
    adder.add_edge(n+i,offset[i],tot_cars,std::get<0>(bookings[i][0])*P);
    for(int j=0;j<bookings[i].size()-1;j++) adder.add_edge(offset[i]+j,offset[i]+j+1,tot_cars,(std::get<0>(bookings[i][j+1]) - std::get<0>(bookings[i][j]))*P);
    adder.add_edge(offset[i+1]-1,t,tot_cars,(max_t - std::get<0>(bookings[i].back()))*P);
  }

  for(int i=0;i<S;i++){
    for(int j=0;j<bookings[i].size();j++){
      int dest = std::get<2>(bookings[i][j]);
      if(bookings[dest].size() == 0 || std::get<1>(bookings[i][j]) > std::get<0>(bookings[dest].back())) adder.add_edge(offset[i]+j,t,1,(max_t - std::get<0>(bookings[i][j]))*P - std::get<3>(bookings[i][j]));
      else{
        int k = find(bookings[dest],0,bookings[dest].size(),std::get<1>(bookings[i][j]));
        adder.add_edge(offset[i]+j,offset[dest]+k,1,(std::get<0>(bookings[dest][k]) - std::get<0>(bookings[i][j]))*P - std::get<3>(bookings[i][j]));
      }
    }
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, s, t);
  long cost = boost::find_flow_cost(G);
  
  std::cout<< tot_cars*max_t*P-cost << std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  while(t-- > 0) resolve_case();
}