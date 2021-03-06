/**
   \file  Robots/LoBot/control/LoMetlogLoader.C
   \brief This file defines the non-inline member functions of the
   lobot::MetlogLoader class.
*/

// //////////////////////////////////////////////////////////////////// //
// The iLab Neuromorphic Vision C++ Toolkit - Copyright (C) 2000-2005   //
// by the University of Southern California (USC) and the iLab at USC.  //
// See http://iLab.usc.edu for information about this project.          //
// //////////////////////////////////////////////////////////////////// //
// Major portions of the iLab Neuromorphic Vision Toolkit are protected //
// under the U.S. patent ``Computation of Intrinsic Perceptual Saliency //
// in Visual Environments, and Applications'' by Christof Koch and      //
// Laurent Itti, California Institute of Technology, 2001 (patent       //
// pending; application number 09/912,225 filed July 23, 2001; see      //
// http://pair.uspto.gov/cgi-bin/final/home.pl for current status).     //
// //////////////////////////////////////////////////////////////////// //
// This file is part of the iLab Neuromorphic Vision C++ Toolkit.       //
//                                                                      //
// The iLab Neuromorphic Vision C++ Toolkit is free software; you can   //
// redistribute it and/or modify it under the terms of the GNU General  //
// Public License as published by the Free Software Foundation; either  //
// version 2 of the License, or (at your option) any later version.     //
//                                                                      //
// The iLab Neuromorphic Vision C++ Toolkit is distributed in the hope  //
// that it will be useful, but WITHOUT ANY WARRANTY; without even the   //
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      //
// PURPOSE.  See the GNU General Public License for more details.       //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with the iLab Neuromorphic Vision C++ Toolkit; if not, write   //
// to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,   //
// Boston, MA 02111-1307 USA.                                           //
// //////////////////////////////////////////////////////////////////// //
//
// Primary maintainer for this file: mviswana usc edu
// $HeadURL: svn://isvn.usc.edu/software/invt/trunk/saliency/src/Robots/LoBot/metlog/LoMetlogLoader.C $
// $Id: LoMetlogLoader.C 13930 2010-09-13 23:53:53Z mviswana $
//

//------------------------------ HEADERS --------------------------------

// lobot headers
#include "Robots/LoBot/metlog/LoMetlogLoader.H"
#include "Robots/LoBot/metlog/LoDataset.H"
#include "Robots/LoBot/metlog/LoExperiment.H"
#include "Robots/LoBot/metlog/LoMetlogList.H"

// DEVNOTE: We must supply the definition of YY_EXTRA_TYPE prior to
// including LoMetlogParser.H, which is generated by flex. Otherwise, the
// generated header will define YY_EXTRA_TYPE as void* and the linker
// will fail to find the lomet_parser_set_extra function, which will end
// up being declared in this module as taking a void* rather than a
// lobot::Experiment* as it actually does.
//
// Moreover, we must include the flex-generated LoMetlogParser.H header
// after LoExperiment.H. Otherwise, the lobot::Experiment class will not
// be properly declared or defined and the compiler really detests that.
#define YY_EXTRA_TYPE lobot::Experiment*
#include "Robots/LoBot/metlog/LoMetlogParser.H"

#include "Robots/LoBot/thread/LoMutex.H"
#include "Robots/LoBot/util/LoString.H"

// Standard C++ headers
#include <iostream>
#include <string>

// Standard C headers
#include <stdio.h>

//----------------------------- NAMESPACE -------------------------------

namespace lobot {

//-------------------------- INITIALIZATION -----------------------------

MetlogLoader* MetlogLoader::create(const MetlogList& L, Dataset* D)
{
   return new MetlogLoader(L, D) ;
}

MetlogLoader::MetlogLoader(const MetlogList& L, Dataset* D)
   : m_logs(L), m_dataset(D)
{
   static int n ;
   static Mutex m ;

   AutoMutex M(m) ;
   start(std::string("lomet_metlog_loader_") + to_string(++n)) ;
}

//-------------------------- METLOG PARSING -----------------------------

void MetlogLoader::run()
{
   yyscan_t parser ;
   try
   {
      lomet_parser_lex_init(&parser) ;
      for(;;)
      {
         std::string name = m_logs.next() ;
         FILE* file = fopen(name.c_str(), "r") ;
         if (! file) {
            std::cerr << Thread::name() << ": "
                      << name << ": unable to open\n" ;
            continue ;
         }

         Experiment* experiment = Experiment::create(name) ;

         lomet_parser_set_extra(experiment, parser) ;
         lomet_parser_set_in(file, parser) ;
         lomet_parser_lex(parser) ;

         lomet_parser_set_extra(0, parser) ;
         lomet_parser_set_in(stdin, parser) ;
         fclose(file) ;

         m_dataset->add(experiment) ;
      }
   }
   catch (MetlogList::eol&)
   {
      lomet_parser_lex_destroy(parser) ;
   }
}

//-----------------------------------------------------------------------

} // end of namespace encapsulating this file's definitions

/* So things look consistent in everyone's emacs... */
/* Local Variables: */
/* indent-tabs-mode: nil */
/* End: */
