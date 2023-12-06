#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <iostream>
#include <utility>
#include <vector>
#include <queue>
typedef CGAL::Exact_predicates_exact_constructions_kernel      K;
typedef CGAL::Triangulation_vertex_base_2<K>                   Vb;
typedef CGAL::Triangulation_face_base_with_info_2<K::FT, K>    Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef Triangulation::Finite_faces_iterator                   Fi;
typedef Triangulation::Face_handle                             Fh;


void resolve_case(){
  int n,m;
  long d;
  std::cin>> n >> m >> d;
  
  std::vector<K::Point_2> points;
  long x,y;
  for(int i=0;i<n;i++){
    std::cin>> x >> y;
    points.push_back(K::Point_2(x,y));
  }
  
  Triangulation t;
  t.insert(points.begin(),points.end());
  
  std::priority_queue<std::pair<K::FT,Fh>> faces;
  K::Point_2 dual;
  for(Fi fi = t.finite_faces_begin(); fi != t.finite_faces_end(); fi++){
    dual = t.dual(fi);
    fi -> info() = CGAL::squared_distance(dual,fi -> vertex(0) -> point())/4;
    for(int i=0;i<3;i++){
      if(t.is_infinite(fi -> neighbor(i))){
        fi -> info() = std::max(fi -> info(),CGAL::squared_distance(fi -> vertex((i+1)%3) -> point(), fi -> vertex((i+2)%3) -> point())/4);
      }
    }
    faces.push({fi->info(),fi});
  }

  Fh face, ne;
  K::FT info;
  while(!faces.empty()){
    if(faces.top().first < faces.top().second -> info()){
      faces.pop();
      continue;
    }
    face = faces.top().second;
    faces.pop();
    for(int i=0;i<3;i++){
      ne = face -> neighbor(i);
      if(t.is_infinite(ne)) continue;
      info = std::min(face -> info(),CGAL::squared_distance(face -> vertex((i+1)%3) -> point(), face -> vertex((i+2)%3) -> point())/4);
      if(info > ne -> info()){
        ne -> info() = info;
        faces.push({info,ne});
      }
    }
  }
  
  long s;
  K::Point_2 p,O(0,0);
  K::FT dist;
  for(int i=0;i<m;i++){
    std::cin >> x >> y >> s;
    dist = CGAL::squared_distance(O,K::Point_2(d+s,0));
    p = K::Point_2(x,y);
    if(CGAL::squared_distance(p,t.nearest_vertex(p) -> point()) < dist){
      std::cout<< "n";
      continue;
    }
    
    face = t.locate(p);
    if(t.is_infinite(face) || face -> info() >= dist) std::cout << "y";
    else std::cout << "n";
  }
  std::cout<<std::endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;
  std::cin >> t;
  while(t-- > 0) resolve_case();
}