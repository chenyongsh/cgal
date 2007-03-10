// Copyright (c) 2003-2004  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with CGAL.
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
// Author(s)     : Laurent Rineau

#include <CGAL/basic.h>

 // if QT is not installed, a message will be issued in runtime.
#ifndef CGAL_USE_QT
#include <iostream>

int main(int, char*)
{

  std::cout << "Sorry, this demo needs QT...";
  std::cout << std::endl;

  return 0;
}

#else

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Filtered_kernel.h>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>

#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/IO/File_poly.h>

typedef CGAL::Simple_cartesian<double> K1;
typedef CGAL::Filtered_kernel<K1> K2;
struct K : public K2 {};

typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds1;

struct Tds : public Tds1 {};

typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds,
  CGAL::Exact_predicates_tag> Tr;

typedef K::Point_2 Point;

void usage(char** argv)
{
  std::cerr << "Usage: " << argv[0]
	    << " [-Q] [-D] [-v] input.poly [output.poly]" << std::endl
	    << "Read a Shewchuk .poly file and output a conforming PSLG."
	    << std::endl
	    << "  -Q   Quiet" << std::endl
	    << "  -D   Make conforming Delaunay, instead of"
    " conforming Gabriel." << std::endl
	    << "  -v   Verbose" << std::endl;
}

int main(int argc, char** argv)
{
  int arg_count = 1;
  bool terminal_output = true;
  bool delaunay = false;
  bool verbose = false;

  if(argc < 2)
    {
      usage(argv);
      return 1;
    }

  while(argv[arg_count][0] == '-' && std::string(argv[arg_count]) != "--")
    {
      if(std::string(argv[arg_count]) == "-Q")
	terminal_output = false;
      else if(std::string(argv[arg_count]) == "-D")
	delaunay = true;
      else if(std::string(argv[arg_count]) == "-v")
	verbose = true;
      else
	{
	  std::cerr << "Unknown option " << argv[arg_count] << std::endl;
	  usage(argv);
	  return 1;
	}
      ++arg_count;
    }
  if(std::string(argv[arg_count]) == "--")
    ++arg_count;

  if(argc < arg_count+1 || argc > arg_count+2)
    {
      usage(argv);
      return 1;
    };
  std::ifstream input(argv[arg_count]);
  if(input)
    {
      Tr t;
      CGAL::read_triangle_poly_file(t, input);
      if(delaunay)
	{
	  if(verbose)
	    std::cerr << "Make conforming Delaunay..." << std::endl;
	  CGAL::make_conforming_Delaunay_2(t);
	}
      else
	{
	  if(verbose)
	    std::cerr << "Make conforming Gabriel..." << std::endl;
	  CGAL::make_conforming_Gabriel_2(t);
	}

      if(argc==arg_count+1)
	{
	  if(terminal_output)
	    CGAL::write_triangle_poly_file(t, std::cout);
	}
      else
	{
	  std::ofstream output(argv[arg_count+1]);
	  CGAL::write_triangle_poly_file(t, output);
	}

      if(terminal_output)
	std::cerr
	  << "Number of points: " << t.number_of_vertices() << std::endl
	  << "Number of triangles: " << t.number_of_faces () << std::endl;

    }
  else
    {
      std::cerr << "Bad file: " << argv[arg_count] << std::endl;
      usage(argv);
      return 1;
    }
  return 0;
};

#endif // CGAL_USE_QT
