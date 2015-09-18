// Copyright (c) 2005  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
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
// Author(s)     : Baruch Zukerman <baruchzu@post.tau.ac.il>
//                 Efi Fogel <efif@post.tau.ac.il>

#ifndef CGAL_GPS_DEFAULT_DCEL_H
#define CGAL_GPS_DEFAULT_DCEL_H

/*! \file
 * This class is the default \dcel{} class used by the General_polygon_set_2
 * and Polygon_set_2} class-templates to represent the undelying internal
 * Arrangement_2 data structure.
 */

#include <CGAL/Arr_default_dcel.h>
#include <CGAL/Union_find.h>

namespace CGAL {

template <class X_monotone_curve_2>
class Gps_halfedge_base : public Arr_halfedge_base<X_monotone_curve_2>
{
  int flooding_flag;
public:

  typedef Arr_halfedge_base<X_monotone_curve_2> Base;
  Gps_halfedge_base()
    : Base()
    , flooding_flag(-1)
  {}

  bool is_flooding_visited() const { return flooding_flag!=-1; }
  bool is_flooding_on_inner_ccb() const { return flooding_flag==0; }
  bool is_flooding_on_outer_ccb() const { return flooding_flag==1; }
  void set_flooding_on_inner_ccb() {flooding_flag=0;}
  void set_flooding_on_outer_ccb() {flooding_flag=1;}
  void set_flooding_visited() {flooding_flag=2;}
  void set_new_ccb_assigned() {flooding_flag=3;}
  bool is_new_ccb_assigned() { return flooding_flag==3;}
};

template <class Traits>
class Gps_face_base : public Arr_face_base
{
  typedef typename   Arr_dcel_base<Arr_vertex_base<typename Traits::Point_2>,
                                   Gps_halfedge_base<typename Traits::X_monotone_curve_2>,
                                   Gps_face_base<Traits> >::Face_iterator Face_iterator;
  typedef typename Union_find<Face_iterator>::handle UF_handle;
public:
  UF_handle uf_handle;
protected:
  mutable char m_info;
  enum
  {
    CONTAINED = 1,
    VISITED   = 2
  };


public:
  //Constructor
  Gps_face_base() :
    Arr_face_base(),
    uf_handle(NULL),
    m_info(0)
  {}

   /*! Assign from another face. */
  virtual void assign (const Arr_face_base& f)
  {
    Arr_face_base::assign (f);

    const Gps_face_base & ex_f = static_cast<const Gps_face_base&>(f);
    m_info = ex_f.m_info;
  }
  
  bool contained() const
  {
    return (m_info & CONTAINED) != 0;
  }

  void set_contained(bool b)
  {
    if (b)
      m_info |= CONTAINED;
    else
      m_info &= ~CONTAINED;
  }

  bool visited() const
  {
    return (m_info & VISITED) != 0;
  }

  void set_visited(bool b) const
  {
    if (b)
      m_info |= VISITED;
    else
      m_info &= ~VISITED;
  }

};





template <class Traits_>
class Gps_default_dcel :
  public Arr_dcel_base<Arr_vertex_base<typename Traits_::Point_2>,
                       Gps_halfedge_base<typename Traits_::X_monotone_curve_2>,
                       Gps_face_base<Traits_> >
{
public:
  /*! Default constructor. */
  Gps_default_dcel() {}
};



} //namespace CGAL

#endif
