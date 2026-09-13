// SP_Char.h
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// SP_Char - the Character class and functions
//
//  The character class only represents a speccy character, with
// its 8 bytes and its attribute. You can set or get any single
// part of that character: pixel values, byte values, ink, paper
// bright, flash or attributes as a whole.
//
//
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



#ifndef SP_CHAR_H
#define SP_CHAR_H

#include "Globals.h"

class SP_Char
{

BYTE SP_Char_Pixels[8];         // The eight bytes of the char
BYTE SP_Char_Attr;              // The attribute byte


public:

//Constructor
SP_Char()
        {
        for(int i=0;i<8;i++)
                SP_Char_Pixels[i]=0;
        SP_Char_Attr=56;        // White paper and black ink by default
        }

//Pixel Setting functions

void SetPixel(int xx,int yy)    // Sets pixel xx, yy
        {SP_Char_Pixels[(yy%8)]|=(1<<(xx%8));}
void ResetPixel(int xx,int yy)  // Resets pixel xx, yy
        {SP_Char_Pixels[(yy%8)]&=(255-(1<<(xx%8)));}
void TogglePixel(int xx,int yy) // Toggle value of pixel xx, yy
        {SP_Char_Pixels[(yy%8)]^=(1<<(xx%8));}

//Colour Setting functions

void SetInk(int k)              // Sets ink value
        {SP_Char_Attr=(SP_Char_Attr & 248)+(k%8);}
void SetPaper(int k)            // Sets paper value
        {SP_Char_Attr=(SP_Char_Attr & 199)+((k%8)<<3);}
void SetBright(int k)           // Sets bright value
        {SP_Char_Attr=(SP_Char_Attr & 191)+((k%2)<<6);}
void SetFlash(int k)            // Sets flash value
        {SP_Char_Attr=(SP_Char_Attr & 127)+((k%2)<<7);}

//Bytes Setting functions

void SetByte(int k, BYTE b)     // Sets byte k of char to value b
        {SP_Char_Pixels[k%8]=b;}
void SetAttr(BYTE b)            // Sets attribute to value b
        {SP_Char_Attr=b;}

//Pixel Getting function

int GetPixel(int xx,int yy)     // Gets bit xx, yy value
        {return ((SP_Char_Pixels[(yy%8)]>>(xx%8))&1);}

//Colour Getting functions

int GetInk()                    // Gets ink value
        {return (SP_Char_Attr%8);}
int GetPaper()                  // Gets paper value
        {return ((SP_Char_Attr%64)>>3);}
int GetBright()                 // Gets bright value
        {return ((SP_Char_Attr%128)>>6);}
int GetFlash()                  // Gets flash value
        {return (SP_Char_Attr>>7);}

//Byte Getting functions

BYTE GetByte(int k)             // Gets value of byte k
        {return SP_Char_Pixels[k%8];}
BYTE GetAttr()                  // Gets attribute value
        {return SP_Char_Attr;}
};
#endif
