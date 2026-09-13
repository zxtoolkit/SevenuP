// Globals.h
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// Global definitions
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


#ifndef GLOBALS_H
#define GLOBALS_H

#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int

#define ZOOMLEVELS 13   // Number of Zoom levels
#define ZOOMBITMAPS 3   // Number of Bitmap based zoom levels
#endif


/// Conversion from wxString to string. 
#define         ws2s(as)   (std::string((as).mb_str(wxConvUTF8))) 

/// Conversion from string to wxString. 
#define         s2ws(s)   (wxString((s).c_str(),wxConvUTF8)) 

/// Conversion from C-string to wxString 
#define         cs2ws(s)   (wxString(s,wxConvUTF8)) 

/// Conversion from const* wxChar to string. 
#define         wc2s(as)   (std::string((wxString(as)).mb_str(wxConvUTF8)))
