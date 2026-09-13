// SevenuP.cpp
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// Init - code
//
// Equivalent to main() in wxWindows. Creates the frame.
//
// Copyright (C) 2002-2007  Jaime Tejedor Gomez, aka Metalbrain
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA



#include "wx/wxprec.h"
#include "wx/intl.h"

#ifndef WX_PRECOMP
  #include "wx/wx.h"
#endif

#include "SevenuP.h"
#include "TheFrame.h"

IMPLEMENT_APP(SevenuP)          // Here starts the wxWindows application

bool SevenuP::OnInit()          // This is executed at startup
{
        TheFrame *frame=new TheFrame(-1, -1, -1, -1, argc, argv);
                                // Create the Frame with this title, at
                                // default position with default size
        frame->Show(TRUE);      // Show it
        SetTopWindow(frame);    // Set it on top
        return true;            // Init was ok
}
