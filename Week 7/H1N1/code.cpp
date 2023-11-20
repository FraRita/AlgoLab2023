///2
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
typedef CGAL::Exact_predicates_inexact_constructions_kernel    K;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<double, K>   Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef Triangulation::Finite_faces_iterator                   Fi;
typedef Triangulation::Face_handle                             Fh;


void resolve_case(int n){
  std::vector<K::Point_2> points;
  int x,y;
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    points.push_back(K::Point_2(x,y));
  }
  
  Triangulation t;
  t.insert(points.begin(),points.end());
  
  std::priority_queue<std::pair<double,Fh>> faces;
  double info;
  bool flag;
  Fh face;
  for(Fi fi = t.finite_faces_begin(); fi != t.finite_faces_end(); fi++){
    face = fi;
    info = 0;
    flag = false;
    for(int i=0;i<3;i++) if(t.is_infinite(face -> neighbor(i))){
      flag = true;
      info = std::max(info,CGAL::squared_distance(face -> vertex((i+1)%3) -> point(), face -> vertex((i+2)%3) -> point()));
    }
    if(flag)  faces.push(std::make_pair(info,face));
    face -> info() = -1;
    
  }
  
  std::pair<double,Fh> elem;
  Fh neighbor;
  while(!faces.empty()){
    elem = faces.top();
    faces.pop();
    face = elem.second;
    info = elem.first;
    face -> info() = std::max(info,face -> info());
    for(int i=0;i<3;i++){
      neighbor = face -> neighbor(i);
      if(!t.is_infinite(neighbor) && neighbor -> info() == -1){
        faces.push(std::make_pair(std::min(face -> info(), CGAL::squared_distance(face -> vertex((i+1)%3) -> point(), face -> vertex((i+2)%3) -> point())), neighbor));
      }
    }
  }
  
  int m;
  std::cin >> m;
  K::Point_2 point;
  Triangulation::Vertex_handle vh,vh1;
  double d;
  for(int i=0;i<m;i++){
    std::cin >> x >> y >> d;
    point = K::Point_2(x,y);
    vh = t.nearest_vertex(point);
    if(CGAL::squared_distance(point, vh -> point()) < d){
      std::cout<< "n";
      continue;
    }
    d *= 4;
    face = t.locate(point);
    
    if(t.is_infinite(face) || face -> info() >= d) std::cout << "y";
    else std::cout << "n";
  }
  std::cout<< std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n;
  std::cin >> n;
  while(n>0){
    resolve_case(n);
    std::cin >> n;
  }
}