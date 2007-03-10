// Copyright (c) 2003,2004,2005  INRIA Sophia-Antipolis (France) and
// Notre Dame University (U.S.A.).  All rights reserved.
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
// Author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>

#include <CGAL/basic.h>

#ifdef CGAL_USE_QT

#include <iostream>
#include <fstream>

#include <qapplication.h>
#include <qmainwindow.h>

//#include <CGAL/IO/Color.h>
#include <CGAL/IO/Qt_widget.h>
#include <CGAL/IO/Qt_widget_layer.h>
#include <CGAL/IO/Qt_widget_standard_toolbar.h>
#include <CGAL/IO/Qt_widget_get_segment.h>
#include <CGAL/IO/Qt_widget_get_point.h>
#include <CGAL/IO/Qt_widget_get_simple_polygon.h>
#include <CGAL/IO/pixmaps/demoicon.xpm>

#include <CGAL/Timer.h>

#include <list>
#include <vector>

#include "typedefs.h"
#include "sdg_insert.h"
#include "sdg_draw.h"

#include "qt_file_toolbar.h"
#include "qt_layers_toolbar.h"
#include "qt_layers.h"


//************************************
// global variables
//************************************
SDG_2 sdg;
int num_selected;
std::vector<Site> sitelist;

#include "my_window.h"

#include "qt_file_toolbar.moc"
#include "qt_layers_toolbar.moc"
#include "my_window.moc"

int
main(int argc, char* argv[])
{
  int size = 750;

  QApplication app( argc, argv );
  My_Window W(size,size);
  app.setMainWidget( &W );
#if !defined (__POWERPC__)
  QPixmap cgal_icon = QPixmap((const char**)demoicon_xpm);
  W.setIcon(cgal_icon);
#endif
  W.show();
  W.set_window(0,size,0,size);
  W.setCaption( W.get_title() );
  return app.exec();
}


#else

#include <iostream>

int
main(int argc, char* argv[])
{
  std::cerr << "This demo needs CGAL's Qt_widget installed "
            << "in order to run..."
            << std::endl << std::endl;
  return 0;
}


#endif
