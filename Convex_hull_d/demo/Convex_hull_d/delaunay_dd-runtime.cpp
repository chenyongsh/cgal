// Copyright (c) 2001, 2002, 2004  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>

#include <CGAL/basic.h>

#ifdef CGAL_USE_LEDA
#include <CGAL/LEDA_basic.h>
#include <CGAL/Homogeneous_d.h>
#include <CGAL/Delaunay_d.h>
#include <CGAL/random_selection.h>
#include <iostream>

#if CGAL_LEDA_VERSION < 500
#include <LEDA/misc.h>
#else
#include <LEDA/system/misc.h>
#endif
#include <CGAL/leda_integer.h>
typedef leda_integer RT;

typedef CGAL::Homogeneous_d<RT> Kernel;
typedef CGAL::Delaunay_d<Kernel> Delaunay_d;
typedef Delaunay_d::Point_d Point_d;
typedef Delaunay_d::Lifted_hyperplane_d Hyperplane_d;
typedef Delaunay_d::Sphere_d Sphere_d;
typedef Delaunay_d::Simplex_handle Simplex_handle;
typedef Delaunay_d::Vertex_handle Vertex_handle;
typedef Delaunay_d::Facet_handle Facet_handle;


template <class R>
CGAL::Point_d<R> random_point_in_range(int d,int l,int h,
                                       CGAL::Point_d<R>)
{
  std::vector<int> V(d+1); V[d]=1;
  for(int i = 0; i<d; ++i)
    V[i] = CGAL::default_random.get_int(l,h);
  return CGAL::Point_d<R>(d,V.begin(),V.end());
}

template <class R>
void random_points_in_range(int n, int d,int l,int h,
                            std::list< CGAL::Point_d<R> >& L)
{ CGAL::Point_d<R> dummy;
  for(int i = 0; i<n; ++i)
    L.push_back(random_point_in_range(d,l,h,dummy));
}

int main(int argc, char* argv[])
{
  // first param is dimension
  // second param is number of points
  int dimension = 4;
  int n = 100;
  int m = 100;

  if (argc > 1 && leda_string(argv[1])=="-h") {
    std::cout<<"usage: "<<argv[0]<<" [dim] [#points] [max coords]\n";
    return 1;
  }
  if (argc > 1)  dimension = atoi(argv[1]);
  if (argc > 2)  n = atoi(argv[2]);
  if (argc > 3)  m = atoi(argv[2]);

  Delaunay_d T(dimension);
  std::list<Point_d> L;

  random_points_in_range(n,dimension,-m,m,L);
  float ti = CGAL_LEDA_SCOPE::used_time();
  int i=0;
  std::list<Point_d>::iterator it;
  for(it = L.begin(); it!=L.end(); ++it) {
    T.insert(*it); i++;
    if (i%10==0)
      std::cout << i << " points inserted" << std::endl;
  }
  std::cout << "used time for inserts  " << CGAL_LEDA_SCOPE::used_time(ti) << std::endl;
  std::cout << "entering check" << std::endl;
  T.is_valid();
  std::cout << "used time for sanity check  " << CGAL_LEDA_SCOPE::used_time(ti) << std::endl;
  std::cout << "entering nearest neighbor location" << std::endl;
  L.clear();
  random_points_in_range(n/10,dimension,-m,m,L);
  ti = CGAL_LEDA_SCOPE::used_time();
  i = 0;
  for(it = L.begin(); it!=L.end(); ++it) {
    T.nearest_neighbor(*it); i++;
    if (i%10==0) std::cout << i << " points located" << std::endl;
  }
  std::cout << "used time for location  " << CGAL_LEDA_SCOPE::used_time(ti) << std::endl;

  T.print_statistics();
  CGAL_LEDA_SCOPE::print_statistics();
  return 0;
}

#else
#include <iostream>

int main()
{
  std::cout << "this program requires LEDA" << std::endl;
  return 0;
}

#endif // CGAL_USE_LEDA
