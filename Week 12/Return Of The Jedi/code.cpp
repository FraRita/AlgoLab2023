///
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <climits>
#include <functional>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::undirectedS,boost::no_property,boost::no_property> graph;


typedef std::pair<int,int> E;
typedef std::tuple<int,int,int> Edge;

void resolve_case(){
  int n,t;
  std::cin>> n >> t;
  t--;
  
  std::vector<std::vector<E>> edges(n);
  std::vector<bool> vis(n,false);
  std::priority_queue<Edge,std::vector<Edge>,std::function<bool(Edge,Edge)>> reachable([](Edge e1, Edge e2){return (std::get<2>(e1) > std::get<2>(e2) || ( std::get<2>(e1) == std::get<2>(e2) && std::get<1>(e1) < std::get<1>(e2) ));});
  
  int w;
  for(int i=0;i<n-1;i++){
    for(int j=0;j<n-1-i;j++){
      std::cin>> w;
      edges[i].push_back({i+j+1,w});
      edges[i+j+1].push_back({i,w});
    }
  }
  
  for(E edge : edges[t]) reachable.push({t,edge.first,edge.second});
  std::vector<Edge> mst(n);
  vis[t] = true;
  mst[t] = {0,0,0};
  int v,n_reach = n-1;
  while(n_reach > 0){
    while(vis[std::get<1>(reachable.top())]) reachable.pop();
    v = std::get<1>(reachable.top());
    mst[v] = reachable.top();
    reachable.pop();
    for(E edge : edges[v]) reachable.push({v,edge.first,edge.second});
    vis[v] = true;
    n_reach--;
  }

  graph G(n);
  long res = 0;
  int diff = INT_MAX;
  for(int i=0;i<n;i++){
    if(i == t) continue;
    boost::add_edge(std::get<0>(mst[i]),std::get<1>(mst[i]),G);
    res += std::get<2>(mst[i]);
  }
  
  std::vector<Edge> non_mst;
  for(int i=0;i<n;i++){
    for(int j=0;j<n-1;j++){
      if(std::make_tuple(i,edges[i][j].first,edges[i][j].second) != mst[edges[i][j].first] && std::make_tuple(edges[i][j].first,i,edges[i][j].second) != mst[i]) non_mst.push_back({i,edges[i][j].first,edges[i][j].second});
    }
  }
  std::sort(non_mst.begin(),non_mst.end(),[](Edge e1,Edge e2){return (std::get<2>(e1)<std::get<2>(e2));});
  
  std::vector<int> components(n);
  for(int i=0;i<n;i++){
    if(i == t) continue;
    boost::remove_edge(std::get<0>(mst[i]),std::get<1>(mst[i]),G);
    boost::connected_components(G,&components[0]);
    int j=0;
    while(components[std::get<0>(non_mst[j])] == components[std::get<1>(non_mst[j])]) j++;
    diff = std::min(diff,std::get<2>(non_mst[j])-std::get<2>(mst[i]));
    boost::add_edge(std::get<0>(mst[i]),std::get<1>(mst[i]),G);
  }
  
  res += diff;
  
  std::cout << res << std::endl;
  
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin>> t;
  for(int i=0;i<t;i++) resolve_case();
}