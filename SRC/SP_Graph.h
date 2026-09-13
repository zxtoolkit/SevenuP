// SP_Graph.h
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// SP_Graph - the Graphic class and functions
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


#ifndef SP_GRAPH_H
#define SP_GRAPH_H

//#include <iostream>          // Only needed for debug purporses

#include "Globals.h"
#include "SP_Char.h"

class SP_Graph
{
int x_size, y_size;             // pixel size of graphic


public:                         // I know it's not good C++ to make elements
                                // of classes public, but it's much easier
                                // for me this way

SP_Char* SP_GRPointer;          // SP_Char's pointer
SP_Graph* Mask;                 // a pointer to another SP_Graph, can be NULL
                                //  or this graphic's mask
SP_Graph* NextGraph;            // a pointer to another SP_Graph, can be NULL
                                //  or next frame of a sprite

wxBitmap *SP_Screen[ZOOMBITMAPS]; // Small zoom bitmaps for the graphic

int x_charsize, y_charsize;     // char size of graphic

int Propied1, Propied2;         // Several uses


SP_Graph(int x_si = 8, int y_si = 8, bool sc=FALSE)  //Constructor
        {
        int ZoomValue2[ZOOMBITMAPS]={1,2,3}; // Zoom bitmap levels
        x_size=x_si;
        y_size=y_si;
        if (x_size>256) x_size=256;
        if (y_size>192) y_size=192;
        Propied1 = 1;
        Propied2 = 0;
        x_charsize=(x_size/8)+(x_size%8!=0);
        y_charsize=(y_size/8)+(y_size%8!=0);
        SP_GRPointer = new SP_Char[x_charsize*y_charsize];
        NextGraph = NULL;
        Mask = NULL;
        for (int op=0;op<ZOOMBITMAPS;op++)
                {
                if (sc)
                        {
                        SP_Screen[op] = new wxBitmap(x_size*ZoomValue2[op],y_size*ZoomValue2[op]);
                        }
                else
                        {
                        SP_Screen[op] = NULL;
                        }
                }
        }

//Destructor
~SP_Graph()
        {
        if (Mask != NULL) delete Mask;
        if (NextGraph != NULL) delete NextGraph;
        for (int op=0;op<ZOOMBITMAPS;op++)
                {
                if (SP_Screen[op] != NULL) delete SP_Screen[op];
                }
        delete [] SP_GRPointer;
        }

//Pixel Setting functions

void SetPixel(int xx,int yy)            // Sets pixel xx, yy
        {SP_GRPointer[x_charsize*(yy/8)+(xx/8)].SetPixel(7-(xx%8),yy%8);}
void ResetPixel(int xx,int yy)          // Resets pixel xx, yy
        {SP_GRPointer[x_charsize*(yy/8)+(xx/8)].ResetPixel(7-(xx%8),yy%8);}
void TogglePixel(int xx,int yy)         // Toggle value of pixel xx, yy
        {SP_GRPointer[x_charsize*(yy/8)+(xx/8)].TogglePixel(7-(xx%8),yy%8);}

//Colour Setting functions

void SetInk(int x, int y, int k)        // Sets ink value of char x,y
        {SP_GRPointer[x_charsize*y+x].SetInk(k);}
void SetPaper(int x, int y, int k)      // Sets paper value of char x,y
        {SP_GRPointer[x_charsize*y+x].SetPaper(k);}
void SetBright(int x, int y, int k)     // Sets bright value of char x,y
        {SP_GRPointer[x_charsize*y+x].SetBright(k);}
void SetFlash(int x, int y, int k)      // Sets flash value of char x,y
        {SP_GRPointer[x_charsize*y+x].SetFlash(k);}

//Bytes Setting functions

void SetByte(int k, BYTE b)     // Sets byte k%8 of char k/8 to value b
        {SP_GRPointer[k/8].SetByte(k%8,b);}
void SetAttr(int k, BYTE b)     // Sets attributes of char k to value b
        {SP_GRPointer[k].SetAttr(b);}

//Pixel Getting functions

int GetPixel(int xx,int yy)     // Gets bit xx, yy value
        {return SP_GRPointer[x_charsize*(yy/8)+(xx/8)].GetPixel(7-(xx%8),yy%8);}
int GetMaskColour(int xx,int yy)        // Gets bit xx, yy mask value
        {return (Mask->GetPixel(xx,yy)*2+GetPixel(xx,yy));}

//Colour Getting functions

int GetPixelColour(int xx,int yy)       // Gets bit xx, yy current colour
        {return (GetPixel(xx,yy)?GetInk(xx/8,yy/8):GetPaper(xx/8,yy/8))+8*GetBright(xx/8,yy/8);}

int GetInk(int x, int y)        // Gets ink value of char x,y
        {return SP_GRPointer[x_charsize*y+x].GetInk();}
int GetPaper(int x, int y)      // Gets paper value of char x,y
        {return SP_GRPointer[x_charsize*y+x].GetPaper();}
int GetBright(int x, int y)     // Gets bright value of char x,y
        {return SP_GRPointer[x_charsize*y+x].GetBright();}
int GetFlash(int x, int y)      // Gets flash value of char x,y
        {return SP_GRPointer[x_charsize*y+x].GetFlash();}

//Byte Getting functions

BYTE GetByte(int k)     // Gets value of byte k%8 of character k/8
        {return SP_GRPointer[k/8].GetByte(k%8);}
BYTE GetByte(int x,int b) // Gets value of byte b (or attributes if b==8)
                          // of character x
        {
        if (b==8)       return GetAttr(x);
        else            return SP_GRPointer[x].GetByte(b);
        }
BYTE GetByteWithAttrAs9(int k)  // Gets byte k%9 of character k/9
                                // (attributes if k%9==8)
        {return GetByte(k/9,k%9);}
BYTE GetAttr(int k)             // Gets attributes of character k
        {return SP_GRPointer[k].GetAttr();}

//Size Getting functions

int GetSizeX()
        {return x_size;}
int GetSizeY()
        {return y_size;}

};
#endif
