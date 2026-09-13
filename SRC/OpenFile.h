// OpenFile.h
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// File management - headers
//
// Creation, opening and saving of graphic files
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


#ifndef OPENFILE_H
#define OPENFILE_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

#include <wx/image.h>
#include <wx/datetime.h>

#include "Globals.h"
#include "SP_Graph.h"

class OpenFile
{
public:
                        // Components
wxString OpenName;                      // File name = filename without .SEV,
                                        //      or NEW01, NEW02...
wxString OpenPath;                      // Complete path and filename

int OpenPropierties;                    // 0 = couldn't open
                                        // 1 = correctly opened file unmasked
                                        // 2 = correctly opened file masked

int OpenFrames;                         // number of frames of the sprite

int flagsure;                           // 0 unmodified, 1 modified

SP_Graph *OpenGraph;                    // Our graphic will be here

                // Functions

OpenFile();                             // Null constructor
OpenFile(int, int, bool, int);          // Constructor for new graphs
OpenFile(std::string);                     // Constructor for graph loading
~OpenFile();                            // Destructor

void Distribute (int* prop,int pixels,int cellsize,int paper,int ink,int* p);
                                // Distribution of inked pixels on import

void Save(std::string);                                    // Save .SEV graphic
void SaveSCR(std::string);                                 // Save .SCR screen
void ExportData(std::string,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int); // Choose...
void SaveBIN(std::string,int,int,int,int,int,int,int,int,int,int,int,int,int); // Save .BIN file
void SaveASM(std::string,int,int,int,int,int,int,int,int,int,int,int,int,int,int,int); // Save .ASM src
void SaveC(std::string,int,int,int,int,int,int,int,int,int,int,int,int,int);   // Save .C source
void Export(std::string,int);                              // Export to image
void ImportBIN(std::string,int,int,int,int,int,int,int,int,int,int,int,int); // Import from binary file
};
#endif // OPENFILE_H
