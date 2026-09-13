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
        m_frame=new TheFrame(-1, -1, -1, -1, argc, argv);
                                // Create the Frame with this title, at
                                // default position with default size
        m_frame->Show(TRUE);    // Show it
        SetTopWindow(m_frame);  // Set it on top
        return true;            // Init was ok
}

#ifdef __WXMAC__

// Files opened from the Finder arrive as an Apple Event, not in argv.
// Hand the whole array to OpenArrayFiles at once, the same entry point
// the drag and drop target uses, so the canvas is refreshed only after
// the last file rather than once per file.
void SevenuP::MacOpenFiles(const wxArrayString &fileNames)
{
        if (m_frame==NULL)      // event arrived before the frame exists
                {
                wxApp::MacOpenFiles(fileNames);
                return;
                }
        m_frame->OpenArrayFiles(fileNames);
        m_frame->Raise();
}

#endif
