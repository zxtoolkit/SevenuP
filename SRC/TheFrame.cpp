// TheFrame.cpp
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// Main Frame - code
//
// Here's where all edition code lies
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

#include "TheFrame.h"

#ifdef __WXMAC__
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define ADD_TOOL(id, bmp, tooltip, help) \
        toolBar->AddTool(id,bmp,wxNullBitmap,FALSE,-1,-1,(wxObject *)0, tooltip,help)

#define ADD_TOOL2(id, bmp, tooltip) \
        toolBar->AddTool(id,bmp,wxNullBitmap,FALSE,-1,-1,(wxObject *)0, tooltip)

// Colors 0-15 Speccy, 16 grid light brown, 17 grid orange, 18-19 mask grid
BYTE RedPal[20]={0,0,200,200,0,0,200,200,0,0,248,248,0,0,248,248,154,238,154,238};
BYTE GreenPal[20]={0,0,0,0,196,196,196,196,0,0,0,0,246,246,246,246,133,174,33,74};
BYTE BluePal[20]={0,200,0,200,0,200,0,200,0,248,0,248,0,248,0,248,64,0,64,0};
BYTE RedPalSel[20]={64,64,220,220,64,64,220,220,64,64,255,255,64,64,255,255,101,17,201,117};
BYTE GreenPalSel[20]={64,64,64,64,216,216,216,216,64,64,64,64,255,255,255,255,122,81,22,1};
BYTE BluePalSel[20]={64,220,64,220,64,220,64,220,64,255,64,255,64,255,64,255,191,255,91,155};

BYTE MaskPal[4]={7,6,0,4}; // .INI modifyable maybe in a future

int ZoomValue[ZOOMLEVELS]={1,2,3,4,6,8,12,16,20,25,32,40,48}; //Zoom levels

// Properties...
        wxString strs4[] = {_("Gfx+Attr"),_("Attr+Gfx"),_("Gfx"),_("Attr")};
        wxString strs5[] = {_("X char"),_("Char line"),_("Y char"),_("Mask"),_("Frame number")};
        wxString strs6[] = {_("Line"),_("Character"),_("Column"),_("Frames"),_("Sprite")};
	
        int priorities_temp[] = {0,1,2,3,4}; // Priorities order while we're
                                           //  on Output priorities dialog
        int disablezoom=0;      // Used to avoid flicker

        int canpaste=0;         // If 0, nothing has been copied to be pasted
        SP_Graph *copypastable; // Temporal storage for copied area
        SP_Graph *tempFill;     // Temporal fill area
        int copypastemask=0;    // Are we copying from mask?

        WORD FillQueue[768];         // Queue used to fill
        int FillQueueHead=0;
        int FillQueueTail=0;
        int FillMaxX,FillMaxY,FillMinX,FillMinY;

        int ValidIni=0;         // Flag to known if SevenuP.ini is valid

// //Time measurement variables (optimization tests only)
//      wxDateTime Time1;
//      wxDateTime Time2;
//      wxTimeSpan TimeDiff;

// Graphic variables
        int startx,starty;
        int sizx,sizy;

        int oldstartx,oldstarty;
        int oldsizx,oldsizy;

        int ssxc=0;                 // Selection char x start
        int ssyc=0;                 // Selection char y start
        int sexc=0;                 // Selection char x end
        int seyc=0;                 // Selection char y end

        int scrollsafe=0;           // Flag to control fast scroll

// ** Conditional compile code: Windows resources are in file SevenuP.rc
// ** Linux resources are loaded here
// ** If you try to compile SevenuP on Macintosh, you may need to change this

#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)

#include "bitmaps/sevenup.xpm"

#include "bitmaps/reset.xpm"
#include "bitmaps/zoomin.xpm"
#include "bitmaps/zoomout.xpm"
#include "bitmaps/save.xpm"

#include "bitmaps/cursor.xpm"

#if defined (__WXMAC__)

// Mac needs to use special ugly bitmaps for now.
#include "bitmaps/mac/bright0.xpm"
#include "bitmaps/mac/bright1.xpm"

#include "bitmaps/mac/color0.xpm"
#include "bitmaps/mac/color1.xpm"
#include "bitmaps/mac/color2.xpm"
#include "bitmaps/mac/color3.xpm"
#include "bitmaps/mac/color4.xpm"
#include "bitmaps/mac/color5.xpm"
#include "bitmaps/mac/color6.xpm"
#include "bitmaps/mac/color7.xpm"
#else
#include "bitmaps/bright0.xpm"
#include "bitmaps/bright1.xpm"

#include "bitmaps/color0.xpm"
#include "bitmaps/color1.xpm"
#include "bitmaps/color2.xpm"
#include "bitmaps/color3.xpm"
#include "bitmaps/color4.xpm"
#include "bitmaps/color5.xpm"
#include "bitmaps/color6.xpm"
#include "bitmaps/color7.xpm"
#endif

#include "bitmaps/transp.xpm"
#include "bitmaps/bright0d.xpm"
#include "bitmaps/bright1d.xpm"
#include "bitmaps/color0d.xpm"
#include "bitmaps/color1d.xpm"
#include "bitmaps/color2d.xpm"
#include "bitmaps/color3d.xpm"
#include "bitmaps/color4d.xpm"
#include "bitmaps/color5d.xpm"
#include "bitmaps/color6d.xpm"
#include "bitmaps/color7d.xpm"
#include "bitmaps/transpd.xpm"

#include "bitmaps/flip_x.xpm"
#include "bitmaps/flip_y.xpm"
#include "bitmaps/inv_bits.xpm"
#include "bitmaps/rotation_right.xpm"
#include "bitmaps/rotation_left.xpm"
#include "bitmaps/rotation_up.xpm"
#include "bitmaps/rotation_down.xpm"
#include "bitmaps/rotation_anticlockwise.xpm"
#include "bitmaps/rotation_clockwise.xpm"

#include "bitmaps/fra_prev.xpm"
#include "bitmaps/fra_next.xpm"
#endif

// ** End of conditional compilation code

// Frame

TheFrame::TheFrame(int xpos, int ypos, int width, int height, int argc, wxChar **argv)
        : wxFrame((wxFrame *) NULL, -1, _T("SevenuP v1.21"), wxPoint(xpos, ypos), wxSize(width, height))
{

// GENERAL VARIABLES

        newfiles=1;             // New files counter
        sselect=0;              // Selection flag

        sel_start_x=0;          // Selection pixel x start
        sel_end_x=0;            // Selection pixel x end
        sel_start_y=0;          // Selection pixel y start
        sel_end_y=0;            // Selection pixel y end

        Files_open=0;           // Number of open files
        Files_current=0;        // Current file opened

        Frame_current=0;        // Selected frame of current file
        Frame_total=0;          // Number of frames of current file

        Undo_total=0;           // Number of undoable steps
        Redo_total=0;           // Number of redoable steps

        GraphUndo=NULL;         // No Undo graphic yet
        GraphRedo=NULL;         // No Redo graphic yet

        bigescreen=wxBitmap(1280,1024); // Big zooms bitmap

// APPLICATION ICON

        SetIcon(wxICON(sevenup));       // Set SevenuP icon

// STATUS BAR

        CreateStatusBar(2);     // 2 subdivisions for the status bar
        SetStatusText(_("SevenuP"),0);

// LOAD CONFIGURATION VARIABLES FROM .INI FILE

        wxChar IniFile[10000];    // Max 10000 char for .ini file
        wxString IniPath,ExePath;
        ExePath=argv[0];
        int searchindxwin=ExePath.rfind(_("\\"));
        int searchindxunix=ExePath.rfind(_("/"));
        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
#ifndef __WXMAC__        
        IniPath=ExePath.Left(pathend+1)+_("SevenuP.ini");
#else
       // Look for the Mac SevenuP.ini in ~/Library/SevenuP/
       IniPath=getenv(_("HOME"));
       IniPath += _("/Library/SevenuP/SevenuP.ini");
#endif
//      int inilen=0;
//        std::ifstream inifile(IniPath.mb_str(wxConvLocal));
        
#ifdef __WXMAC__
        // If we didn't find the mac SevenuP.ini in ~/Library/SevenuP
        // copy the one in SevenuP.app/contents/Resources/ there.
        if (inifile.fail())
            {
                char path[PATH_MAX];
                sprintf(path, _("%s/Library/SevenuP"),getenv(_("HOME")));
                mkdir(path, 0755);
                
                sprintf(path, _("%s/SevenuP.ini"), path);
                FILE* ip = fopen(ExePath.Left(pathend+1)+_("../Resources/SevenuP.ini"), _("r"));
                FILE* op = fopen(path, _("w"));
                
                if(ip && op) 
                    {
                    // Crude copy routine - works well enough though.
                    char ch;
                    while((ch = fgetc(ip)) != EOF)
                        {
                        fputc(ch, op);
                        }
                    
                    fclose(ip);
                    fclose(op);
                    }
                    
                // Try again...
                inifile.clear();
                inifile.open(path);
            }
#endif

//        if (inifile.fail())
//                {
//                SetStatusText(_("SevenuP.ini failed. Using defaults"),0);
//                }
//        else
//                {
//                while ((inilen<9999)&&(!inifile.eof()))
//                        {
//                        char cunicode;
//                        inifile.get(cunicode);
//                        IniFile[inilen++]=(wxChar)cunicode;
//                        }
//                IniFile[inilen++]=';';
//                IniFile[inilen]=0;
//                ValidIni=1;
//                }
        zoom=GetIniValue(IniFile,_("\nZoom="),0,12,5);
        AutoZoom=GetIniValue(IniFile,_("\nAutoZoom="),0,1,0);
        size_wx=GetIniValue(IniFile,_("\nSize_wx="),470,1280,500);
        size_wy=GetIniValue(IniFile,_("\nSize_wy="),260,1024,300);
        cursormode=GetIniValue(IniFile,_("\nCursor_Mode="),0,2,0);

        col_ink=GetIniValue(IniFile,_("\nInk="),0,8,8);
        col_paper=GetIniValue(IniFile,_("\nPaper="),0,8,8);
        col_bright=GetIniValue(IniFile,_("\nBright="),0,2,2);
        col_flash=GetIniValue(IniFile,_("\nFlash="),0,2,2);

        int Prior=GetIniValue(IniFile,_("\nPriorities="),0,43210,01234);
        int n0,n1,n2,n3,n4;
        int u[10];
        n0=Prior/10000;
        n1=(Prior-n0*10000)/1000;
        n2=(Prior-n0*10000-n1*1000)/100;
        n3=(Prior-n0*10000-n1*1000-n2*100)/10;
        n4=Prior-n0*10000-n1*1000-n2*100-n3*10;
        u[n0]=1;
        u[n1]=1;
        u[n2]=1;
        u[n3]=1;
        u[n4]=1;
        if (u[0]+u[1]+u[2]+u[3]+u[4]==5)
                {
                priorities[0] = n0;
                priorities[1] = n1;
                priorities[2] = n2;
                priorities[3] = n3;
                priorities[4] = n4;
                }
        else
                {
                priorities[0] = 0;
                priorities[1] = 1;
                priorities[2] = 2;
                priorities[3] = 3;
                priorities[4] = 4;
                }
        gridpix=GetIniValue(IniFile,_("\nPixel_grid="),0,1,1);
        gridchr=GetIniValue(IniFile,_("\nCharacter_grid="),0,1,1);

        newdefaultx=GetIniValue(IniFile,_("\nNewSize_x="),1,256,32);
        newdefaulty=GetIniValue(IniFile,_("\nNewSize_y="),1,192,32);

        optradioboxdef=GetIniValue(IniFile,_("\nSaveAttrOption="),0,3,0);
	interleave=GetIniValue(IniFile,_("\nInterleave="),0,5,0);
	if (interleave==3) interleave=0;
        maskfirst=GetIniValue(IniFile,_("\nMaskFirst="),0,1,0);
        AttrMask=GetIniValue(IniFile,_("\nAttrMask="),0,1,0);
        AttrMaskInk=GetIniValue(IniFile,_("\nAttrMaskInk="),0,1,0);
        AttrMaskPaper=GetIniValue(IniFile,_("\nAttrMaskPaper="),0,1,0);
        AttrMaskBright=GetIniValue(IniFile,_("\nAttrMaskBright="),0,1,0);
        AttrMaskFlash=GetIniValue(IniFile,_("\nAttrMaskFlash="),0,1,0);
        zigzag=GetIniValue(IniFile,_("\nZigZag="),0,1,0);
        z88dksize=GetIniValue(IniFile,_("\nZ88dksize="),0,1,0);
        nolabel=GetIniValue(IniFile,_("\nNoLabel="),0,1,0);

        appendfile=GetIniValue(IniFile,_("\nAppend="),0,1,0);
        asmtype=GetIniValue(IniFile,_("\nAsmType="),0,7,0);
	defaultexport=GetIniValue(IniFile,_("\nDefaultExport="),0,2,0);

        effradioboxdef=GetIniValue(IniFile,_("\nEff_pixattr="),0,2,0);
        eff_arr_shift=GetIniValue(IniFile,_("\nEff_arr_shift="),0,1,0);
        eff_arr_char=GetIniValue(IniFile,_("\nEff_arr_char="),0,1,0);

        RememberOpenFilePath=GetIniValue(IniFile,_("\nRememberOpenFilePath="),0,1,1);
        RememberImportImagePath=GetIniValue(IniFile,_("\nRememberImportImagePath="),0,1,1);
        RememberSavePath=GetIniValue(IniFile,_("\nRememberSavePath="),0,1,1);
        RememberExportDataPath=GetIniValue(IniFile,_("\nRememberExportDataPath="),0,1,1);
        RememberExportImagePath=GetIniValue(IniFile,_("\nRememberExportImagePath="),0,1,1);

        OpenFilePath=GetIniValueS(IniFile,_("\nOpenFilePath="),_("."));
        ImportImagePath=GetIniValueS(IniFile,_("\nImportImagePath="),_("."));
        SavePath=GetIniValueS(IniFile,_("\nSavePath="),_("."));
        ExportDataPath=GetIniValueS(IniFile,_("\nExportDataPath="),_("."));
        ExportImagePath=GetIniValueS(IniFile,_("\nExportImagePath="),_("."));

        Warn_closefile=GetIniValue(IniFile,_("\nWarn_closefile="),0,1,1);
        Warn_exitprogram=GetIniValue(IniFile,_("\nWarn_exitprogram="),0,1,1);
        Warn_spriteremove=GetIniValue(IniFile,_("\nWarn_spriteremove="),0,1,1);
        Save_showdialog=GetIniValue(IniFile,_("\nSave_showdialog="),0,1,1);

        oldcursormode=cursormode;// Remember non-paste or fill mode
        switch (cursormode)
                {
                case 0: {
                        left_func=1;    // Function for button 1
                        right_func=2;   // Function for button 2
                        break;
                        }
                case 1: {
                        left_func=3;    // Function for button 1
                        right_func=4;   // Function for button 2
                        break;
                        }
                case 2: {
                        left_func=5;    // Function for button 1
                        right_func=6;   // Function for button 2
                        break;
                        }
                }
        switch (effradioboxdef)
                {
                case 0: {
                        eff_attr=1;
                        eff_pix=1;
                        break;
                        }
                case 1: {
                        eff_attr=0;
                        eff_pix=1;
                        break;
                        }
                case 2: {
                        eff_attr=1;
                        eff_pix=0;
                        break;
                        }
                }

// MENU BAR

        menuBar = new wxMenuBar;

        menuFile = new wxMenu;
         menuFile_SelAsm = new wxMenu;
        menuEdit = new wxMenu;
        menuFill = new wxMenu;
        menuEffects = new wxMenu;
        menuMask = new wxMenu;
        menuSprite = new wxMenu;
        menuGrid = new wxMenu;
        menuZoom = new wxMenu;
        menuInfo = new wxMenu;

        menuFile_SelAsm->Append( FILE_ASMSELECT_0, _("Generic"),_("Several assemblers, decimal data"),TRUE);
        menuFile_SelAsm->Append( FILE_ASMSELECT_1, _("TASM"),_("TASM, decimal data"),TRUE);
        menuFile_SelAsm->Append( FILE_ASMSELECT_2, _("The E-Z80 Way"),_("The E-Z80 Way"),TRUE);
        menuFile_SelAsm->Append( FILE_ASMSELECT_3, _("tniASM"),_("tniASM, decimal data"),TRUE);
        menuFile_SelAsm->AppendSeparator();
        menuFile_SelAsm->Append( FILE_ASMSELECT_4, _("Generic hex (with $)"),_("AS80, ZMAC & Z80ASM, hex data"),TRUE);
        menuFile_SelAsm->Append( FILE_ASMSELECT_5, _("Generic hex (with 0x)"),_("AS80, Z80v4 & Z80-ASM, hex data"),TRUE);
        menuFile_SelAsm->Append( FILE_ASMSELECT_6, _("TASM hex"),_("TASM, hex data"),TRUE);
        menuFile_SelAsm->Append( FILE_ASMSELECT_7, _("tniASM hex"),_("tniASM, hex data"),TRUE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_0+asmtype,TRUE);

        menuFile->Append( FILE_NEW, _("&New\tA"),_("New file"));
        menuFile->Append( FILE_LOAD, _("&Load\tL"),_("Load file"));
        menuFile->Append( FILE_IMPORT, _("&Image Import\tI"),_("Import image"));
        menuFile->Append( FILE_BINIMPORT, _("&Binary Import"),_("Import from binary"));
        menuFile->AppendSeparator();
        menuFile->Append( FILE_SAVE, _("&Save\tS"),_("Save file"));
        menuFile->Append( FILE_FASTSAVE,_("Fast Save\tCTRL-S"),_("Fast save"));
        menuFile->Append( FILE_EXPORTDATA,_("Export &Data\tD"),_("Export to binary or source"));
        menuFile->Append( FILE_EXPORTIMAGE,_("&Export Image\tE"),_("Export to image"));
        menuFile->AppendSeparator();
        menuFile->Append( FILE_CLOSE,_("Close\tCTRL-W"),_("Close file"));
        menuFile->AppendSeparator();
        menuFile->Append( FILE_RENAME,_("Rename\tF2"),_("Rename file"));
        menuFile->AppendSeparator();
        menuFile->Append( FILE_SAVEOPT,_("&Output options\tALT-O"),_("Select save output format"));
        menuFile->Append( FILE_ASMSELECT,_("Assembler output"), menuFile_SelAsm,_("Select target assembler for ASM code"));
        menuFile->AppendSeparator();
        menuFile->Append( FILE_QUIT, _("&Quit\tCTRL-Q"),_("Quit program"));
        menuFile->AppendSeparator();

        menuEdit->Append( EDIT_UNDO,_("Undo\tCtrl-Z"),_("Undo last action"));
        menuEdit->Append( EDIT_REDO,_("Redo\tCtrl-X"),_("Redo last undoed action"));
        menuEdit->AppendSeparator();
        menuEdit->Append( EDIT_COPY,_("Copy\tCtrl-C"),_("Copy selected zone"));
        menuEdit->Append( EDIT_PASTE,_("Paste\tCtrl-V"),_("Paste selected zone"));

        menuFill->Append( FILL_SOLID,_("Fill\tF"),_("Solid Fill"));
        menuFill->Append( FILL_PATTERN,_("Textured Fill\tT"),_("Fill with a copied pattern"));

        menuEffects->Append( EFF_OPT,_("Effects &Options\tALT-E"),_("Set options for effects"));
        menuEffects->AppendSeparator();
        menuEffects->Append( EFF_INV,_("&Invert\tR"),_("Toggle ink & paper"));
        menuEffects->AppendSeparator();
        menuEffects->Append( EFF_FLIP_X,_("&X Flip\tX"),_("Flip graphic horizontally"));
        menuEffects->Append( EFF_FLIP_Y,_("&Y Flip\tY"),_("Flip graphic vertically"));
        menuEffects->AppendSeparator();
        menuEffects->Append( EFF_ROT_CLOCK,_("Clock Rotation\tB"),_("Clockwise 90 degrees rotation"));
        menuEffects->Append( EFF_ROT_ANTICLOCK,_("Anticlock Rotation\tV"),_("Anticlockwise 90 degrees rotation"));
        menuEffects->AppendSeparator();
        menuEffects->Append( EFF_ROT_LEFT,_("Left"),_("Rotate/shift left"));
        menuEffects->Append( EFF_ROT_RIGHT,_("Right"),_("Rotate/shift right"));
        menuEffects->Append( EFF_ROT_UP,_("Up"),_("Rotate/shift up"));
        menuEffects->Append( EFF_ROT_DOWN,_("Down"),_("Rotate/shift down"));

        menuMask->Append( MASK_USE,_("Use mask\tCTRL-M"),_("Toggle use/don't use mask"), TRUE);
        menuMask->Append( MASK_VIEW,_("View mask\tM"),_("Toggle graphic/mask view"), TRUE);
        menuMask->AppendSeparator();
        menuMask->Append( MASK_AUTO,_("Auto mask"),_("Creates a mask around the graphic"));

        menuSprite->Append(SPR_INS_NEXT,_("Insert next frame\tN"),_("Insert next sprite frame"));
        menuSprite->Append(SPR_INS_PREV,_("Insert previous frame\tP"),_("Insert previous sprite frame"));
        menuSprite->Append(SPR_MOVE,_("Move frame"),_("Move current frame to another position"));
        menuSprite->Append(SPR_REMOVE,_("Remove frame"),_("Remove current frame from sprite"));
        menuSprite->AppendSeparator();
        menuSprite->Append(SPR_SEL_PREV,_("Select previous frame\tLEFT"),_("Select previous sprite frame"));
        menuSprite->Append(SPR_SEL_NEXT,_("Select next frame\tRIGHT"),_("Select next sprite frame"));
        menuSprite->Append(SPR_SEL_GOTO,_("Select frame number..."),_("Select frame"));

        menuGrid->Append( GRID_PIXEL,_("&Pixel Grid\tG"),_("Toggle pixel grid"), TRUE);
        menuGrid->Append( GRID_CHAR,_("&Char Grid\tCTRL-G"),_("Toggle character grid"), TRUE);

        menuZoom->Append( ZOOM_1,_("x1"),_("Sets 1x1 zoom"), TRUE);
        menuZoom->Append( ZOOM_2,_("x2"),_("Sets 2x2 zoom"), TRUE);
        menuZoom->Append( ZOOM_3,_("x3"),_("Sets 3x3 zoom"), TRUE);
        menuZoom->Append( ZOOM_4,_("x4"),_("Sets 4x4 zoom"), TRUE);
        menuZoom->Append( ZOOM_5,_("x6"),_("Sets 6x6 zoom"), TRUE);
        menuZoom->Append( ZOOM_6,_("x8"),_("Sets 8x8 zoom"), TRUE);
        menuZoom->Append( ZOOM_7,_("x12"),_("Sets 12x12 zoom"), TRUE);
        menuZoom->Append( ZOOM_8,_("x16"),_("Sets 16x16 zoom"), TRUE);
        menuZoom->Append( ZOOM_9,_("x20"),_("Sets 20x20 zoom"), TRUE);
        menuZoom->Append( ZOOM_10,_("x25"),_("Sets 25x25 zoom"), TRUE);
        menuZoom->Append( ZOOM_11,_("x32"),_("Sets 32x32 zoom"), TRUE);
        menuZoom->Append( ZOOM_12,_("x40"),_("Sets 40x40 zoom"), TRUE);
        menuZoom->Append( ZOOM_13,_("x48"),_("Sets 48x48 zoom"), TRUE);
        menuZoom->AppendSeparator();
        menuZoom->Append( ZOOM_AUTO,_("AutoZoom"),_("Toggle Autozoom"), TRUE);

        menuInfo->Append( INFO_ABOUT,_("&About\tCTRL-A"),_("Info about program"));

        menuBar->Append( menuFile,_("&File"));
        menuBar->Append( menuEdit,_("&Edit"));
        menuBar->Append( menuFill,_("Fi&ll"));
        menuBar->Append( menuEffects,_("Effe&cts"));
        menuBar->Append( menuMask,_("&Mask"));
        menuBar->Append( menuSprite,_("&Sprite"));
        menuBar->Append( menuGrid,_("&Grid"));
        menuBar->Append( menuZoom,_("&Zoom"));
        menuBar->Append( menuInfo,_("&Info"));

        menuBar->Check(GRID_PIXEL,gridpix);
        menuBar->Check(GRID_CHAR,gridchr);
        menuBar->Check(ZOOM_1+zoom,TRUE);
        menuBar->Check(ZOOM_AUTO,AutoZoom);

        SetMenuBar(menuBar);

// TOOLBAR

        wxBitmap tbBitmaps[16];

        tbBitmaps[0] = wxBITMAP(erase);
        tbBitmaps[1] = wxBITMAP(save);
        tbBitmaps[2] = wxBITMAP(zoomin);
        tbBitmaps[3] = wxBITMAP(zoomout);
        tbBitmaps[4] = wxBITMAP(cursor);
        tbBitmaps[5] = wxBITMAP(flip_x);
        tbBitmaps[6] = wxBITMAP(flip_y);
        tbBitmaps[7] = wxBITMAP(inv_bits);
        tbBitmaps[8] = wxBITMAP(rotation_left);
        tbBitmaps[9] = wxBITMAP(rotation_right);
        tbBitmaps[10] = wxBITMAP(rotation_up);
        tbBitmaps[11] = wxBITMAP(rotation_down);
        tbBitmaps[12] = wxBITMAP(rotation_clockwise);
        tbBitmaps[13] = wxBITMAP(rotation_anticlockwise);
        tbBitmaps[14] = wxBITMAP(fra_prev);
        tbBitmaps[15] = wxBITMAP(fra_next);

        toolBar = CreateToolBar();
        toolBar->SetMargins(5,5);
        toolBar->SetToolBitmapSize(wxSize(16,16));

        ADD_TOOL(FILE_SAVE, tbBitmaps[1], _("Save as"), _("Save graphic"));
        toolBar->AddSeparator();
        ADD_TOOL(ZOOM_ZOOMIN, tbBitmaps[2], _("Zoom in"), _("Increase Zoom"));
        ADD_TOOL(ZOOM_ZOOMOUT, tbBitmaps[3], _("Zoom out"), _("Decrease Zoom"));
        toolBar->AddSeparator();
        ADD_TOOL2(TOOLBAR_CHANGE, tbBitmaps[4], _("Change cursor mode"));
        toolBar->AddSeparator();
        ADD_TOOL(EFF_INV, tbBitmaps[7], _("Invert"),_("Invert"));
        toolBar->AddSeparator();
        ADD_TOOL(EFF_FLIP_X, tbBitmaps[5], _("X Flip"),_("Flip horizontally"));
        ADD_TOOL(EFF_FLIP_Y, tbBitmaps[6], _("Y Flip"),_("Flip vertically"));
        toolBar->AddSeparator();
        ADD_TOOL(EFF_ROT_CLOCK, tbBitmaps[12], _("Clock"),_("Rotate 90 degrees clockwise"));
        ADD_TOOL(EFF_ROT_ANTICLOCK, tbBitmaps[13], _("Anticlock"),_("Rotate 90 degrees anticlockwise"));
        toolBar->AddSeparator();
        ADD_TOOL(EFF_ROT_LEFT, tbBitmaps[8], _("Left"),_("Rotate/shift left"));
        ADD_TOOL(EFF_ROT_RIGHT, tbBitmaps[9], _("Right"),_("Rotate/shift right"));
        ADD_TOOL(EFF_ROT_UP, tbBitmaps[10], _("Up"),_("Rotate/shift up"));
        ADD_TOOL(EFF_ROT_DOWN, tbBitmaps[11], _("Down"),_("Rotate/shift down"));
        toolBar->AddSeparator();
        ADD_TOOL(SPR_SEL_PREV, tbBitmaps[14],_("Prev. Frame"),_("Select previous sprite frame"));
        ADD_TOOL(SPR_SEL_NEXT, tbBitmaps[15],_("Next Frame"),_("Select next sprite frame"));
        toolBar->AddSeparator();
        ADD_TOOL(TOOLBAR_ERASE, tbBitmaps[0], _("Erase"), _("Erase"));

        toolBar->Realize();

// MAIN FRAME

        wxBitmap coBitmaps[11];
        wxBitmap coBitmapsd[11];
        int depth = ::wxDisplayDepth();
        for(int i = 0; i < 10; ++i) {
            coBitmaps[i].SetDepth(depth);
            coBitmapsd[i].SetDepth(depth);
        }

        coBitmaps[0] = wxBITMAP(color0);
        coBitmaps[1] = wxBITMAP(color1);
        coBitmaps[2] = wxBITMAP(color2);
        coBitmaps[3] = wxBITMAP(color3);
        coBitmaps[4] = wxBITMAP(color4);
        coBitmaps[5] = wxBITMAP(color5);
        coBitmaps[6] = wxBITMAP(color6);
        coBitmaps[7] = wxBITMAP(color7);
        coBitmaps[8] = wxBITMAP(bright0);
        coBitmaps[9] = wxBITMAP(bright1);
        coBitmaps[10] = wxBITMAP(transp);

        coBitmapsd[0] = wxBITMAP(color0d);
        coBitmapsd[1] = wxBITMAP(color1d);
        coBitmapsd[2] = wxBITMAP(color2d);
        coBitmapsd[3] = wxBITMAP(color3d);
        coBitmapsd[4] = wxBITMAP(color4d);
        coBitmapsd[5] = wxBITMAP(color5d);
        coBitmapsd[6] = wxBITMAP(color6d);
        coBitmapsd[7] = wxBITMAP(color7d);
        coBitmapsd[8] = wxBITMAP(bright0d);
        coBitmapsd[9] = wxBITMAP(bright1d);
        coBitmapsd[10] = wxBITMAP(transpd);

        mainbox = new wxBoxSizer( wxVERTICAL );
        canvas = new wxScrolledWindow( this, ID_PANEL, wxDefaultPosition, wxSize(size_wx,size_wy), wxSUNKEN_BORDER );
        canvas2 = new MyWindow(canvas,0,0,size_wx,size_wy,wxRETAINED);

        boxink = new wxBoxSizer( wxHORIZONTAL );
        boxinktext = new wxStaticText( this, ID_TEXT, _("INK:    "), wxDefaultPosition, wxDefaultSize, 0 );
        Butt_ink[0] = new wxBitmapButton( this, ID_BITMAPBUTTONI0, coBitmaps[0], wxDefaultPosition, wxDefaultSize );
        Butt_ink[1] = new wxBitmapButton( this, ID_BITMAPBUTTONI1, coBitmaps[1], wxDefaultPosition, wxDefaultSize );
        Butt_ink[2] = new wxBitmapButton( this, ID_BITMAPBUTTONI2, coBitmaps[2], wxDefaultPosition, wxDefaultSize );
        Butt_ink[3] = new wxBitmapButton( this, ID_BITMAPBUTTONI3, coBitmaps[3], wxDefaultPosition, wxDefaultSize );
        Butt_ink[4] = new wxBitmapButton( this, ID_BITMAPBUTTONI4, coBitmaps[4], wxDefaultPosition, wxDefaultSize );
        Butt_ink[5] = new wxBitmapButton( this, ID_BITMAPBUTTONI5, coBitmaps[5], wxDefaultPosition, wxDefaultSize );
        Butt_ink[6] = new wxBitmapButton( this, ID_BITMAPBUTTONI6, coBitmaps[6], wxDefaultPosition, wxDefaultSize );
        Butt_ink[7] = new wxBitmapButton( this, ID_BITMAPBUTTONI7, coBitmaps[7], wxDefaultPosition, wxDefaultSize );
        Butt_ink[8] = new wxBitmapButton( this, ID_BITMAPBUTTONIT, coBitmaps[10], wxDefaultPosition, wxDefaultSize );
        Butt_ink[0]->SetBitmapDisabled(coBitmapsd[0]);
        Butt_ink[1]->SetBitmapDisabled(coBitmapsd[1]);
        Butt_ink[2]->SetBitmapDisabled(coBitmapsd[2]);
        Butt_ink[3]->SetBitmapDisabled(coBitmapsd[3]);
        Butt_ink[4]->SetBitmapDisabled(coBitmapsd[4]);
        Butt_ink[5]->SetBitmapDisabled(coBitmapsd[5]);
        Butt_ink[6]->SetBitmapDisabled(coBitmapsd[6]);
        Butt_ink[7]->SetBitmapDisabled(coBitmapsd[7]);
        Butt_ink[8]->SetBitmapDisabled(coBitmapsd[10]);
        boxpaper = new wxBoxSizer( wxHORIZONTAL );
        boxpapertext = new wxStaticText( this, ID_TEXT, _("PAPER:"), wxDefaultPosition, wxDefaultSize, 0 );
        Butt_paper[0] = new wxBitmapButton( this, ID_BITMAPBUTTONP0, coBitmaps[0], wxDefaultPosition, wxDefaultSize );
        Butt_paper[1] = new wxBitmapButton( this, ID_BITMAPBUTTONP1, coBitmaps[1], wxDefaultPosition, wxDefaultSize );
        Butt_paper[2] = new wxBitmapButton( this, ID_BITMAPBUTTONP2, coBitmaps[2], wxDefaultPosition, wxDefaultSize );
        Butt_paper[3] = new wxBitmapButton( this, ID_BITMAPBUTTONP3, coBitmaps[3], wxDefaultPosition, wxDefaultSize );
        Butt_paper[4] = new wxBitmapButton( this, ID_BITMAPBUTTONP4, coBitmaps[4], wxDefaultPosition, wxDefaultSize );
        Butt_paper[5] = new wxBitmapButton( this, ID_BITMAPBUTTONP5, coBitmaps[5], wxDefaultPosition, wxDefaultSize );
        Butt_paper[6] = new wxBitmapButton( this, ID_BITMAPBUTTONP6, coBitmaps[6], wxDefaultPosition, wxDefaultSize );
        Butt_paper[7] = new wxBitmapButton( this, ID_BITMAPBUTTONP7, coBitmaps[7], wxDefaultPosition, wxDefaultSize );
        Butt_paper[8] = new wxBitmapButton( this, ID_BITMAPBUTTONPT, coBitmaps[10], wxDefaultPosition, wxDefaultSize );
        Butt_paper[0]->SetBitmapDisabled(coBitmapsd[0]);
        Butt_paper[1]->SetBitmapDisabled(coBitmapsd[1]);
        Butt_paper[2]->SetBitmapDisabled(coBitmapsd[2]);
        Butt_paper[3]->SetBitmapDisabled(coBitmapsd[3]);
        Butt_paper[4]->SetBitmapDisabled(coBitmapsd[4]);
        Butt_paper[5]->SetBitmapDisabled(coBitmapsd[5]);
        Butt_paper[6]->SetBitmapDisabled(coBitmapsd[6]);
        Butt_paper[7]->SetBitmapDisabled(coBitmapsd[7]);
        Butt_paper[8]->SetBitmapDisabled(coBitmapsd[10]);
        boxbriflatext1 = new wxStaticText( this, ID_TEXT,_(" BRIGHT: "), wxDefaultPosition, wxDefaultSize, 0 );
        Butt_bright[0] = new wxBitmapButton( this, ID_BITMAPBUTTONB0, coBitmaps[8], wxDefaultPosition, wxDefaultSize );
        Butt_bright[1] = new wxBitmapButton( this, ID_BITMAPBUTTONB1, coBitmaps[9], wxDefaultPosition, wxDefaultSize );
        Butt_bright[2] = new wxBitmapButton( this, ID_BITMAPBUTTONBT, coBitmaps[10], wxDefaultPosition, wxDefaultSize );
        Butt_bright[0]->SetBitmapDisabled(coBitmapsd[8]);
        Butt_bright[1]->SetBitmapDisabled(coBitmapsd[9]);
        Butt_bright[2]->SetBitmapDisabled(coBitmapsd[10]);
        boxbriflatext2 = new wxStaticText( this, ID_TEXT,_("  FLASH:"), wxDefaultPosition, wxDefaultSize, 0 );
        Butt_flash[0] = new wxBitmapButton( this, ID_BITMAPBUTTONF0, coBitmaps[8], wxDefaultPosition, wxDefaultSize );
        Butt_flash[1] = new wxBitmapButton( this, ID_BITMAPBUTTONF1, coBitmaps[9], wxDefaultPosition, wxDefaultSize );
        Butt_flash[2] = new wxBitmapButton( this, ID_BITMAPBUTTONFT, coBitmaps[10], wxDefaultPosition, wxDefaultSize );
        Butt_flash[0]->SetBitmapDisabled(coBitmapsd[8]);
        Butt_flash[1]->SetBitmapDisabled(coBitmapsd[9]);
        Butt_flash[2]->SetBitmapDisabled(coBitmapsd[10]);
        mainbox->Add( canvas, 1, wxGROW|wxALL, 2 );
        boxink->Add( boxinktext, 0, wxALIGN_CENTRE|wxALL, 2 );
        boxink->Add( Butt_ink[0], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[1], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[2], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[3], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[4], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[5], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[6], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[7], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_ink[8], 0, wxALIGN_CENTRE|wxALL, 2 );
        boxink->Add( boxbriflatext1, 0, wxALIGN_CENTRE|wxALL, 2 );
        boxink->Add( Butt_bright[0], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_bright[1], 0, wxALIGN_CENTRE, 2 );
        boxink->Add( Butt_bright[2], 0, wxALIGN_CENTRE|wxALL, 2 );
        mainbox->Add( boxink, 0, wxALIGN_CENTRE, 0 );
        boxpaper->Add( boxpapertext, 0, wxALIGN_CENTRE|wxALL, 2 );
        boxpaper->Add( Butt_paper[0], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[1], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[2], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[3], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[4], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[5], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[6], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[7], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_paper[8], 0, wxALIGN_CENTRE|wxALL, 2 );
        boxpaper->Add( boxbriflatext2, 0, wxALIGN_CENTRE|wxALL, 2 );
        boxpaper->Add( Butt_flash[0], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_flash[1], 0, wxALIGN_CENTRE, 2 );
        boxpaper->Add( Butt_flash[2], 0, wxALIGN_CENTRE|wxALL, 2 );

        mainbox->Add( boxpaper, 0, wxALIGN_CENTRE, 2 );

        Butt_ink[col_ink]->Enable( FALSE );
        Butt_paper[col_paper]->Enable( FALSE );
        Butt_bright[col_bright]->Enable( FALSE );
        Butt_flash[col_flash]->Enable( FALSE );

        this->SetAutoLayout( TRUE );
        this->SetSizer( mainbox );
        mainbox->Fit( this );
        mainbox->SetSizeHints( this );
        canvas->SetScrollbars(1,1,1,1); // 1x1 when no graphic
        canvas2->SetSize(1,1);          // 1x1 when no graphic
        canvas2->framee=this;
        this->SetSizeHints(470,260,1280,1024,-1,-1); // Limit max size to
                                // avoid someone setting a screen bigger than
                                // the bigescreen bitmap (would crash)

        this->SetDropTarget(new DnDFile(this));

        if (argc>1)
                {
                for (int n=1;n<argc;n++)
                        {
                        if (Files_open==12)
                                {
                                (void)wxMessageBox(_("Can't open more than 12 graphics"),_("SevenuP"));
                                break;
                                }
                        Files[Files_open] = new class OpenFile(wc2s(argv[n]));
                        if (Files[Files_open]->OpenGraph->Propied1!=0)
                                {
                                Files_current=Files_open++;
                                if (sselect==2)
                                        {
                                        sselect=0;
                                        DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                                        }
                                Current = Files[Files_current];
                                Graphic2 = Files[Files_current]->OpenGraph;
                                Frame_total=Current->OpenFrames;
                                wxChar num0=48+(Files_open%10);
                                wxString menunumber,menuname;
                                menunumber=num0;

                                // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                                if (Files_open>10)
                                        {
                                        menuname=_("1")+menunumber+_(" - ")+Current->OpenName;
                                        }
                                else
                                        {
                                        if (Files_open==10)     menuname=_("1")+menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                                        else                    menuname=menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                                        }
#else
                                if (Files_open>9)
                                        {
                                        menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                                        }
                                else
                                        {
                                        menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                                        }
#endif
                                menuFile->Append(FILE_1+Files_open-1, menuname);
                                size_i = Graphic2->GetSizeX();
                                size_j = Graphic2->GetSizeY();
                                sizechar_i= Graphic2->x_charsize;
                                sizechar_j= Graphic2->y_charsize;
                                if (AutoZoom)
                                        {
                                        int clientx,clienty,tryzoom;
                                        canvas->GetClientSize(&clientx,&clienty);
                                        menuBar->Check(ZOOM_1+zoom,FALSE);
                                        if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,TRUE);
                                        if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,TRUE);
                                        for (tryzoom=0;tryzoom<ZOOMLEVELS;tryzoom++)
                                                {
                                                if ((size_i*ZoomValue[tryzoom]<clientx)&&(size_j*ZoomValue[tryzoom]<clienty))
                                                        {
                                                        zoom=tryzoom;
                                                        }
                                                }
                                        menuBar->Check(ZOOM_1+zoom,TRUE);
                                        if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,FALSE);
                                        if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,FALSE);
                                        }
                                size_x = size_i*ZoomValue[zoom];
                                size_y = size_j*ZoomValue[zoom];
                                DrawArea(0,0,sizechar_i,sizechar_j);
                                }
                        else
                                {
                                delete Files[Files_open];
                                }
                        }
                }
        canvas2->Show(TRUE);
        wxInitAllImageHandlers();
        if (Files_open==0) return;
        Mask_view=0;
        menuMask->Check(MASK_VIEW,FALSE);
        Mask_use=Current->OpenPropierties-1;
        menuMask->Check(MASK_USE,(Mask_use));
        Frame_current=1;
        sselect=0;
        sel_start_x=sel_end_x=0;
        sel_start_y=sel_end_y=0;
        SetTit();
        ClearUndo();
        disablezoom=1;
        canvas->Scroll(0,0);
        canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        disablezoom=0;
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

TheFrame::~TheFrame()
{
}

// FILE MENU FUNCTIONS

// Create a new graphic
void TheFrame::FileNew(wxCommandEvent &event)
{
        if (Files_open==12)
                {
                (void)wxMessageBox(_("Can't open more than 12 graphics"),_("SevenuP"));
                return;
                }
        wxDialog dialog(this,-1,wxString(_("New Graph Properties")));

                newitem0 = new wxBoxSizer (wxVERTICAL);
                newitem1 = new wxBoxSizer (wxHORIZONTAL);
                wxFlexGridSizer *newitem3 = new wxFlexGridSizer( 2, 0, 0 );
                wxStaticText *newitem4 = new wxStaticText( &dialog, ID_TEXT, _("Size X:"), wxDefaultPosition, wxDefaultSize, 0 );
                newitem5 = new wxSpinCtrl( &dialog, ID_NEW_SPINCTRL1, wxString::Format(_("%d"),newdefaultx), wxDefaultPosition, wxSize(50,-1), 0, 1, 256, newdefaultx );
                wxStaticText *newitem6 = new wxStaticText( &dialog, ID_TEXT,_("Size Y:"), wxDefaultPosition, wxDefaultSize, 0 );
                newitem7 = new wxSpinCtrl( &dialog, ID_NEW_SPINCTRL2, wxString::Format(_("%d"),newdefaulty), wxDefaultPosition, wxSize(50,-1), 0, 1, 192, newdefaulty );
                wxButton *newitem8 = new wxButton( &dialog, wxID_OK,_("OK"), wxDefaultPosition, wxDefaultSize, 0 );
                wxButton *newitem9 = new wxButton( &dialog, wxID_CANCEL,_("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );

                newitem3->Add( newitem4, 0, wxALIGN_CENTRE|wxALL, 0 );
                newitem3->Add( newitem5, 0, wxALIGN_CENTRE|wxALL, 5 );
                newitem3->Add( newitem6, 0, wxALIGN_CENTRE|wxALL, 5 );
                newitem3->Add( newitem7, 0, wxALIGN_CENTRE|wxALL, 0 );

                newitem1->Add( newitem8, 0, wxALIGN_CENTRE|wxALL, 5 );
                newitem1->Add( newitem9, 0, wxALIGN_CENTRE|wxALL, 5 );

                newitem0->Add( newitem3, 0, 0, 5 );
                newitem0->Add( newitem1, 0, wxALIGN_CENTRE|wxALL, 5 );


        dialog.SetAutoLayout(TRUE);
        dialog.SetSizer(newitem0);
        newitem0->Fit(&dialog);
 	dialog.CentreOnParent();
        if (dialog.ShowModal() == wxID_OK)
                {
                Files[Files_open] = new class OpenFile(newitem5->GetValue(),newitem7->GetValue(),FALSE,newfiles++);
                if (newfiles==100) newfiles=0;
                Files_current=Files_open++;

                if (sselect==2)
                        {
                        sselect=0;
                        DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                        }
                sselect=0;
                sel_start_x=sel_end_x=0;
                sel_start_y=sel_end_y=0;

                Current = Files[Files_current];
                Graphic2 = Files[Files_current]->OpenGraph;

                Frame_current=1;
                Frame_total=Current->OpenFrames;

                SetTit();
                ClearUndo();

                size_i = Graphic2->GetSizeX();
                size_j = Graphic2->GetSizeY();
                sizechar_i= Graphic2->x_charsize;
                sizechar_j= Graphic2->y_charsize;
                size_x = size_i*ZoomValue[zoom];
                size_y = size_j*ZoomValue[zoom];
                disablezoom=1;
                canvas->Scroll(0,0);
                canvas->SetScrollbars(1,1,size_x,size_y);
                canvas2->SetSize(size_x,size_y);
                disablezoom=0;

                wxChar num0=48+(Files_open%10);
                wxString menunumber,menuname;
                menunumber=num0;

                // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                if (Files_open>10)
                        {
                        menuname=_("1")+menunumber+_(" - ")+Current->OpenName;
                        }
                else
                        {
			if (Files_open==10)	menuname=_("1")+menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
			else			menuname=menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                        }
#else
                if (Files_open>9)
                        {
                        menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                        }
                else
                        {
                        menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                        }
#endif

                menuFile->Append(FILE_1+Files_open-1, menuname);

                menuMask->Check(MASK_USE,FALSE);
                menuMask->Check(MASK_VIEW,FALSE);
                Mask_use=0;
                Mask_view=0;

                DrawArea(0,0,sizechar_i,sizechar_j);
                wxClientDC dc(canvas2);
                Rfrsh(dc,TRUE);
                }
}

// Load a .SEV graphic or a .SCR screen
void TheFrame::FileLoad(wxCommandEvent &event)
{
        if (Files_open==12)
                {
                (void)wxMessageBox(_("Can't open more than 12 graphics"),_("SevenuP"));
                return;
                }

	wxFileDialog dialog(this,_("Select a graphic file"), OpenFilePath, _(""),_("Any SevenuP file (*.sev; *.scr)|*.sev;*.scr|SEVenuP graphic format (*.sev)|*.sev|SCReen memory snapshot (*.scr)|*.scr"), 0);

	dialog.CentreOnParent();
        if (dialog.ShowModal() == wxID_OK)
                {
                lastx=-1;
                wxString s = dialog.GetPath();
                if (RememberOpenFilePath==1)
                        {
                        int searchindxwin=s.rfind(_("\\"));
                        int searchindxunix=s.rfind(_("/"));
                        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
                        OpenFilePath=s.Left(pathend+1);
                        }
                Files[Files_open] = new class OpenFile(ws2s(s));

                if (Files[Files_open]->OpenGraph->Propied1!=0)
                        {
                        Files_current=Files_open++;

                        if (sselect==2)
                                {
                                sselect=0;
                                DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                                }
                        sselect=0;
                        sel_start_x=sel_end_x=0;
                        sel_start_y=sel_end_y=0;

                        Current = Files[Files_current];
                        Graphic2 = Files[Files_current]->OpenGraph;

                        Frame_current=1;
                        Frame_total=Current->OpenFrames;

                        SetTit();
                        ClearUndo();

                        Mask_view=0;
                        menuMask->Check(MASK_VIEW,FALSE);
                        Mask_use=Current->OpenPropierties-1;
                        menuMask->Check(MASK_USE,(Mask_use));

                        size_i = Graphic2->GetSizeX();
                        size_j = Graphic2->GetSizeY();
                        sizechar_i= Graphic2->x_charsize;
                        sizechar_j= Graphic2->y_charsize;
                        if (AutoZoom)
                                {
                                int clientx,clienty,tryzoom;
                                canvas->GetClientSize(&clientx,&clienty);
                                menuBar->Check(ZOOM_1+zoom,FALSE);
                                if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,TRUE);
                                if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,TRUE);
                                for (tryzoom=0;tryzoom<ZOOMLEVELS;tryzoom++)
                                        {
                                        if ((size_i*ZoomValue[tryzoom]<clientx)&&(size_j*ZoomValue[tryzoom]<clienty))
                                                {
                                                zoom=tryzoom;
                                                }
                                        }
                                menuBar->Check(ZOOM_1+zoom,TRUE);
                                if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,FALSE);
                                if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,FALSE);
                                }
                        size_x = size_i*ZoomValue[zoom];
                        size_y = size_j*ZoomValue[zoom];
                        disablezoom=1;
                        canvas->Scroll(0,0);
                        canvas->SetScrollbars(1,1,size_x,size_y);
                        canvas2->SetSize(size_x,size_y);
                        disablezoom=0;

                        wxChar num0=48+(Files_open%10);
                        wxString menunumber,menuname;
                        menunumber=num0;

                        // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                        if (Files_open>10)
                                {
                                menuname=_("1")+menunumber+_(" - ")+Current->OpenName;
                                }
                        else
                                {
				if (Files_open==10) menuname=_("1")+menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
				else menuname=menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                                }
#else
                        if (Files_open>9)
                                {
                                menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                                }
                        else
                        {
                                menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                                }
#endif

                        menuFile->Append(FILE_1+Files_open-1, menuname);

                        DrawArea(0,0,sizechar_i,sizechar_j);
                        wxClientDC dc(canvas2);
                        Rfrsh(dc,TRUE);
                        }
                else
                        {
                        delete Files[Files_open];
                        }
                }
}


void TheFrame::FileImport(wxCommandEvent &event)
{
        if (Files_open==12)
                {
                (void)wxMessageBox(_("Can't open more than 12 graphics"),_("SevenuP"));
                return;
                }
        wxFileDialog dialog(this, _("Select an image file"), ImportImagePath, _(""),_("Any image|*.bmp;*.gif;*.jpg;*.png;*.pcx;*.tif;*.iff;*.xmp|BMP - Windows Bitmap|*.bmp|GIF - Compuserve Graphic Interchange Format|*.gif|JPG - Joint Picture Experts Group|*.jpg|PNG - Portable Network Graphic|*.png|PCX - Zsoft Paintbrush|*.pcx|TIF - Tagged Image File Format|*.tif|IFF - Amiga Interchange File Format|*.iff|XPM - X-Bitmap|*.xpm"), 0);

	dialog.CentreOnParent();
        if (dialog.ShowModal() == wxID_OK)
                {
                lastx=-1;
                wxString s = dialog.GetPath();
                if (RememberImportImagePath==1)
                        {
                        int searchindxwin=s.rfind(_("\\"));
                        int searchindxunix=s.rfind(_("/"));
                        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
                        ImportImagePath=s.Left(pathend+1);
                        }
                Files[Files_open] = new class OpenFile(ws2s(s));

                if (Files[Files_open]->OpenGraph->Propied1!=0)
                        {
                        Files_current=Files_open++;

                        if (sselect==2)
                                {
                                sselect=0;
                                DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                                }
                        sselect=0;
                        sel_start_x=sel_end_x=0;
                        sel_start_y=sel_end_y=0;

                        Current = Files[Files_current];
                        Graphic2 = Files[Files_current]->OpenGraph;

                        Frame_current=1;
                        Frame_total=Current->OpenFrames;

                        SetTit();
                        ClearUndo();

                        Mask_view=0;
                        menuMask->Check(MASK_VIEW,FALSE);
                        Mask_use=Current->OpenPropierties-1;
                        menuMask->Check(MASK_USE,(Mask_use));

                        size_i = Graphic2->GetSizeX();
                        size_j = Graphic2->GetSizeY();
                        sizechar_i= Graphic2->x_charsize;
                        sizechar_j= Graphic2->y_charsize;
                        if (AutoZoom)
                                {
                                int clientx,clienty,tryzoom;
                                canvas->GetClientSize(&clientx,&clienty);
                                menuBar->Check(ZOOM_1+zoom,FALSE);
                                if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,TRUE);
                                if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,TRUE);
                                for (tryzoom=0;tryzoom<ZOOMLEVELS;tryzoom++)
                                        {
                                        if ((size_i*ZoomValue[tryzoom]<clientx)&&(size_j*ZoomValue[tryzoom]<clienty))
                                                {
                                                zoom=tryzoom;
                                                }
                                        }
                                menuBar->Check(ZOOM_1+zoom,TRUE);
                                if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,FALSE);
                                if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,FALSE);
                                }
                        size_x = size_i*ZoomValue[zoom];
                        size_y = size_j*ZoomValue[zoom];
                        disablezoom=1;
                        canvas->Scroll(0,0);
                        canvas->SetScrollbars(1,1,size_x,size_y);
                        canvas2->SetSize(size_x,size_y);
                        disablezoom=0;

                        wxChar num0=48+(Files_open%10);
                        wxString menunumber,menuname;
                        menunumber=num0;

                        // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                        if (Files_open>10)
                                {
                                menuname=_("1")+menunumber+_(" - ")+Current->OpenName;
                                }
                        else
                                {
				if (Files_open==10) menuname=_("1")+menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
				else menuname=menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                                }
#else
                        if (Files_open>9)
                                {
                                menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                                }
                        else
                        {
                                menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                                }
#endif

                        menuFile->Append(FILE_1+Files_open-1, menuname);

                        DrawArea(0,0,sizechar_i,sizechar_j);
                        wxClientDC dc(canvas2);
                        Rfrsh(dc,TRUE);
                        }
                else
                        {
                        delete Files[Files_open];
                        }
                }
}

void TheFrame::FileBinImport(wxCommandEvent &event)
{
        if (Files_open==0) return;
        wxFileDialog dialog(this, _("Select a binary file"), ImportImagePath, _(""), _("Any file|*.*|BIN - Binary File|*.bin"), 0);
	dialog.CentreOnParent();
        if (dialog.ShowModal() == wxID_OK)
                {
                lastx=-1;
                wxString s = dialog.GetPath();
                if (RememberImportImagePath==1)
                        {
                        int searchindxwin=s.rfind(_("\\"));
                        int searchindxunix=s.rfind(_("/"));
                        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
			ImportImagePath=s.Left(pathend+1);
                        }
                Files[Files_current]->ImportBIN(ws2s(s),optradioboxdef,priorities[0],priorities[1],priorities[2],priorities[3],priorities[4],maskfirst,zigzag,AttrMask,AttrMaskFlash*128+AttrMaskBright*64+AttrMaskPaper*56+AttrMaskInk*7,interleave,z88dksize);
	        wxClientDC dc(canvas2);
	        Rfrsh(dc,TRUE);
                }
}


// Export graphic to an image file
void TheFrame::FileExportImage(wxCommandEvent &event)
{
        if (Files_open==0) return;
        lastx=-1;

        wxFileDialog dialog(this,_("Choose name and image format to save"), ExportImagePath, Current->OpenName,_("PNG - Portable Network Graphic|*.png|BMP - Windows Bitmap|*.bmp|JPG - Joint Picture Experts Group|*.jpg|PCX - Zsoft Paintbrush|*.pcx|TIF - Tagged Image File Format|*.tif|XPM - X-Bitmap|*.xpm"),wxSAVE|wxOVERWRITE_PROMPT);
        
        dialog.CentreOnParent();
        if (dialog.ShowModal() == wxID_OK)
                {
                wxString s=dialog.GetPath();
	        wxString ext;
	        ext = s.Right(4);
	        ext = ext.Lower();
	        int fi=dialog.GetFilterIndex();
		wxString extensions[] = {_(".png"),_(".bmp"),_(".jpg"),_(".pcx"),_(".tif"),_(".xpm")};
       		if (ext!=extensions[fi])
        	       	{
               		s+=extensions[fi];
               		}
                if (RememberExportImagePath==1)
                        {
                        int searchindxwin=s.rfind(_("\\"));
                        int searchindxunix=s.rfind(_("/"));
                        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
                        ExportImagePath=s.Left(pathend+1);
                        }
                Files[Files_current]->Export(ws2s(s),Frame_current);
                }
}

// Close graph
void TheFrame::FileClose(wxCommandEvent &event)
{
        if (Files_open==0) return;
        wxMessageDialog dialog( this, _("Unsaved work will be lost\nAre you sure?"),_("Warning"),wxOK|wxCANCEL);
        dialog.CentreOnParent();
        if ((Current->flagsure==1)&&(Warn_closefile==1))
                {
                if (dialog.ShowModal() == wxID_CANCEL)
                        {
                        return;
                        }
                }
        delete Files[Files_current];
        Files_open--;
        if (Files_open<1)
                {
                menuFile->Delete(FILE_1);
                Files_current=0;
                Files_open=0;
                disablezoom=1;
                canvas->Scroll(0,0);
                canvas->SetScrollbars(1,1,1,1);
                canvas2->SetSize(1,1);
                size_i=1;
                size_j=1;
                disablezoom=0;
                this->SetTitle(_("SevenuP v1.21"));
                ClearUndo();
                Mask_view=0;
                menuMask->Check(MASK_VIEW,FALSE);
                Mask_use=0;
                menuMask->Check(MASK_USE,FALSE);
                wxClientDC dc(canvas2);
                Rfrsh(dc,TRUE);
                return;
                }
        for (int i=Files_current;i<Files_open;i++)
                {
                Files[i]=Files[i+1];
                wxChar num0=48+((i+1)%10);
                wxString menunumber,menuname;
                menunumber=num0;
                // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                if (i>9)
                        {
                        menuname=_("1")+menunumber+_(" - ")+Files[i]->OpenName;
                        }
                else
                        {
			if (i==9) menuname=_("1")+menunumber+_(" - ")+Files[i]->OpenName.Left(Files[i]->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
			else   menuname=menunumber+_(" - ")+Files[i]->OpenName.Left(Files[i]->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                        }
#else
                if (i>8)
                        {
                        menuname=Files[i]->OpenName.Left(Files[i]->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                        }
                else
                        {
                        menuname=Files[i]->OpenName.Left(Files[i]->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                        }
#endif
                menuFile->SetLabel(FILE_1+i, menuname);
                }
        menuFile->Delete(FILE_1+Files_open);
        if (Files_current==Files_open) Files_current--;
        sselect=0;
        sel_start_x=sel_end_x=0;
        sel_start_y=sel_end_y=0;
        Current = Files[Files_current];
        Graphic2 = Files[Files_current]->OpenGraph;
        Frame_current=1;
        Frame_total=Current->OpenFrames;
        Mask_view=0;
        menuMask->Check(MASK_VIEW,FALSE);
        Mask_use=Current->OpenPropierties-1;
        menuMask->Check(MASK_USE,(Mask_use));
        size_i = Graphic2->GetSizeX();
        size_j = Graphic2->GetSizeY();
        sizechar_i= Graphic2->x_charsize;
        sizechar_j= Graphic2->y_charsize;
        size_x = size_i*ZoomValue[zoom];
        size_y = size_j*ZoomValue[zoom];
        disablezoom=1;
        canvas->Scroll(0,0);
        canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        disablezoom=0;
        SetTit();
        ClearUndo();
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

void TheFrame::FileRename(wxCommandEvent &event)
{
        if (Files_open==0) return;
        wxTextEntryDialog rename(this,_("Enter new name"),_("Rename File"),_(""),wxOK | wxCANCEL);
        rename.CentreOnParent();
        if (rename.ShowModal() == wxID_OK)
                {
                Current->OpenName=rename.GetValue()+_('\0');
                Current->OpenPath=_("");
                SetTit();
                }
}

// Save a .SEV graphic, using current path & file name
void TheFrame::FileFastSave(wxCommandEvent &event)
{
        if (Files_open==0) return;
        lastx=-1;
        if (Files[Files_current]->OpenPath!=_(""))
                {
                Files[Files_current]->Save(ws2s(Files[Files_current]->OpenPath));
		if (Save_showdialog==1)
			{
                	(void)wxMessageBox(Files[Files_current]->OpenPath+_(" saved"),_("SevenuP"));
			}
                }
        else
                {
                wxFileDialog dialog(this, _("Choose a file name to save"), SavePath, Current->OpenName,_("SEVenuP graphic format (*.sev)|*.sev"), wxSAVE|wxOVERWRITE_PROMPT);
                dialog.CentreOnParent();
                if (dialog.ShowModal() == wxID_OK)
                        {
                        wxString s=dialog.GetPath();
	        	wxString ext;
	        	ext = s.Right(4);
	        	ext = ext.Lower();
       			if (ext!=_(".sev"))
        	       		{
               			s+=_(".sev");
	               		}
                        if (RememberSavePath==1)
                                {
                                int searchindxwin=s.rfind(_("\\"));
                                int searchindxunix=s.rfind(_("/"));
                                int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
                                SavePath=s.Left(pathend+1);
                                }

                        Files[Files_current]->Save(ws2s(s));
                        Files[Files_current]->OpenPath=s;
                        SetTit();
                        }
                }
}

// Save a .SEV graphic or .SCR screen, giving the file name
void TheFrame::FileSave(wxCommandEvent &event)
{
        if (Files_open==0) return;
        lastx=-1;
	wxFileDialog *dialog;
	wxString extensions[] = {_(".scr"),_(".sev")};
	if ((Current->OpenGraph->GetSizeX()==256)&&(Current->OpenGraph->GetSizeY()==192))
                {
	        dialog = new wxFileDialog(this, _("Choose a file name to save"), SavePath, Current->OpenName, _("SCReen memory snapshot (*.scr)|*.scr|SEVenuP graphic format (*.sev)|*.sev"), wxSAVE|wxOVERWRITE_PROMPT);
		}
	else	{
	        dialog = new wxFileDialog(this,_("Choose a file name to save"), SavePath, Current->OpenName,_("SEVenuP graphic format (*.sev)|*.sev"), wxSAVE|wxOVERWRITE_PROMPT);
		extensions[0] =_(".sev");
		}
	dialog->CentreOnParent();
        if (dialog->ShowModal() == wxID_OK)
                {
                wxString s=dialog->GetPath();
                if (RememberSavePath==1)
                        {
                        int searchindxwin=s.rfind(_("\\"));
                        int searchindxunix=s.rfind(_("/"));
                        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
                        SavePath=s.Left(pathend+1);
                        }
	        wxString ext;
	        ext = s.Right(4);
	        ext = ext.Lower();
	        int fi=dialog->GetFilterIndex();
       		if (ext!=extensions[fi])
        	       	{
               		s+=extensions[fi];
               		}
                Files[Files_current]->OpenPath=s;
                Files[Files_current]->Save(ws2s(s));
                SetTit();

                wxChar num0=48+((Files_current+1)%10);
                wxString menunumber,menuname;
                menunumber=num0;

                // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                if (Files_open>9)
                        {
                        menuname=_("1")+menunumber+_(" - ")+Current->OpenName;
                        }
                else
                        {
			if (Files_open==10)	menuname=_("1")+menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
			else			menuname=menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                        }
#else
                if (Files_current>8)
                        {
                        menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                        }
                else
                        {
                        menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                        }
#endif
                menuFile->SetLabel(FILE_1+Files_current, menuname);
                }
}

// Save as binary or source
void TheFrame::FileExportData(wxCommandEvent &event)
{
        if (Files_open==0) return;
        wxString ExportDataExtensions;
        lastx=-1;
	wxString extensions[] = {_(".bin"),_(".asm"),_(".c")};
        switch (defaultexport)
        	{
        	case 0:	{
        		ExportDataExtensions=_("raw BINary (*.BIN)|*.BIN|ASM Source (*.ASM)|*.ASM|C Source (*.C)|*.C");
        		break;
        		}
        	case 1: {
        		ExportDataExtensions=_("ASM Source (*.ASM)|*.ASM|C Source (*.C)|*.C|raw BINary (*.BIN)|*.BIN");
			extensions[0] = _(".asm");
			extensions[1] = _(".c");
			extensions[2] = _(".bin");
        		break;
        		}
        	case 2: {
        		ExportDataExtensions=_("C Source (*.C)|*.C|raw BINary (*.BIN)|*.BIN|ASM Source (*.ASM)|*.ASM");
			extensions[0] = _(".c");
			extensions[1] = _(".bin");
			extensions[2] = _(".asm");
        		break;
        		}
        	}
        wxFileDialog dialog(this, _("Choose a file name to save"), ExportDataPath, Current->OpenName, ExportDataExtensions, wxSAVE|(wxOVERWRITE_PROMPT*(1-appendfile)));
        dialog.CentreOnParent();
        if (dialog.ShowModal() == wxID_OK)
                {
                wxString s=dialog.GetPath();

	        int fi=dialog.GetFilterIndex();
	        wxString ext;
	        ext = s.Right(extensions[fi].Len());
	        ext = ext.Lower();
       		if (ext!=extensions[fi])
        	       	{
               		s+=extensions[fi];
               		}
                if (RememberExportDataPath==1)
                        {
                        int searchindxwin=s.rfind(_("\\"));
                        int searchindxunix=s.rfind(_("/"));
                        int pathend=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
                        ExportDataPath=s.Left(pathend+1);
                        }

                Files[Files_current]->ExportData(ws2s(s),optradioboxdef,priorities[0],priorities[1],priorities[2],priorities[3],priorities[4],maskfirst,asmtype,appendfile,zigzag,AttrMask,AttrMaskFlash*128+AttrMaskBright*64+AttrMaskPaper*56+AttrMaskInk*7,interleave,z88dksize,nolabel);
                }
}

// Show output options dialog
void TheFrame::FileSaveOPT(wxCommandEvent &event)
{
        GetOutputOptions();
}

// Select generic assembler, decimal output
void TheFrame::FileSaveSelAsm0(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=0;
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,TRUE);
        SetStatusText(_("Generic assembler, dec"),0);
}

// Select TASM assembler, decimal output
void TheFrame::FileSaveSelAsm1(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=1;
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,TRUE);
        SetStatusText(_("TASM, dec"),0);
}

// Select The E-Z80 Way assembler
void TheFrame::FileSaveSelAsm2(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=2;
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,TRUE);
        SetStatusText(_("The E-Z80 Way"),0);
}

// Select tniASM assembler, decimal output
void TheFrame::FileSaveSelAsm3(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=3;
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,TRUE);
        SetStatusText(_("tniASM, dec"),0);
}

// Select generic assembler, hexadecimal output with prefix "$"
void TheFrame::FileSaveSelAsm4(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=4;
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,TRUE);
        SetStatusText(_("Generic assembler, hex using $"),0);
}

// Select generic assembler, hexadecimal output with prefix "0x"
void TheFrame::FileSaveSelAsm5(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=5;
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,TRUE);
        SetStatusText(_("Generic assembler, hex using 0x"),0);
}

// Select TASM assembler, decimal output
void TheFrame::FileSaveSelAsm6(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,FALSE);
        asmtype=6;
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,TRUE);
        SetStatusText(_("TASM, hex"),0);
}

// Select tniASM assembler, decimal output
void TheFrame::FileSaveSelAsm7(wxCommandEvent &event)
{
        menuFile_SelAsm->Check(FILE_ASMSELECT_0,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_1,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_2,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_3,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_4,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_5,FALSE);
        menuFile_SelAsm->Check(FILE_ASMSELECT_6,FALSE);
        asmtype=7;
        menuFile_SelAsm->Check(FILE_ASMSELECT_7,TRUE);
        SetStatusText(_("tniASM, hex"),0);
}

// Close application
void TheFrame::FileQuit(wxCommandEvent &event)
{
        Close(FALSE);
}

// Select another open graphic
void TheFrame::FileSelect (wxCommandEvent &event)
{
        if (sselect==2)
                {
                sselect=0;
                DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                }
        sselect=0;
        sel_start_x=sel_end_x=0;
        sel_start_y=sel_end_y=0;
        int eve=event.GetId()-FILE_1;
        if (eve<Files_open) Files_current=eve;
        Current = Files[Files_current];
        Graphic2 = Files[Files_current]->OpenGraph;
        Frame_current=1;
        Frame_total=Current->OpenFrames;
        Mask_view=0;
        menuMask->Check(MASK_VIEW,FALSE);
        Mask_use=Current->OpenPropierties-1;
        menuMask->Check(MASK_USE,(Mask_use));
        size_i = Graphic2->GetSizeX();
        size_j = Graphic2->GetSizeY();
        sizechar_i= Graphic2->x_charsize;
        sizechar_j= Graphic2->y_charsize;
        size_x = size_i*ZoomValue[zoom];
        size_y = size_j*ZoomValue[zoom];
        disablezoom=1;
        canvas->Scroll(0,0);
        canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        disablezoom=0;
        SetTit();
        ClearUndo();
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// EDIT MENU FUNCTIONS

// Undo last action
void TheFrame::EditUndo(wxCommandEvent &event)
{
        if (Files_open==0) return;
        if (Undo_total==0) return;

        SaveRedo();
        for (int i=0;i<size_i;i++)
                {
                for (int j=0;j<size_j;j++)
                        {
                        if (GraphUndo->GetPixel(i,j)==1)
                                {
                                Graphic2->SetPixel(i,j);
                                }
                        else
                                {
                                Graphic2->ResetPixel(i,j);
                                }
                        }
                }
        for (int i=0;i<sizechar_i;i++)
                {
                for (int j=0;j<sizechar_j;j++)
                        {
                        int k=GraphUndo->GetAttr(i+j*sizechar_i);
                        Graphic2->SetAttr(i+j*sizechar_i,k);
                        }
                }
        if ((Current->OpenPropierties==2)&&(GraphUndo->Mask!=NULL))
                {
                for (int i=0;i<size_i;i++)
                        {
                        for (int j=0;j<size_j;j++)
                                {
                                if (GraphUndo->Mask->GetPixel(i,j)==1)
                                        {
                                        Graphic2->Mask->SetPixel(i,j);
                                        }
                                else
                                        {
                                        Graphic2->Mask->ResetPixel(i,j);
                                        }
                                }
                        }
                }

        Aux=GraphUndo;
        GraphUndo=Aux->NextGraph;
        Aux->NextGraph=NULL;
        delete Aux;
        Undo_total--;
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// Redo last undoed action
void TheFrame::EditRedo(wxCommandEvent &event)
{
        if (Files_open==0) return;
        if (Redo_total==0) return;

        SaveUndo(FALSE);

        for (int i=0;i<size_i;i++)
                {
                for (int j=0;j<size_j;j++)
                        {
                        if (GraphRedo->GetPixel(i,j)==1)
                                {
                                Graphic2->SetPixel(i,j);
                                }
                        else
                                {
                                Graphic2->ResetPixel(i,j);
                                }
                        }
                }
        for (int i=0;i<sizechar_i;i++)
                {
                for (int j=0;j<sizechar_j;j++)
                        {
                        int k=GraphRedo->GetAttr(i+j*sizechar_i);
                        Graphic2->SetAttr(i+j*sizechar_i,k);
                        }
                }
        if ((Current->OpenPropierties==2)&&(GraphRedo->Mask!=NULL))
                {
                for (int i=0;i<size_i;i++)
                        {
                        for (int j=0;j<size_j;j++)
                                {
                                if (GraphRedo->Mask->GetPixel(i,j)==1)
                                        {
                                        Graphic2->Mask->SetPixel(i,j);
                                        }
                                else
                                        {
                                        Graphic2->Mask->ResetPixel(i,j);
                                        }
                                }
                        }
                }

        Aux=GraphRedo;
        GraphRedo=Aux->NextGraph;
        Aux->NextGraph=NULL;
        delete Aux;
        Redo_total--;
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// Copy area or graphic
void TheFrame::EditCopy(wxCommandEvent &event)
{
        if (Files_open==0) return;
        int chrsizx_cp;
        int copypasteoffsetx,copypasteoffsety;

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                copypastemask=1;
                }
        else
                {
                Graphic4=Graphic2;
                copypastemask=0;
                }

        if (canpaste==1)
                {
                delete copypastable;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;

        copypasteoffsetx=sel_start_x%8;
        copypasteoffsety=sel_start_y%8;
        copypastable = new SP_Graph(sel_end_x-ssxc*8,sel_end_y-ssyc*8);
        copypastable->Propied1=copypasteoffsetx;
        copypastable->Propied2=copypasteoffsety;

        chrsizx_cp=copypastable->x_charsize;

        for (int i=sel_start_x;i<sel_end_x;i++)
                {
                for (int j=sel_start_y;j<sel_end_y;j++)
                        {
                        if(Graphic4->GetPixel(i,j)) copypastable->SetPixel(i-sel_start_x+copypasteoffsetx,j-sel_start_y+copypasteoffsety);
                        }
                }

        for (int i=ssxc;i<sexc;i++)
                {
                for (int j=ssyc;j<seyc;j++)
                        {
                        copypastable->SetAttr((j-ssyc)*chrsizx_cp+i-ssxc,Graphic2->GetAttr(j*sizechar_i+i));
                        }
                }
        canpaste=1;
}

// Enter paste mode, saving old cursor mode if necessary
void TheFrame::EditPaste(wxCommandEvent &event)
{
        if (Files_open==0) return;
        if (canpaste)
                {
                if (left_func!=9)
                        {
                        left_func=9;
                        right_func=10;
                        if (cursormode<3) oldcursormode=cursormode;
                        cursormode=3;
                        SetStatusText(_("Paste/Cancel paste"),0);
                        }
                else    {
                        switch (oldcursormode)
                                {
                                case 1: {
                                        cursormode=1;
                                        left_func=3;
                                        right_func=4;
                                        switch (sselect)
                                                {
                                                case 0: {
                                                        SetStatusText(_("Toggle Pixel/Select zone"),0);
                                                        break;
                                                        }
                                                case 1: {
                                                        SetStatusText(_("Toggle Pixel/Close selection"),0);
                                                        break;
                                                        }
                                                case 2: {
                                                        SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                                        break;
                                                        }
                                                }
                                        break;
                                        }
                                case 2: {
                                        cursormode=2;
                                        left_func=5;
                                        right_func=6;
                                        if ((Mask_use==1)&&(Mask_view==1))
                                                {
                                                SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                                }
                                        else
                                                {
                                                SetStatusText(_("Set Attributes/Get Attributes"),0);
                                                }
                                        break;
                                        }
                                case 3:
                                case 4:
                                case 5:
                                case 0: {
                                        cursormode=0;
                                        left_func=1;
                                        right_func=2;
                                        SetStatusText(_("Set Pixel/Reset Pixel"),0);
                                        break;
                                        }
                                }
                        }
                }
}

// Toggle fill mode, saving old cursor mode if necessary
void TheFrame::FillSolid(wxCommandEvent &event)
{
        if (Files_open==0) return;
        if (left_func!=7)
                {
                left_func=7;    // Fill solid
                right_func=10;
                if (cursormode<3) oldcursormode=cursormode;
                cursormode=4;   // Fill
                SetStatusText(_("Fill/Cancel fill"),0);
                }
        else    {
                switch (oldcursormode)
                        {
                        case 1: {
                                cursormode=1;
                                left_func=3;
                                right_func=4;
                                switch (sselect)
                                        {
                                        case 0: {
                                                SetStatusText(_("Toggle Pixel/Select zone"),0);
                                                break;
                                                }
                                        case 1: {
                                                SetStatusText(_("Toggle Pixel/Close selection"),0);
                                                break;
                                                }
                                        case 2: {
                                                SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                                break;
                                                }
                                        }
                                break;
                                }
                        case 2: {
                                cursormode=2;
                                left_func=5;
                                right_func=6;
                                if ((Mask_use==1)&&(Mask_view==1))
                                        {
                                        SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                        }
                                else
                                        {
                                        SetStatusText(_("Set Attributes/Get Attributes"),0);
                                        }
                                break;
                                }
                        case 3:
                        case 4:
                        case 5:
                        case 0: {
                                cursormode=0;
                                left_func=1;
                                right_func=2;
                                SetStatusText(_("Set Pixel/Reset Pixel"),0);
                                break;
                                }
                        }
                }
}

// Enter pattern fill mode, saving old cursor mode if necessary
void TheFrame::FillPattern(wxCommandEvent &event)
{
        if (Files_open==0) return;
        if (canpaste)
                {
                if (left_func!=8)
                        {
                        left_func=8;    // Fill pattern
                        right_func=10;
                        if (cursormode<3) oldcursormode=cursormode;
                        cursormode=5;   // Fill pattern
                        SetStatusText(_("Pattern fill/Cancel fill"),0);
                        }
                else
                        {
                        switch (oldcursormode)
                                {
                                case 1: {
                                        cursormode=1;
                                        left_func=3;
                                        right_func=4;
                                        switch (sselect)
                                                {
                                                case 0: {
                                                        SetStatusText(_("Toggle Pixel/Select zone"),0);
                                                        break;
                                                        }
                                                case 1: {
                                                        SetStatusText(_("Toggle Pixel/Close selection"),0);
                                                        break;
                                                        }
                                                case 2: {
                                                        SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                                        break;
                                                        }
                                                }
                                        break;
                                        }
                                case 2: {
                                        cursormode=2;
                                        left_func=5;
                                        right_func=6;
                                        if ((Mask_use==1)&&(Mask_view==1))
                                                {
                                                SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                                }
                                        else
                                                {
                                                SetStatusText(_("Set Attributes/Get Attributes"),0);
                                                }
                                        break;
                                        }
                                case 3:
                                case 4:
                                case 5:
                                case 0: {
                                        cursormode=0;
                                        left_func=1;
                                        right_func=2;
                                        SetStatusText(_("Set Pixel/Reset Pixel"),0);
                                        break;
                                        }
                                }
                        }
                }
}

// EFFECTS MENU FUNCTIONS

// Do the X Flip effect
void TheFrame::EffFlipX(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;

        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if ((eff_pix==1)||(Mask_view==1))
                {
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(i,j)) Graphic3->SetPixel(sel_end_x+sel_start_x-i-1,j);
                                }
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }
                }
        if ((eff_attr==1)&&(Mask_view==0))
                {
                for (int i=((sel_start_x)/8);i<((sel_end_x+7)/8);i++)
                        {
                        for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                {
                                Graphic3->SetAttr(j*sizechar_i+i,Graphic2->GetAttr(j*sizechar_i+sel_start_x/8+(sel_end_x-1)/8-i));
                                }
                        }
                for (int i=((sel_start_x)/8);i<((sel_end_x+7)/8);i++)
                        {
                        for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                {
                                Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                }
                        }
                }
        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Do the Y Flip effect
void TheFrame::EffFlipY(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }


        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;
        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if ((eff_pix==1)||(Mask_view==1))
                {
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(i,j)) Graphic3->SetPixel(i,sel_end_y+sel_start_y-j-1);
                                }
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }
                }
        if ((eff_attr==1)&&(Mask_view==0))
                {
                for (int i=((sel_start_x)/8);i<((sel_end_x+7)/8);i++)
                        {
                        for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                {
                                Graphic3->SetAttr(j*sizechar_i+i,Graphic2->GetAttr((sel_start_y/8+(sel_end_y-1)/8-j)*sizechar_i+i));
                                }
                        }
                for (int i=((sel_start_x)/8);i<((sel_end_x+7)/8);i++)
                        {
                        for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                {
                                Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                }
                        }
                }
        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Do the invert effect
void TheFrame::EffInvert(wxCommandEvent &event)
{
        if (Files_open==0) return;
        wxClientDC dc(canvas2);
        int atributo;

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;
        Current->flagsure=1;
        SaveUndo();

        if ((eff_pix==1)||(Mask_view==1))
                {
                for (int i=sel_start_x;i<sel_end_x;i++)
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                Graphic4->TogglePixel(i,j);
                                }
                }
        if ((eff_attr)&&(Mask_view==0))
                {
                for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                        {
                        for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                {
                                atributo=128*Graphic2->GetFlash(i,j)+64*Graphic2->GetBright(i,j)+8*Graphic2->GetInk(i,j)+Graphic2->GetPaper(i,j);
                                Graphic2->SetAttr(j*sizechar_i+i,atributo);
                                }
                        }
                }
        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Left rotation/shift
void TheFrame::EffRot_left(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;
        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if (eff_arr_char==TRUE)
                {
                if (sel_end_x-sel_start_x<9)
                        {
                        return;
                        }
                if ((eff_pix==1)||(Mask_view==1)) // 8 pixels
                        {
                        for (int i=sel_start_x;i<sel_end_x-8;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        if(Graphic4->GetPixel(i+8,j)) Graphic3->SetPixel(i,j);
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        for (int i=0;i<8;i++)
                                                {
                                                if(Graphic4->GetPixel(sel_start_x+i,j)) Graphic3->SetPixel(sel_end_x-8+i,j);
                                                }
                                        }
                                }

                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                        else                        Graphic4->ResetPixel(i,j);
                                        }
                                }
                        }
                if ((eff_attr==1)&&(Mask_view==0))        // 1 attr char
                        {
                        for (int i=((sel_start_x)/8);i<(((sel_end_x+7)/8)-1);i++)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic3->SetAttr(j*sizechar_i+i,Graphic2->GetAttr(j*sizechar_i+i+1));
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic3->SetAttr(j*sizechar_i+(sel_end_x-1)/8,Graphic2->GetAttr(j*sizechar_i+sel_start_x/8));
                                        }
                                }
                        for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                        }
                                }
                        }
                }
        else    // 1 pixel
                {
                if (sel_end_x-sel_start_x<2)
                        {
                        return;
                        }
                for (int i=sel_start_x;i<sel_end_x-1;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(i+1,j)) Graphic3->SetPixel(i,j);
                                }
                        }
                if (eff_arr_shift==FALSE)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(sel_start_x,j)) Graphic3->SetPixel(sel_end_x-1,j);
                                }
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }

                }

        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Right rotation/shift
void TheFrame::EffRot_right(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;
        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if (eff_arr_char==TRUE)
                {
                if (sel_end_x-sel_start_x<9)
                        {
                        return;
                        }
                if ((eff_pix==1)||(Mask_view==1)) // 8 pixels
                        {
                        for (int i=sel_start_x+8;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        if(Graphic4->GetPixel(i-8,j)) Graphic3->SetPixel(i,j);
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        for (int i=0;i<8;i++)
                                                {
                                                if(Graphic4->GetPixel(sel_end_x-8+i,j)) Graphic3->SetPixel(sel_start_x+i,j);
                                                }
                                        }
                                }

                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                        else                        Graphic4->ResetPixel(i,j);
                                        }
                                }
                        }
                if ((eff_attr==1)&&(Mask_view==0))        // 1 attr char
                        {
                        for (int i=((sel_start_x+8)/8);i<(((sel_end_x+7)/8));i++)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic3->SetAttr(j*sizechar_i+i,Graphic2->GetAttr(j*sizechar_i+i-1));
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic3->SetAttr(j*sizechar_i+sel_start_x/8,Graphic2->GetAttr(j*sizechar_i+(sel_end_x-1)/8));
                                        }
                                }
                        for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                        }
                                }
                        }
                }
        else    // 1 pixel
                {
                if (sel_end_x-sel_start_x<2)
                        {
                        return;
                        }
                for (int i=sel_start_x+1;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(i-1,j)) Graphic3->SetPixel(i,j);
                                }
                        }
                if (eff_arr_shift==FALSE)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(sel_end_x-1,j)) Graphic3->SetPixel(sel_start_x,j);
                                }
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }

                }

        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Up rotation/shift
void TheFrame::EffRot_up(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;
        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if (eff_arr_char==TRUE)
                {
                if (sel_end_y-sel_start_y<9)
                        {
                        return;
                        }
                if ((eff_pix==1)||(Mask_view==1)) // 8 pixels
                        {
                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y-8;j++)
                                        {
                                        if(Graphic4->GetPixel(i,j+8)) Graphic3->SetPixel(i,j);
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int i=sel_start_x;i<sel_end_x;i++)
                                        {
                                        for (int j=0;j<8;j++)
                                                {
                                                if(Graphic4->GetPixel(i,sel_start_y+j)) Graphic3->SetPixel(i,sel_end_y-8+j);
                                                }
                                        }
                                }

                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                        else                        Graphic4->ResetPixel(i,j);
                                        }
                                }
                        }
                if ((eff_attr==1)&&(Mask_view==0))        // 1 attr char
                        {
                        for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                {
                                for (int j=(sel_start_y/8);j<(((sel_end_y+7)/8)-1);j++)
                                        {
                                        Graphic3->SetAttr(j*sizechar_i+i,Graphic2->GetAttr((j+1)*sizechar_i+i));
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                        {
                                        Graphic3->SetAttr(((sel_end_y-1)/8)*sizechar_i+i,Graphic2->GetAttr((sel_start_y/8)*sizechar_i+i));
                                        }
                                }
                        for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
					}
                                }
                        }
                }
        else    // 1 pixel
                {
                if (sel_end_y-sel_start_y<2)
                        {
                        return;
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y-1;j++)
                                {
                                if(Graphic4->GetPixel(i,j+1)) Graphic3->SetPixel(i,j);
                                }
                        }
                if (eff_arr_shift==FALSE)
                        {
                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                if(Graphic4->GetPixel(i,sel_start_y)) Graphic3->SetPixel(i,sel_end_y-1);
                                }
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }

                }

        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Down rotation/shift
void TheFrame::EffRot_down(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;
        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if (eff_arr_char==TRUE)
                {
                if (sel_end_y-sel_start_y<9)
                        {
                        return;
                        }
                if ((eff_pix==1)||(Mask_view==1)) // 8 pixels
                        {
                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y+8;j<sel_end_y;j++)
                                        {
                                        if(Graphic4->GetPixel(i,j-8)) Graphic3->SetPixel(i,j);
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int i=sel_start_x;i<sel_end_x;i++)
                                        {
                                        for (int j=0;j<8;j++)
                                                {
                                                if(Graphic4->GetPixel(i,sel_end_y-8+j)) Graphic3->SetPixel(i,sel_start_y+j);
                                                }
                                        }
                                }

                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                for (int j=sel_start_y;j<sel_end_y;j++)
                                        {
                                        if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                        else                        Graphic4->ResetPixel(i,j);
                                        }
                                }
                        }
                if ((eff_attr==1)&&(Mask_view==0))        // 1 attr char
                        {
                        for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                {
                                for (int j=((sel_start_y+8)/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic3->SetAttr(j*sizechar_i+i,Graphic2->GetAttr((j-1)*sizechar_i+i));
                                        }
                                }
                        if (eff_arr_shift==FALSE)
                                {
                                for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                        {
                                        Graphic3->SetAttr((sel_start_y/8)*sizechar_i+i,Graphic2->GetAttr(((sel_end_y-1)/8)*sizechar_i+i));
                                        }
                                }
                        for (int i=(sel_start_x/8);i<((sel_end_x+7)/8);i++)
                                {
                                for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                        {
                                        Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                        }
                                }
                        }
                }
        else    // 1 pixel
                {
                if (sel_end_y-sel_start_y<2)
                        {
                        return;
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y+1;j<sel_end_y;j++)
                                {
                                if(Graphic4->GetPixel(i,j-1)) Graphic3->SetPixel(i,j);
                                }
                        }
                if (eff_arr_shift==FALSE)
                        {
                        for (int i=sel_start_x;i<sel_end_x;i++)
                                {
                                if(Graphic4->GetPixel(i,sel_end_y-1)) Graphic3->SetPixel(i,sel_start_y);
                                }
                        }
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }

                }

        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Clockwise rotation
void TheFrame::EffRot_clock(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        int size_rot_pix, size_rot_chr;

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }
        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        size_rot_pix = sel_end_x-sel_start_x;
        if (size_rot_pix>(sel_end_y-sel_start_y)) size_rot_pix = sel_end_y-sel_start_y;

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_start_x+size_rot_pix+7)/8;
        seyc=(sel_start_y+size_rot_pix+7)/8;

        size_rot_chr = sexc-ssxc;
        if (size_rot_chr>(seyc-ssyc)) size_rot_pix = seyc-ssyc;

        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if ((eff_pix==1)||(Mask_view==1))
                {
                for (int i=0;i<size_rot_pix;i++)
                        {
                        for (int j=0;j<size_rot_pix;j++)
                                {
                                if(Graphic4->GetPixel(sel_start_x+i,sel_start_y+j)) Graphic3->SetPixel(sel_start_x+size_rot_pix-j-1,sel_start_y+i);
                                }
                        }
                for (int i=sel_start_x;i<sel_start_x+size_rot_pix;i++)
                        {
                        for (int j=sel_start_y;j<sel_start_y+size_rot_pix;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }
                }
        if ((eff_attr==1)&&(Mask_view==0)&&((sel_start_x+size_rot_pix)%8==0)&&((sel_start_y+size_rot_pix)%8==0))
                {
                for (int i=0;i<size_rot_chr;i++)
                        {
                        for (int j=0;j<size_rot_chr;j++)
                                {
                                Graphic3->SetAttr((j+ssyc)*sizechar_i+i+ssxc,Graphic2->GetAttr((ssyc-i+size_rot_chr-1)*sizechar_i+ssxc+j));
                                }
                        }
                for (int i=ssxc;i<ssxc+size_rot_chr;i++)
                        {
                        for (int j=ssyc;j<ssyc+size_rot_chr;j++)
                                {
                                Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                }
                        }
                }
        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Anticlockwise rotation
void TheFrame::EffRot_anticlock(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        SaveUndo();

        int size_rot_pix, size_rot_chr;

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }
        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                }

        size_rot_pix = sel_end_x-sel_start_x;
        if (size_rot_pix>(sel_end_y-sel_start_y)) size_rot_pix = sel_end_y-sel_start_y;

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_start_x+size_rot_pix+7)/8;
        seyc=(sel_start_y+size_rot_pix+7)/8;

        size_rot_chr = sexc-ssxc;
        if (size_rot_chr>(seyc-ssyc)) size_rot_pix = seyc-ssyc;

        wxClientDC dc(canvas2);

        SP_Graph *Graphic3=new SP_Graph(sizechar_i*8,sizechar_j*8);

        if ((eff_pix==1)||(Mask_view==1))
                {
                for (int i=0;i<size_rot_pix;i++)
                        {
                        for (int j=0;j<size_rot_pix;j++)
                                {
                                if(Graphic4->GetPixel(sel_start_x+i,sel_start_y+j)) Graphic3->SetPixel(sel_start_x+j,sel_start_y+size_rot_pix-1-i);
                                }
                        }
                for (int i=sel_start_x;i<sel_start_x+size_rot_pix;i++)
                        {
                        for (int j=sel_start_y;j<sel_start_y+size_rot_pix;j++)
                                {
                                if(Graphic3->GetPixel(i,j)) Graphic4->SetPixel(i,j);
                                else                        Graphic4->ResetPixel(i,j);
                                }
                        }
                }
        if ((eff_attr==1)&&(Mask_view==0)&&((sel_start_x+size_rot_pix)%8==0)&&((sel_start_y+size_rot_pix)%8==0))
                {
                for (int i=0;i<size_rot_chr;i++)
                        {
                        for (int j=0;j<size_rot_chr;j++)
                                {
                                Graphic3->SetAttr((j+ssyc)*sizechar_i+i+ssxc,Graphic2->GetAttr((ssyc+i)*sizechar_i+ssxc+size_rot_chr-j-1));
                                }
                        }
                for (int i=ssxc;i<ssxc+size_rot_chr;i++)
                        {
                        for (int j=ssyc;j<ssyc+size_rot_chr;j++)
                                {
                                Graphic2->SetAttr(j*sizechar_i+i,Graphic3->GetAttr(j*sizechar_i+i));
                                }
                        }
                }

        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// Show effects options dialog
void TheFrame::EffOpt(wxCommandEvent &event)
{
        GetEffectsOptions();
}

// MASK MENU FUNCTIONS

// Toggle mask use, creating bitmaps if necessary
void TheFrame::MaskUse(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                Mask_use=0;
                menuMask->Check(MASK_USE,FALSE);
                return;
                }
        switch (Mask_use)
                {
                case 0: {
                        menuMask->Check(MASK_USE,TRUE);
                        Mask_use=1;
                        Current->OpenPropierties=2;

                        SP_Graph *temp;
                        temp=Graphic2;
                        Graphic2=Current->OpenGraph;
                        for (int framm=0;framm<Frame_total;framm++)
                                {
                                if (Graphic2->Mask==NULL)
                                        {
                                        Graphic2->Mask = new SP_Graph(size_i,size_j,TRUE);
                                        DrawArea(0,0,sizechar_i,sizechar_j); // Paint first void mask...
                                        }
                                Graphic2=Graphic2->NextGraph;
                                }
                        Graphic2=temp;
                        break;
                        }
                case 1: {
                        menuMask->Check(MASK_USE,FALSE);
                        menuMask->Check(MASK_VIEW,FALSE);
                        Mask_use=0;
                        Mask_view=0;
                        Current->OpenPropierties=1;
                        break;
                        }
                }
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// Toggle mask view mode
void TheFrame::MaskView(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                Mask_use=0;
                menuMask->Check(MASK_USE,FALSE);
                Mask_view=0;
                menuMask->Check(MASK_VIEW,FALSE);
                return;
                }
        if (Mask_use==0)
                {
                Mask_view=0;
                menuMask->Check(MASK_VIEW,FALSE);
                return;
                }
        switch (Mask_view)
                {
                case 0: {
                        menuMask->Check(MASK_VIEW,TRUE);
                        Mask_view=1;
                        break;
                        }
                case 1: {
                        menuMask->Check(MASK_VIEW,FALSE);
                        Mask_view=0;
                        break;
                        }
                }
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

void TheFrame::MaskAuto(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if (Mask_use==0)
                {
                return;
                }
        SaveUndo();
        int Maskviewold=Mask_view;
        Mask_view=1;
        Current->flagsure=1;

        for (int i=0;i<size_i;i++)
                for (int j=0;j<size_j;j++)
                        {
                        if (Graphic2->GetPixel(i,j)==1)
                                {
                                Graphic2->Mask->SetPixel(i,j);
                                }
                        else
                                {
                                Graphic2->Mask->ResetPixel(i,j);
                                }
                        }
        for (int i=0;i<size_i;i++)
                {
                PixelAction(7,i,0);
                PixelAction(7,i,size_j-1);
                }
        for (int j=1;j<size_j-1;j++)
                {
                PixelAction(7,0,j);
                PixelAction(7,size_i-1,j);
                }
        Mask_view=Maskviewold;

        for (int i=0;i<size_i;i++)
                for (int j=0;j<size_j;j++)
                        {
                        if (Graphic2->GetPixel(i,j)==1)
                                {
                                Graphic2->Mask->ResetPixel(i,j);
                                }
                        }
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}


// SPRITE MENU FUNCTIONS

// Insert next frame in current sprite
void TheFrame::SpriteInsertNext(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if ((Frame_total==999)||(17*(Frame_total+1)*sizechar_i*sizechar_j>41984))
                {
                (void)wxMessageBox(_("Max number of frames reached"),_("SevenuP"));
                return;
                }
        SP_Graph *NewGra = new SP_Graph (Current->OpenGraph->GetSizeX(),Current->OpenGraph->GetSizeY(),TRUE);
        if (Current->OpenPropierties==2)
                {
                NewGra->Mask=new SP_Graph (Current->OpenGraph->GetSizeX(),Current->OpenGraph->GetSizeY(),TRUE);
                }
        NewGra->NextGraph=Graphic2->NextGraph;
        Graphic2->NextGraph=NewGra;
        Current->OpenFrames++;
        Frame_total++;
        Frame_current++;
        Graphic2=Graphic2->NextGraph;
        SetTit();
        ClearUndo();
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// Insert previous frame in current sprite
void TheFrame::SpriteInsertPrev(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if ((Frame_total==999)||(17*(Frame_total+1)*sizechar_i*sizechar_j>41984))
                {
                (void)wxMessageBox(_("Max number of frames reached"),_("SevenuP"));
                return;
                }
        SP_Graph *NewGra = new SP_Graph (Current->OpenGraph->GetSizeX(),Current->OpenGraph->GetSizeY(),TRUE);
        if (Current->OpenPropierties==2)
                {
                NewGra->Mask=new SP_Graph (Current->OpenGraph->GetSizeX(),Current->OpenGraph->GetSizeY(),TRUE);
                }
        if (Frame_current==1)
                {
                NewGra->NextGraph=Graphic2;
                Current->OpenGraph=NewGra;
                }
        else
                {
                Graphic2=Current->OpenGraph;
                for (int frms=0;frms<(Frame_current-2);frms++)
                        {
                        Graphic2=Graphic2->NextGraph;
                        }
                NewGra->NextGraph=Graphic2->NextGraph;
                Graphic2->NextGraph=NewGra;
                }
        Graphic2=NewGra;
        Current->OpenFrames++;
        Frame_total++;
        SetTit();
        ClearUndo();
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}


// Change position of current frame in sprite
void TheFrame::SpriteMove(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if (Frame_total==1)
                {
                return;
                }
        long newpos=wxGetNumberFromUser(_(""),_("New frame position:"),_("Move frame"),1,1,Frame_total,this);

        if (newpos==-1)
                {
                return;
                }

        SP_Graph *GraphMove=Graphic2;

        if (Frame_current==1)
                {
                Current->OpenGraph=GraphMove->NextGraph;
                }
        else
                {
                        Graphic2=Current->OpenGraph;
                        for (int frms=0;frms<(Frame_current-2);frms++)
                                {
                                Graphic2=Graphic2->NextGraph;
                                }
                        Graphic2->NextGraph=GraphMove->NextGraph;
                }

        if (newpos==1)
                {
                GraphMove->NextGraph=Current->OpenGraph;
                Current->OpenGraph=GraphMove;
                }
        else
                {
                Graphic2=Current->OpenGraph;
                for (int frms=0;frms<(newpos-2);frms++)
                        {
                        Graphic2=Graphic2->NextGraph;
                        }
                GraphMove->NextGraph=Graphic2->NextGraph;
                Graphic2->NextGraph=GraphMove;
                }
        Frame_current=newpos;
        Graphic2=GraphMove;
        SetTit();
}


// Remove current frame in sprite
void TheFrame::SpriteRemove(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if (Frame_total==1)
                {
                return;
                }
        wxMessageDialog dialog( this,_("This action can't be reversed\nAre you sure?"),_("Warning"),wxOK|wxCANCEL);
        dialog.CentreOnParent();
	if (Warn_spriteremove==1)
		{
		if (dialog.ShowModal() == wxID_CANCEL)
                	{
                	return;
                	}
		}
        SP_Graph *GraphErase=Graphic2;

        if (Frame_current==1)
                {
                Current->OpenGraph=GraphErase->NextGraph;
                }
        else
                {
                Graphic2=Current->OpenGraph;
                for (int frms=0;frms<(Frame_current-2);frms++)
                        {
                        Graphic2=Graphic2->NextGraph;
                        }
                Graphic2->NextGraph=GraphErase->NextGraph;
                }
        if (Frame_current!=Frame_total)         Graphic2=GraphErase->NextGraph;
        else                                    Frame_current--;
        Frame_total--;
        Current->OpenFrames--;
        SetTit();
        ClearUndo();
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
        GraphErase->NextGraph=NULL;
        delete GraphErase;
}

// Select previous frame of current sprite
void TheFrame::SpritePrev(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if (Frame_current==1)
                {
                Frame_current=Frame_total;
                }
        else
                {
                Frame_current--;
                }
        Graphic2=Current->OpenGraph;
        for (int frms=0;frms<(Frame_current-1);frms++)
                {
                Graphic2=Graphic2->NextGraph;
                }
        SetTit();
        ClearUndo();
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}


// Select next frame of current sprite
void TheFrame::SpriteNext(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if (Frame_current==Frame_total)
                {
                Frame_current=1;
                Graphic2=Current->OpenGraph;
                }
        else
                {
                Frame_current++;
                Graphic2=Graphic2->NextGraph;
                }
        SetTit();
        ClearUndo();
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// Select next frame of current sprite
void TheFrame::SpriteGoto(wxCommandEvent &event)
{
        if (Files_open==0)
                {
                return;
                }
        if (Frame_total==1)
                {
                return;
                }
        long newpos=wxGetNumberFromUser(_(""),_("Go to frame:"),_("Select frame"),1,1,Frame_total,this);

        if (newpos==-1)
                {
                return;
                }
        ClearUndo();
        Frame_current=newpos;
        Graphic2=Current->OpenGraph;
        for (int frms=0;frms<(Frame_current-1);frms++)
                {
                Graphic2=Graphic2->NextGraph;
                }
        SetTit();
        DrawArea(0,0,sizechar_i,sizechar_j);
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

// GRID MENU FUNCTIONS

// Toggle character grid and refresh screen
void TheFrame::GridChar(wxCommandEvent &event)
{
        gridchr=!gridchr;
        if (zoom>3)
                {
                wxClientDC dc(canvas2);
                Rfrsh(dc,TRUE);
                }
}

// Toggle pixel grid and refresh screen
void TheFrame::GridPixel(wxCommandEvent &event)
{
        gridpix=!gridpix;
        if (zoom>3)
                {
                wxClientDC dc(canvas2);
                Rfrsh(dc,TRUE);
                }
}

// ZOOM MENU FUNCTIONS

// Toggle AutoZoom on load feature
void TheFrame::ZoomAuto(wxCommandEvent &event)
{
        AutoZoom=!AutoZoom;
}

// Set an instant zoom
void TheFrame::SetZoom(wxCommandEvent &event)
{
        menuBar->Check(ZOOM_1+zoom,FALSE);
        int oldzoom = zoom;
        zoom=event.GetId()-ZOOM_1;
        menuBar->Check(ZOOM_1+zoom,TRUE);
        if (zoom==oldzoom) return;
        if (zoom == 0) toolBar->EnableTool(ZOOM_ZOOMOUT, FALSE);
        if (oldzoom == 0) toolBar->EnableTool(ZOOM_ZOOMOUT, TRUE);
        if (zoom == ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN, FALSE);
        if (oldzoom == ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN, TRUE);
        canvas->GetViewStart(&startx,&starty);
        size_x = size_i*ZoomValue[zoom];
        size_y = size_j*ZoomValue[zoom];
        disablezoom=1;
	canvas->Scroll(0,0);
	canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        canvas->Scroll(startx*ZoomValue[zoom]/ZoomValue[oldzoom],starty*ZoomValue[zoom]/ZoomValue[oldzoom]);
        wxClientDC dc(canvas2);
        disablezoom=0;
	Rfrsh(dc,TRUE);
}

// INFO MENU FUNCTIONS

// Inform about the current version
void TheFrame::InfoAbout(wxCommandEvent &event)
{
        (void)wxMessageBox(_("SevenuP - version 1.21\n(C) 2002-2007 by Metalbrain"),_("About SevenuP"));
}

// TOOLBAR FUNCTIONS

// Increase zoom level

void TheFrame::ZoomIn(wxCommandEvent &event)
{
        DoZoomIn();
}

void TheFrame::DoZoomIn(void)
{
        if (zoom == (ZOOMLEVELS-1)) return;
        menuBar->Check(ZOOM_1+zoom,FALSE);
        zoom++;
        menuBar->Check(ZOOM_1+zoom,TRUE);
        if (zoom == 1) toolBar->EnableTool(ZOOM_ZOOMOUT, TRUE);
        if (zoom == (ZOOMLEVELS-1)) toolBar->EnableTool(ZOOM_ZOOMIN, FALSE);
        canvas->GetViewStart(&startx,&starty);
        size_x = size_i*ZoomValue[zoom];
        size_y = size_j*ZoomValue[zoom];
        disablezoom=1;
	canvas->Scroll(0,0);
	canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        canvas->Scroll(startx*ZoomValue[zoom]/ZoomValue[zoom-1],starty*ZoomValue[zoom]/ZoomValue[zoom-1]);
        wxClientDC dc(canvas2);
        disablezoom=0;
	Rfrsh(dc,TRUE);
}

// Decrease zoom level

void TheFrame::ZoomOut(wxCommandEvent &event)
{
        DoZoomOut();
}
  
void TheFrame::DoZoomOut(void)
{
        if (zoom == 0) return;
        menuBar->Check(ZOOM_1+zoom,FALSE);
        zoom--;
        menuBar->Check(ZOOM_1+zoom,TRUE);
        if (zoom == 0)  toolBar->EnableTool(ZOOM_ZOOMOUT, FALSE);
        if (zoom == (ZOOMLEVELS-2)) toolBar->EnableTool(ZOOM_ZOOMIN, TRUE);

        canvas->GetViewStart(&startx,&starty);
        size_x = size_i*ZoomValue[zoom];
        size_y = size_j*ZoomValue[zoom];
        disablezoom=1;
	canvas->Scroll(0,0);
	canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        canvas->Scroll(startx*ZoomValue[zoom]/ZoomValue[zoom+1],starty*ZoomValue[zoom]/ZoomValue[zoom+1]);
        wxClientDC dc(canvas2);
	disablezoom=0;

	Rfrsh(dc,TRUE);
}

// Change cursor mode
void TheFrame::ChangeCursor(wxCommandEvent &event)
{
        switch (cursormode)
                {
                case 0: {
                        cursormode++;
                        left_func=3;
                        right_func=4;
                        switch (sselect)
                                {
                                case 0: {
                                        SetStatusText(_("Toggle Pixel/Select zone"),0);
                                        break;
                                        }
                                case 1: {
                                        SetStatusText(_("Toggle Pixel/Close selection"),0);
                                        break;
                                        }
                                case 2: {
                                        SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                        break;
                                        }
                                }
                        break;
                        }
                case 1: {
                        cursormode++;
                        left_func=5;
                        right_func=6;
                        if ((Mask_use==1)&&(Mask_view==1))
                                {
                                SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                }
                        else
                                {
                                SetStatusText(_("Set Attributes/Get Attributes"),0);
                                }
                        break;
                        }
                case 2:
                case 3:
                case 4:
                case 5: {
                        cursormode=0;
                        left_func=1;
                        right_func=2;
                        SetStatusText(_("Set Pixel/Reset Pixel"),0);
                        break;
                        }
                }
}

// Erase graphic or area
void TheFrame::Erase(wxCommandEvent &event)
{
        if (Files_open==0) return;
        Current->flagsure=1;
        wxClientDC dc(canvas2);

        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }

        if (sselect!=2)
                {
                sel_start_x=0;
                sel_start_y=0;
                sel_end_x=size_i;
                sel_end_y=size_j;
                sselect=0;
                if (cursormode==1) SetStatusText(_("Toggle Pixel/Select zone"),0);
                }

        ssxc=sel_start_x/8;
        ssyc=sel_start_y/8;
        sexc=(sel_end_x+7)/8;
        seyc=(sel_end_y+7)/8;

        SaveUndo();
        if ((eff_pix==1)||(Mask_view==1))
                {
                for (int i=sel_start_x;i<sel_end_x;i++)
                        {
                        for (int j=sel_start_y;j<sel_end_y;j++)
                                {
                                Graphic4->ResetPixel(i,j);
                                }
                        }
                }
        if ((eff_attr==1)&&(Mask_view==0))
                {
                for (int i=((sel_start_x)/8);i<((sel_end_x+7)/8);i++)
                        {
                        for (int j=(sel_start_y/8);j<((sel_end_y+7)/8);j++)
                                {
                                Graphic2->SetAttr(j*sizechar_i+i,56);
                                }
                        }
                }
        DrawArea(ssxc,ssyc,sexc,seyc);
        MacroRfrsh(dc,TRUE);
}

// COLOR FUNCTIONS

// Process ink button pressing
void TheFrame::ButtonINK(wxCommandEvent &event)
{
        Butt_ink[col_ink]->Enable( TRUE );
        col_ink=event.GetId()-ID_BITMAPBUTTONI0;
        Butt_ink[col_ink]->Enable( FALSE );
}

// Process paper button pressing
void TheFrame::ButtonPAPER(wxCommandEvent &event)
{
        Butt_paper[col_paper]->Enable( TRUE );
        col_paper=event.GetId()-ID_BITMAPBUTTONP0;
        Butt_paper[col_paper]->Enable( FALSE );
}

// Process bright button pressing
void TheFrame::ButtonBRIGHT(wxCommandEvent &event)
{
        Butt_bright[col_bright]->Enable( TRUE );
        col_bright=event.GetId()-ID_BITMAPBUTTONB0;
        Butt_bright[col_bright]->Enable( FALSE );
}

// Process flash button pressing
void TheFrame::ButtonFLASH(wxCommandEvent &event)
{
        Butt_flash[col_flash]->Enable( TRUE );
        col_flash=event.GetId()-ID_BITMAPBUTTONF0;
        Butt_flash[col_flash]->Enable( FALSE );
}

// Set a certain ink value
void TheFrame::SetSelectInk(int c)
{
        Butt_ink[col_ink]->Enable( TRUE );
        col_ink=c;
        Butt_ink[col_ink]->Enable( FALSE );
}

// Set a certain paper value
void TheFrame::SetSelectPaper(int c)
{
        Butt_paper[col_paper]->Enable( TRUE );
        col_paper=c;
        Butt_paper[col_paper]->Enable( FALSE );
}

// Set a certain bright value
void TheFrame::SetSelectBright(int c)
{
        Butt_bright[col_bright]->Enable( TRUE );
        col_bright=c;
        Butt_bright[col_bright]->Enable( FALSE );
}

// Set a certain flash value
void TheFrame::SetSelectFlash(int c)
{
        Butt_flash[col_flash]->Enable( TRUE );
        col_flash=c;
        Butt_flash[col_flash]->Enable( FALSE );
}

// SCREEN FUNCTIONS

// Draw a pixel in small bitmaps
void TheFrame::DrawCell(int i, int j)
{
        int norcolmal = Graphic2->GetPixelColour(i,j);

        wxColour normalcol(RedPal[norcolmal],GreenPal[norcolmal],BluePal[norcolmal]);

        if (sselect==2)
                {
                if ( (i<sel_end_x) && (j<sel_end_y) && (i>(sel_start_x-1)) && (j>(sel_start_y-1)) )
                        {
                        normalcol.Set(RedPalSel[norcolmal],GreenPalSel[norcolmal],BluePalSel[norcolmal]);
                        }
                }
        wxPen normalpen(normalcol,1,wxSOLID);
        wxBrush brush(normalcol,wxSOLID);

        wxMemoryDC temp_dc;

	// Conditional compilation code
	#if defined (__WXMSW__)
	temp_dc.SetBrush(brush);	// Win Only
	temp_dc.SetPen(normalpen);	// Win Only
	#endif

        for (int z=0;z<ZOOMBITMAPS;z++)
                {
                temp_dc.SelectObject(*Graphic2->SP_Screen[z]);
		// Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
	        temp_dc.SetBrush(brush);	// Linux only
	        temp_dc.SetPen(normalpen);	// Linux only
#endif
                int sqsize=ZoomValue[z];
                if (sqsize==1)  temp_dc.DrawPoint(i,j);
                else            temp_dc.DrawRectangle(i*sqsize,j*sqsize,sqsize,sqsize);
                temp_dc.SelectObject(wxNullBitmap);
                }
        if (Mask_use==1) // If masked, update mask bitmaps too
                {
                norcolmal = MaskPal[Graphic2->GetMaskColour(i,j)];
                normalcol.Set(RedPal[norcolmal],GreenPal[norcolmal],BluePal[norcolmal]);
                if (sselect==2)
                        {
                        if ( (i<sel_end_x) && (j<sel_end_y) && (i>(sel_start_x-1)) && (j>(sel_start_y-1)) )
                                {
                                normalcol.Set(RedPalSel[norcolmal],GreenPalSel[norcolmal],BluePalSel[norcolmal]);
                                }
                        }
                normalpen.SetColour(normalcol);
                brush.SetColour(normalcol);

		// Conditional compilation code
		#if defined (__WXMSW__)
                temp_dc.SetBrush(brush);	// Win only
                temp_dc.SetPen(normalpen);	// Win only
		#endif

		for (int z=0;z<ZOOMBITMAPS;z++)
                        {
                        temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[z]);
			// Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
			temp_dc.SetBrush(brush);	// Linux only
			temp_dc.SetPen(normalpen);	// Linux only
#endif
                        int sqsize=ZoomValue[z];
                        if (sqsize==1)  temp_dc.DrawPoint(i,j);
                        else            temp_dc.DrawRectangle(i*sqsize,j*sqsize,sqsize,sqsize);
                        temp_dc.SelectObject(wxNullBitmap);
                        }
                }
}

// Draw a pixel in big zoom modes
void TheFrame::DrawBigCell(int i, int j)
{
        int norcolmal,grdcolpix,grdcolchr;
        int i2,j2;
        wxMemoryDC temp_dc;

        int sqsize=ZoomValue[zoom];

        i2=i-startx/sqsize;
        j2=j-starty/sqsize;

        if ((Mask_use==1)&&(Mask_view==1))
                {
                norcolmal = MaskPal[Graphic2->GetMaskColour(i,j)];
                }
        else
                {
                norcolmal = Graphic2->GetPixelColour(i,j);
                }

        wxColour normalcol(RedPal[norcolmal],GreenPal[norcolmal],BluePal[norcolmal]);

        if (sselect==2)
                {
                if ( (i<sel_end_x) && (j<sel_end_y) && (i>(sel_start_x-1)) && (j>(sel_start_y-1)) )
                        {
                        normalcol.Set(RedPalSel[norcolmal],GreenPalSel[norcolmal],BluePalSel[norcolmal]);
                        }
                }
        wxPen normalpen(normalcol,1,wxSOLID);
        wxBrush brush(normalcol,wxSOLID);

        temp_dc.SelectObject(bigescreen);
        temp_dc.SetBrush(brush);
        temp_dc.SetPen(normalpen);
        temp_dc.DrawRectangle(i2*sqsize,j2*sqsize,sqsize,sqsize);

        if (zoom<4) return;

        if (gridchr&&gridpix)
                {
                grdcolpix = ((Mask_use==1)&&(Mask_view==1))?19:17;
                grdcolchr = ((Mask_use==1)&&(Mask_view==1))?18:16;
                wxColour grdpixcol(RedPal[grdcolpix],GreenPal[grdcolpix],BluePal[grdcolpix]);
                wxColour grdchrcol(RedPal[grdcolchr],GreenPal[grdcolchr],BluePal[grdcolchr]);
                if (sselect==2)
                        {
                        if ( (i<sel_end_x) && (j<sel_end_y) && (i>(sel_start_x-1)) && (j>(sel_start_y-1)) )
                                {
                                grdpixcol.Set(RedPalSel[grdcolpix],GreenPalSel[grdcolpix],BluePalSel[grdcolpix]);
                                grdchrcol.Set(RedPalSel[grdcolchr],GreenPalSel[grdcolchr],BluePalSel[grdcolchr]);
                                }
                        }
                wxPen grdpixpen(grdpixcol,1,wxSOLID);
                wxPen grdchrpen(grdchrcol,1,wxSOLID);
                if (i%8!=7)     temp_dc.SetPen(grdchrpen);
                else            temp_dc.SetPen(grdpixpen);
                temp_dc.DrawLine(i2*sqsize+sqsize-1,j2*sqsize,i2*sqsize+sqsize-1,j2*sqsize+sqsize);
                if ((j%8==7)!=(i%8==7))
                        {
                        if (j%8!=7)     temp_dc.SetPen(grdchrpen);
                        else            temp_dc.SetPen(grdpixpen);
                        }
                temp_dc.DrawLine(i2*sqsize,j2*sqsize+sqsize-1,i2*sqsize+sqsize-(i%8==7),j2*sqsize+sqsize-1);
                }
        if (gridchr&&!gridpix&&(i%8==7||j%8==7))
                {
                grdcolpix = ((Mask_use==1)&&(Mask_view==1))?19:17;
                wxColour grdpixcol(RedPal[grdcolpix],GreenPal[grdcolpix],BluePal[grdcolpix]);
                if (sselect==2)
                        {
                        if ( (i<sel_end_x) && (j<sel_end_y) && (i>(sel_start_x-1)) && (j>(sel_start_y-1)) )
                                {
                                grdpixcol.Set(RedPalSel[grdcolpix],GreenPalSel[grdcolpix],BluePalSel[grdcolpix]);
                                }
                        }
                wxPen grdpixpen(grdpixcol,1,wxSOLID);
                temp_dc.SetPen(grdpixpen);
                if (i%8==7) temp_dc.DrawLine(i2*sqsize+sqsize-1,j2*sqsize,i2*sqsize+sqsize-1,j2*sqsize+sqsize);
                if (j%8==7) temp_dc.DrawLine(i2*sqsize,j2*sqsize+sqsize-1,i2*sqsize+sqsize,j2*sqsize+sqsize-1);
                }
        if (gridpix&&!gridchr)
                {
                grdcolpix = ((Mask_use==1)&&(Mask_view==1))?18:16;
                wxColour grdpixcol(RedPal[grdcolpix],GreenPal[grdcolpix],BluePal[grdcolpix]);
                if (sselect==2)
                        {
                        if ( (i<sel_end_x) && (j<sel_end_y) && (i>(sel_start_x-1)) && (j>(sel_start_y-1)) )
                                {
                                grdpixcol.Set(RedPalSel[grdcolpix],GreenPalSel[grdcolpix],BluePalSel[grdcolpix]);
                                }
                        }
                wxPen grdpixpen(grdpixcol,1,wxSOLID);
                temp_dc.SetPen(grdpixpen);
                temp_dc.DrawLine(i2*sqsize+sqsize-1,j2*sqsize,i2*sqsize+sqsize-1,j2*sqsize+sqsize);
                temp_dc.DrawLine(i2*sqsize,j2*sqsize+sqsize-1,i2*sqsize+sqsize,j2*sqsize+sqsize-1);
                }
        temp_dc.SelectObject(wxNullBitmap);
}

// Draw an area in small bitmaps
void TheFrame::DrawArea(int i, int j, int i2, int j2)
{
        wxMemoryDC temp_dc;
        wxColour charcolours[8];
        wxPen pens[8];
        wxBrush brushes[8];

        for (int z=0;z<ZOOMBITMAPS;z++)
                {
                temp_dc.SelectObject(*Graphic2->SP_Screen[z]);
                int sqsize=ZoomValue[z];
                for (int x=i;x<i2;x++)
                        {
                        for (int y=j;y<j2;y++)
                                {
                                int color_bit0 = Graphic2->GetPaper(x,y)+8*Graphic2->GetBright(x,y);
                                int color_bit1 = Graphic2->GetInk(x,y)+8*Graphic2->GetBright(x,y);

                                charcolours[0] = wxColour(RedPal[color_bit0],GreenPal[color_bit0],BluePal[color_bit0]);
                                charcolours[1] = wxColour(RedPal[color_bit1],GreenPal[color_bit1],BluePal[color_bit1]);
                                charcolours[2] = wxColour(RedPalSel[color_bit0],GreenPalSel[color_bit0],BluePalSel[color_bit0]);
                                charcolours[3] = wxColour(RedPalSel[color_bit1],GreenPalSel[color_bit1],BluePalSel[color_bit1]);

                                pens[0]=wxPen(charcolours[0],1,wxSOLID);
                                pens[1]=wxPen(charcolours[1],1,wxSOLID);
                                pens[2]=wxPen(charcolours[2],1,wxSOLID);
                                pens[3]=wxPen(charcolours[3],1,wxSOLID);
                                brushes[0]=wxBrush(charcolours[0],wxSOLID);
                                brushes[1]=wxBrush(charcolours[1],wxSOLID);
                                brushes[2]=wxBrush(charcolours[2],wxSOLID);
                                brushes[3]=wxBrush(charcolours[3],wxSOLID);
                                for (int xx=(x*8);xx<((x*8)+8);xx++)
                                        {
                                        for (int yy=(y*8);yy<((y*8)+8);yy++)
                                                {
                                                int color=Graphic2->GetPixel(xx,yy)+2*((sselect==2) && (xx<sel_end_x) && (yy<sel_end_y) && (xx>(sel_start_x-1)) && (yy>(sel_start_y-1)));
                                                temp_dc.SetBrush(brushes[color]);
                                                temp_dc.SetPen(pens[color]);
                                                if (sqsize==1)  temp_dc.DrawPoint(xx,yy);
                                                else            temp_dc.DrawRectangle(xx*sqsize,yy*sqsize,sqsize,sqsize);
                                                }
                                        }
                                }
                        }
                temp_dc.SelectObject(wxNullBitmap);
                }
        if (Mask_use==1)
                {
                for (int ini=0;ini<4;ini++)
                        {
                        charcolours[ini] = wxColour(RedPal[MaskPal[ini]],GreenPal[MaskPal[ini]],BluePal[MaskPal[ini]]);
                        charcolours[ini+4] = wxColour(RedPalSel[MaskPal[ini]],GreenPalSel[MaskPal[ini]],BluePalSel[MaskPal[ini]]);
                        pens[ini] = wxPen(charcolours[ini],1,wxSOLID);
                        pens[ini+4] = wxPen(charcolours[ini+4],1,wxSOLID);
                        brushes[ini]=wxBrush(charcolours[ini],wxSOLID);
                        brushes[ini+4]=wxBrush(charcolours[ini+4],wxSOLID);
                        }
                for (int z=0;z<ZOOMBITMAPS;z++)
                        {
                        temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[z]);
                        int sqsize=ZoomValue[z];
                        for (int x=i*8;x<i2*8;x++)
                                {
                                for (int y=j*8;y<j2*8;y++)
                                        {
                                        int color=Graphic2->GetMaskColour(x,y)+4*((sselect==2) && (x<sel_end_x) && (y<sel_end_y) && (x>(sel_start_x-1)) && (y>(sel_start_y-1)));
                                        temp_dc.SetBrush(brushes[color]);
                                        temp_dc.SetPen(pens[color]);
                                        if (sqsize==1)  temp_dc.DrawPoint(x,y);
                                        else            temp_dc.DrawRectangle(x*sqsize,y*sqsize,sqsize,sqsize);
                                        }
                                }
                        temp_dc.SelectObject(wxNullBitmap);
                        }
                }
}

// Refresh a certain area
void TheFrame::AreaRfrsh(wxDC& dc, bool draw_bitmaps)
{
        wxMemoryDC temp_dc;
        int startoffx,startoffy;
        canvas->GetViewStart(&startx,&starty);
        canvas->GetClientSize(&sizx,&sizy);
        if (zoom<ZOOMBITMAPS)
                {
                if (draw_bitmaps)
                        {
                        if (Mask_view==1) temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[zoom]);
                        else              temp_dc.SelectObject(*Graphic2->SP_Screen[zoom]);
                        dc.Blit(startx,starty,sizx,sizy,&temp_dc,startx,starty);
                        }
                }
        else
                {
                startoffx=startx%ZoomValue[zoom];
                startoffy=starty%ZoomValue[zoom];
                for (int i=ssxc;i<sexc;i++)
                        {
                        for (int j=ssyc;j<seyc;j++)
                                {
                                DrawBigCell(i, j);
                                };
                        };
                if (draw_bitmaps)
                        {
                        temp_dc.SelectObject(bigescreen);
                        dc.Blit(startx,starty,sizx,sizy,&temp_dc,startoffx,startoffy);
                        }
                }
        if (draw_bitmaps)
                {
                temp_dc.SelectObject(wxNullBitmap);
                oldstartx=startx;
                oldstarty=starty;
                oldsizx=sizx;
                oldsizy=sizy;
                }
}

// Refresh affected area
void TheFrame::MacroRfrsh(wxDC& dc, bool WXUNUSED(draw_bitmaps))
{
        wxMemoryDC temp_dc;
        int startoffx,startoffy;
        canvas->GetViewStart(&startx,&starty);
        canvas->GetClientSize(&sizx,&sizy);
        if (zoom<ZOOMBITMAPS)
                {
                if (Mask_view==1) temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[zoom]);
                else              temp_dc.SelectObject(*Graphic2->SP_Screen[zoom]);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startx,starty);
                }
        else
                {
                startoffx=startx%ZoomValue[zoom];
                startoffy=starty%ZoomValue[zoom];
                for (int i=ssxc*8;i<sexc*8;i++)
                        {
                        for (int j=ssyc*8;j<seyc*8;j++)
                                {
                                DrawBigCell(i, j);
                                }
                        }
                temp_dc.SelectObject(bigescreen);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startoffx,startoffy);
                }
        temp_dc.SelectObject(wxNullBitmap);
        oldstartx=startx;
        oldstarty=starty;
        oldsizx=sizx;
        oldsizy=sizy;
}

// Refresh a character (for instant colour clash)
void TheFrame::MiniRfrsh(wxDC& dc, int xc, int yc, bool WXUNUSED(draw_bitmaps))
{
        wxMemoryDC temp_dc;
        int startoffx,startoffy;
        canvas->GetViewStart(&startx,&starty);
        canvas->GetClientSize(&sizx,&sizy);

        if (zoom<ZOOMBITMAPS)
                {
                if (Mask_view==1) temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[zoom]);
                else              temp_dc.SelectObject(*Graphic2->SP_Screen[zoom]);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startx,starty);
                }
        else
                {
                startoffx=startx%ZoomValue[zoom];
                startoffy=starty%ZoomValue[zoom];
                for (int i=xc*8;i<xc*8+8;i++)
                        {
                        for (int j=yc*8;j<(yc*8+8);j++)
                                {
                                DrawBigCell(i, j);
                                };
                        };
                temp_dc.SelectObject(bigescreen);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startoffx,startoffy);
                }
        temp_dc.SelectObject(wxNullBitmap);
        oldstartx=startx;
        oldstarty=starty;
        oldsizx=sizx;
        oldsizy=sizy;
}

// Refresh a pixel
void TheFrame::MicroRfrsh(wxDC& dc, int x, int y, bool WXUNUSED(draw_bitmaps))
{
        wxMemoryDC temp_dc;
        int startoffx,startoffy;

        canvas->GetViewStart(&startx,&starty);
        canvas->GetClientSize(&sizx,&sizy);

        if (zoom<ZOOMBITMAPS)
                {
                if (Mask_view==1) temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[zoom]);
                else              temp_dc.SelectObject(*Graphic2->SP_Screen[zoom]);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startx,starty);
                }
        else
                {
                startoffx=startx%ZoomValue[zoom];
                startoffy=starty%ZoomValue[zoom];
                DrawBigCell(x, y);
                temp_dc.SelectObject(bigescreen);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startoffx,startoffy);
                }
        temp_dc.SelectObject(wxNullBitmap);
        oldstartx=startx;
        oldstarty=starty;
        oldsizx=sizx;
        oldsizy=sizy;
}

// Refresh window
void TheFrame::Rfrsh(wxDC& dc, bool draw_bitmaps)
{
        if (Files_open==0) return;
        wxMemoryDC temp_dc;

        int startoffx,startoffy,startcharx,startchary;
        int sizerelx,sizerely;

        canvas->GetViewStart(&startx,&starty);
        canvas->GetClientSize(&sizx,&sizy);

        if (zoom<ZOOMBITMAPS)
                {
                if (Mask_view==1) temp_dc.SelectObject(*Graphic2->Mask->SP_Screen[zoom]);
                else              temp_dc.SelectObject(*Graphic2->SP_Screen[zoom]);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startx,starty);
                }
        else
                {
                startoffx=startx%ZoomValue[zoom];
                startoffy=starty%ZoomValue[zoom];
                if (draw_bitmaps)
                        {
                        startcharx=startx/ZoomValue[zoom];
                        startchary=starty/ZoomValue[zoom];
                        sizerelx=(startx+sizx)/ZoomValue[zoom];
                        sizerely=(starty+sizy)/ZoomValue[zoom];
                        sizerelx++;
                        sizerely++;
                        if (sizerelx>size_i) sizerelx=size_i;
                        if (sizerely>size_j) sizerely=size_j;
                        for (int i=startcharx;i<sizerelx;i++)
                                {
                                for (int j=startchary;j<sizerely;j++)
                                        {
                                        DrawBigCell(i, j);
                                        };
                                };
                        }
                temp_dc.SelectObject(bigescreen);
                dc.Blit(startx,starty,sizx,sizy,&temp_dc,startoffx,startoffy);
                }
        temp_dc.SelectObject(wxNullBitmap);
        oldstartx=startx;
        oldstarty=starty;
        oldsizx=sizx;
        oldsizy=sizy;
}

void TheFrame::RfScroll(wxDC& dc)
{
        wxMemoryDC temp_dc;
        int startoffx,startoffy;
        startoffx=startx%ZoomValue[zoom];
        startoffy=starty%ZoomValue[zoom];
        temp_dc.SelectObject(bigescreen);
        temp_dc.Blit(startoffx,startoffy,sizx,sizy,&dc,startx,starty);
        temp_dc.SelectObject(wxNullBitmap);
}

// MORE FUNCTIONS

// The output options dialog
void TheFrame::GetOutputOptions()
        {
        priorities_temp[0]=priorities[0];
        priorities_temp[1]=priorities[1];
        priorities_temp[2]=priorities[2];
        priorities_temp[3]=priorities[3];
        priorities_temp[4]=priorities[4];

        wxString strs7[] = {strs5[priorities_temp[0]],strs5[priorities_temp[1]],strs5[priorities_temp[2]],strs5[priorities_temp[3]],strs5[priorities_temp[4]]};
        wxDialog dialogOutputOptions(this,-1,wxString(_("Set Output Options")));
                optitem0 = new wxBoxSizer( wxVERTICAL );
                optitem1 = new wxBoxSizer( wxHORIZONTAL );
                optitem2 = new wxRadioBox( &dialogOutputOptions, ID_SAVEOPT_RADIOBOX, _("Data Outputted"), wxDefaultPosition, wxDefaultSize, 4, strs4, 1, wxRA_SPECIFY_COLS );
                optitem15 = new wxRadioBox( &dialogOutputOptions, ID_SAVEOPT_INTERLEAVE, _("Interleave"), wxDefaultPosition, wxDefaultSize, 5, strs6, 1, wxRA_SPECIFY_COLS );
                optitem3 = new wxBoxSizer( wxVERTICAL );
                wxButton *optitem4 = new wxButton( &dialogOutputOptions, wxID_OK,_("OK"), wxDefaultPosition, wxDefaultSize, 0 );
                wxButton *optitem5 = new wxButton( &dialogOutputOptions, wxID_CANCEL,_("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem6 = new wxBoxSizer( wxHORIZONTAL );
                wxStaticBox *optitem8 = new wxStaticBox( &dialogOutputOptions, -1,_("Byte sort priority") );
                wxStaticBoxSizer *optitem7 = new wxStaticBoxSizer( optitem8, wxVERTICAL );
                optitem9 = new wxListBox( &dialogOutputOptions, ID_SAVEOPT_LISTBOX, wxDefaultPosition, wxSize(80,75), 5, strs7, wxLB_SINGLE );
                optitem10 = new wxBoxSizer( wxVERTICAL );
                wxButton *optitem11 = new wxButton( &dialogOutputOptions, ID_SAVEOPT_BUTTON_UP,_("Move up"), wxDefaultPosition, wxDefaultSize, 0 );
                wxButton *optitem12 = new wxButton( &dialogOutputOptions, ID_SAVEOPT_BUTTON_DOWN,_("Move Down"), wxDefaultPosition, wxDefaultSize, 0 );

                optitem13 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_MASK,_("Mask Before Graph"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem14 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_APPEND,_("Append Data"), wxDefaultPosition, wxDefaultSize, 0 );
                
                optitem16 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_ZIGZAG,_("Horizontal Zig Zag"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem17 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_ATMASK,_("Attribute Mask"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem18 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_ATMASKI,_("   Ink"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem19 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_ATMASKP,_("   Paper"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem20 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_ATMASKB,_("   Bright"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem21 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_ATMASKF,_("   Flash"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem22 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_Z88DK,_("Add z88dk extra info"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem24 = new wxCheckBox( &dialogOutputOptions, ID_SAVEOPT_NOLABEL,_("No ASM Label"), wxDefaultPosition, wxDefaultSize, 0 );
                optitem23 = new wxBoxSizer( wxHORIZONTAL );

                
                optitem1->Add( optitem2, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem3->Add( optitem13, 0, wxALIGN_CENTRE|wxALL, 5 );
		optitem3->Add( optitem17, 0, wxALIGN_CENTRE|wxALL, 5 );
		optitem3->Add( optitem18, 0, wxALIGN_CENTRE|wxALL, 5 );
		optitem3->Add( optitem19, 0, wxALIGN_CENTRE|wxALL, 5 );
		optitem3->Add( optitem20, 0, wxALIGN_CENTRE|wxALL, 5 );
		optitem3->Add( optitem21, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem1->Add( optitem3, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem0->Add( optitem1, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem7->Add( optitem9, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem6->Add( optitem7, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem10->Add( optitem11, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem10->Add( optitem12, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem6->Add( optitem10, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem6->Add( optitem15, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem0->Add( optitem6, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem0->Add( optitem14, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem0->Add( optitem16, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem0->Add( optitem22, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem0->Add( optitem24, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem23->Add( optitem4, 0, wxALIGN_CENTRE|wxALL, 5 );
                optitem23->Add( optitem5, 0, wxALIGN_CENTRE|wxALL, 5 );
		optitem0->Add( optitem23, 0, wxALIGN_CENTRE|wxALL, 5 );

		
                optitem2->SetSelection(optradioboxdef);
                optitem15->SetSelection(interleave-(interleave>3));
                optitem13->SetValue(maskfirst);
                optitem14->SetValue(appendfile);
                optitem16->SetValue(zigzag);
                optitem17->SetValue(AttrMask);
                optitem18->SetValue(AttrMaskInk);
		optitem19->SetValue(AttrMaskPaper);
		optitem20->SetValue(AttrMaskBright);
		optitem21->SetValue(AttrMaskFlash);
		optitem22->SetValue(z88dksize);
		optitem24->SetValue(nolabel);

                dialogOutputOptions.SetAutoLayout(TRUE);
                dialogOutputOptions.SetSizer(optitem0);
                optitem0->Fit(&dialogOutputOptions);

        dialogOutputOptions.SetExtraStyle(0);    // Reset wxWS_EX_BLOCK_EVENTS to enable button event
        dialogOutputOptions.CentreOnParent();
        if (dialogOutputOptions.ShowModal() == wxID_OK)
                {
                optradioboxdef=optitem2->GetSelection();
                interleave=optitem15->GetSelection()+(optitem15->GetSelection()>2);
                priorities[0]=priorities_temp[0];
                priorities[1]=priorities_temp[1];
                priorities[2]=priorities_temp[2];
                priorities[3]=priorities_temp[3];
                priorities[4]=priorities_temp[4];
                maskfirst=optitem13->GetValue();
                appendfile=optitem14->GetValue();
                zigzag=optitem16->GetValue();
                AttrMask=optitem17->GetValue();
                AttrMaskInk=optitem18->GetValue();
                AttrMaskPaper=optitem19->GetValue();
                AttrMaskBright=optitem20->GetValue();
                AttrMaskFlash=optitem21->GetValue();
                z88dksize=optitem22->GetValue();
                nolabel=optitem24->GetValue();
                }
        return;
        }

// Move down a sort priority option
void TheFrame::MoveOptDOWN(wxCommandEvent &event)
{
        int cccc=optitem9->GetSelection();
        if (cccc<4)
                {
                wxString strga=optitem9->GetString(cccc+1);
                wxString strgb=optitem9->GetString(cccc);
                int priora=priorities_temp[cccc];
                int priorb=priorities_temp[cccc+1];
                optitem9->SetString(cccc,strga);
                optitem9->SetString(cccc+1,strgb);
                optitem9->SetSelection(cccc,FALSE);
                optitem9->SetSelection(cccc+1,TRUE);
                priorities_temp[cccc]=priorb;
                priorities_temp[cccc+1]=priora;
                }
}

//Move up a sort priority option
void TheFrame::MoveOptUP(wxCommandEvent &event)
{
        int cccc=optitem9->GetSelection();
        if (cccc!=0)
                {
                wxString strga=optitem9->GetString(cccc-1);
                wxString strgb=optitem9->GetString(cccc);
                int priora=priorities_temp[cccc-1];
                int priorb=priorities_temp[cccc];
                optitem9->SetString(cccc,strga);
                optitem9->SetString(cccc-1,strgb);
                optitem9->SetSelection(cccc,FALSE);
                optitem9->SetSelection(cccc-1,TRUE);
                priorities_temp[cccc]=priora;
                priorities_temp[cccc-1]=priorb;
                }
}

// The effects options dialog
void TheFrame::GetEffectsOptions()
        {
        wxString strs4[] ={_("Pixels and Attributes"),_("Pixels"),_("Attributes")};
        wxDialog effectopt(this,-1,wxString(_("Set Effects Options")));

                effitem0 = new wxBoxSizer( wxVERTICAL );

                effitem1 = new wxRadioBox( &effectopt, ID_RADIOBOX_EFF,_("Effects affect to:"), wxDefaultPosition, wxDefaultSize, 3, strs4, 1, wxRA_SPECIFY_COLS );
                effitem2 = new wxCheckBox( &effectopt, ID_CHECKBOX_EFF1,_("Shift (instead rotation) "), wxDefaultPosition, wxDefaultSize, 0 );
                effitem3 = new wxCheckBox( &effectopt, ID_CHECKBOX_EFF2,_("Character rotation/shift"), wxDefaultPosition, wxDefaultSize, 0 );
                effitem4 = new wxBoxSizer( wxHORIZONTAL );
                wxButton *effitem5 = new wxButton( &effectopt, wxID_OK,_("OK"), wxDefaultPosition, wxDefaultSize, 0 );
                wxButton *effitem6 = new wxButton( &effectopt, wxID_CANCEL,_("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );

                effitem0->Add( effitem1, 0, wxALIGN_CENTRE|wxALL, 5 );
                effitem0->Add( effitem2, 0, wxALIGN_CENTRE|wxALL, 5 );
                effitem0->Add( effitem3, 0, wxALIGN_CENTRE|wxALL, 5 );
                effitem5->SetDefault();
                effitem4->Add( effitem5, 0, wxALIGN_CENTRE|wxALL, 5 );
                effitem4->Add( effitem6, 0, wxALIGN_CENTRE|wxALL, 5 );
                effitem0->Add( effitem4, 0, wxALIGN_CENTRE|wxALL, 5 );

                effitem1->SetSelection(effradioboxdef);
                effitem2->SetValue(eff_arr_shift);
                effitem3->SetValue(eff_arr_char);

                effectopt.SetAutoLayout(TRUE);
                effectopt.SetSizer(effitem0);
                effitem0->Fit(&effectopt);
	
	effectopt.CentreOnParent();
        if (effectopt.ShowModal() == wxID_OK)
                {
                effradioboxdef=effitem1->GetSelection();
                switch (effradioboxdef)
                        {
                        case 0: {
                                eff_attr=1;
                                eff_pix=1;
                                break;
                                }
                        case 1: {
                                eff_attr=0;
                                eff_pix=1;
                                break;
                                }
                        case 2: {
                                eff_attr=1;
                                eff_pix=0;
                                break;
                                }
                        }
                eff_arr_shift=effitem2->GetValue();
                eff_arr_char=effitem3->GetValue();
                }
        return;
        }

// Save current state to enable undo for the next action
void TheFrame::SaveUndo(bool cl)
{
        if (cl&&(Redo_total>0))
                {
                delete GraphRedo;
                GraphRedo=NULL;
                Redo_total=0;
                }
        if (Undo_total==4)
                {
                delete GraphUndo->NextGraph->NextGraph->NextGraph;
                GraphUndo->NextGraph->NextGraph->NextGraph=NULL;
                Undo_total--;
                }
        if (Undo_total==0)
                {
                GraphUndo = new SP_Graph (size_i,size_j);
                if (Current->OpenPropierties==2)
                        {
                        GraphUndo->Mask = new SP_Graph (size_i,size_j);
                        }
                }
        else
                {
                Aux=new SP_Graph (size_i,size_j);
                if (Current->OpenPropierties==2)
                        {
                        Aux->Mask = new SP_Graph (size_i,size_j);
                        }
                Aux->NextGraph=GraphUndo;
                GraphUndo=Aux;
                }

        for (int i=0;i<size_i;i++)
                {
                for (int j=0;j<size_j;j++)
                        {
                        if (Graphic2->GetPixel(i,j)==1)
                                {
                                GraphUndo->SetPixel(i,j);
                                }
                        else
                                {
                                GraphUndo->ResetPixel(i,j);
                                }
                        }
                }
        for (int i=0;i<sizechar_i;i++)
                {
                for (int j=0;j<sizechar_j;j++)
                        {
                        int k=Graphic2->GetAttr(i+j*sizechar_i);
                        GraphUndo->SetAttr(i+j*sizechar_i,k);
                        }
                }
        if (Current->OpenPropierties==2)
                {
                for (int i=0;i<size_i;i++)
                        {
                        for (int j=0;j<size_j;j++)
                                {
                                if (Graphic2->Mask->GetPixel(i,j)==1)
                                        {
                                        GraphUndo->Mask->SetPixel(i,j);
                                        }
                                else
                                        {
                                        GraphUndo->Mask->ResetPixel(i,j);
                                        }
                                }
                        }
                }
        Undo_total++;
}

void TheFrame::SaveRedo()
{
        if (Redo_total==0)
                {
                GraphRedo = new SP_Graph (size_i,size_j);
                if (Current->OpenPropierties==2)
                        {
                        GraphRedo->Mask = new SP_Graph (size_i,size_j);
                        }
                }
        else
                {
                Aux=new SP_Graph (size_i,size_j);
                if (Current->OpenPropierties==2)
                        {
                        Aux->Mask = new SP_Graph (size_i,size_j);
                        }
                Aux->NextGraph=GraphRedo;
                GraphRedo=Aux;
                }

        for (int i=0;i<size_i;i++)
                {
                for (int j=0;j<size_j;j++)
                        {
                        if (Graphic2->GetPixel(i,j)==1)
                                {
                                GraphRedo->SetPixel(i,j);
                                }
                        else
                                {
                                GraphRedo->ResetPixel(i,j);
                                }
                        }
                }
        for (int i=0;i<sizechar_i;i++)
                {
                for (int j=0;j<sizechar_j;j++)
                        {
                        int k=Graphic2->GetAttr(i+j*sizechar_i);
                        GraphRedo->SetAttr(i+j*sizechar_i,k);
                        }
                }
        if (Current->OpenPropierties==2)
                {
                for (int i=0;i<size_i;i++)
                        {
                        for (int j=0;j<size_j;j++)
                                {
                                if (Graphic2->Mask->GetPixel(i,j)==1)
                                        {
                                        GraphRedo->Mask->SetPixel(i,j);
                                        }
                                else
                                        {
                                        GraphRedo->Mask->ResetPixel(i,j);
                                        }
                                }
                        }
                }
        Redo_total++;
}

void TheFrame::ClearUndo()
{
        if (Redo_total>0)
                {
                delete GraphRedo;
                GraphRedo=NULL;
                Redo_total=0;
                }
        if (Undo_total>0)
                {
                delete GraphUndo;
                GraphUndo=NULL;
                Undo_total=0;
                }
}

// Draw a line between two points, applying the appropiate pixel action
//  on each pixel
void TheFrame::Bresenham (int action, int x, int y)
{
        wxInt32 d,ii,jj,di,ai,si,dj,aj,sj;
        di=x-lastx;
        ai=abs(di) <<1;
        si=(di<0)?-1:1;
        dj=y-lasty;
        aj=abs(dj)<<1;
        sj=(dj<0)?-1:1;
        ii=lastx;
        jj=lasty;
        if (ai>aj)
                {
                d=aj-(ai>>1);

                while (ii!=x)
                        {
                        if (d>=0)
                                {
                                jj+=sj;
                                d-=ai;
                                }
                        ii+=si;
                        d+=aj;
                        PixelAction(action,ii,jj);
                        }
                }
        else
                {
                d=ai-(aj>>1);
                while(jj!=y)
                        {
                        if (d>=0)
                                {
                                ii+=si;
                                d-=aj;
                                }
                        jj+=sj;
                        d+=ai;
                        PixelAction(action,ii,jj);
                        }
                }
}

void TheFrame::PushQueue (int x, int y)
{
        tempFill->SetPixel(x,y);
        if (x<FillMinX) FillMinX=x;
        if (y<FillMinY) FillMinY=y;
        if (x>FillMaxX) FillMaxX=x;
        if (y>FillMaxY) FillMaxY=y;
        int value = y*256+x;
        FillQueue[FillQueueHead++]=value;
        if (FillQueueHead==768) FillQueueHead=0;
}

bool TheFrame::PopQueue (int& x, int& y)
{
        if (FillQueueTail==FillQueueHead) return (FALSE);
        int value=FillQueue[FillQueueTail++];
        if (FillQueueTail==768) FillQueueTail=0;
        x= value % 256;
        y= value / 256;
        return (TRUE);
}

// Make an action on a certain pixel of a char
void TheFrame::PixelAction (int action, int x, int y)
{
        int xc=x/8;
        int yc=y/8;
        if (Files_open==0) return;
        wxClientDC dc(canvas2);
        SP_Graph *Graphic4;
        if ((Mask_use==1)&&(Mask_view==1))
                {
                Graphic4=Graphic2->Mask;
                }
        else
                {
                Graphic4=Graphic2;
                }
        int oldcol=Graphic2->GetAttr(xc+yc*sizechar_i);
        switch (action)
                {
                case 1: {       // Set
                        Graphic4->SetPixel(x,y);
                        Current->flagsure=1;
                        break;
                        }
                case 2: {       // Reset
                        Graphic4->ResetPixel(x,y);
                        Current->flagsure=1;
                        break;
                        }
                case 3: {       // Toggle
                        Graphic4->TogglePixel(x,y);
                        Current->flagsure=1;
                        break;
                        }
                case 4: {       // Select mode > no pixel action, thus return
                        switch (sselect)
                                {
                                case 0: {
                                        sel_start_x=x;
                                        sel_start_y=y;
                                        sselect++;
                                        SetStatusText(_("Toggle Pixel/Close selection"),0);
                                        break;
                                        }
                                case 1:
                                        {
                                        sel_end_x=x;
                                        sel_end_y=y;
                                        if (sel_end_x<sel_start_x)
                                                {
                                                int swap=sel_start_x;
                                                sel_start_x=sel_end_x;
                                                sel_end_x=swap;
                                                }
                                        if (sel_end_y<sel_start_y)
                                                {
                                                int swap=sel_start_y;
                                                sel_start_y=sel_end_y;
                                                sel_end_y=swap;
                                                }
                                        sel_end_x++;
                                        sel_end_y++;
                                        ssxc=sel_start_x;
                                        ssyc=sel_start_y;
                                        sexc=sel_end_x;
                                        seyc=sel_end_y;
                                        sselect++;
                                        SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                        DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                                        AreaRfrsh(dc,TRUE);
                                        break;
                                        }
                                case 2:
                                        {
                                        sselect=0;
                                        SetStatusText(_("Toggle Pixel/Select zone"),0);
                                        ssxc=sel_start_x;
                                        ssyc=sel_start_y;
                                        sexc=sel_end_x;
                                        seyc=sel_end_y;
                                        DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                                        AreaRfrsh(dc,TRUE);
                                        sel_start_x=sel_end_x=0;
                                        sel_start_y=sel_end_y=0;
                                        break;
                                        }
                                }
                        return;
                        }
                case 5: {
                        if ((Mask_use==1)&&(Mask_view==1))
                                {  // Toggle mask
                                Graphic4->TogglePixel(x,y);
                                Current->flagsure=1;
                                }
                        else    // Set colour > just set attributes
                                {
                                Current->flagsure=1;
                                }
                        break;
                        }
                case 6: {
                        if ((Mask_use==1)&&(Mask_view==1))
                                {  // Toggle graphic
                                Graphic2->TogglePixel(x,y);
                                Current->flagsure=1;
                                }
                        else    // Get colour > no pixel action, thus return
                                {
                                SetSelectPaper(Graphic2->GetPaper(xc,yc));
                                SetSelectInk(Graphic2->GetInk(xc,yc));
                                SetSelectBright(Graphic2->GetBright(xc,yc));
                                SetSelectFlash(Graphic2->GetFlash(xc,yc));
                                return;
                                }
                        break;
                        }
                case 7:         // Fill
                case 8: {       // Fill with pattern
                        if (Graphic4->GetPixel(x,y)!=0) return;
                        Current->flagsure=1;
                        tempFill = new SP_Graph(Graphic2->GetSizeX(),Graphic2->GetSizeY());
                        int fillx,filly;
                        FillMaxX=FillMaxY=0;
                        FillMinX=Graphic4->GetSizeX();
                        FillMinY=Graphic4->GetSizeY();
                        PushQueue(x,y);
                        while(PopQueue(fillx,filly))
                        {
                        if ((filly>0)&&(Graphic4->GetPixel(fillx,filly-1)==0)&&(tempFill->GetPixel(fillx,filly-1)==0)) PushQueue(fillx,filly-1);
                        if ((filly<(Graphic2->GetSizeY()-1))&&(Graphic4->GetPixel(fillx,filly+1)==0)&&(tempFill->GetPixel(fillx,filly+1)==0)) PushQueue(fillx,filly+1);
                        if ((fillx>0)&&(Graphic4->GetPixel(fillx-1,filly)==0)&&(tempFill->GetPixel(fillx-1,filly)==0)) PushQueue(fillx-1,filly);
                        if ((fillx<(Graphic2->GetSizeX()-1))&&(Graphic4->GetPixel(fillx+1,filly)==0)&&(tempFill->GetPixel(fillx+1,filly)==0)) PushQueue(fillx+1,filly);
                        }
                        if (action==8)
                                {       // Apply pattern
                                int FP1=copypastable->Propied1;
                                int FP2=copypastable->Propied2;
                                int SIZEX=copypastable->GetSizeX()-FP1;
                                int SIZEY=copypastable->GetSizeY()-FP2;
                                for (int i=FillMinX;i<FillMaxX+1;i++)
                                        for (int j=FillMinY;j<FillMaxY+1;j++)
                                                {
                                                if (copypastable->GetPixel(((i-x+SIZEX*256)%SIZEX)+FP1,((j-y+SIZEY*192)%SIZEY)+FP2)==0) tempFill->ResetPixel(i,j);
                                                }
                                }
                        for (int i=FillMinX;i<FillMaxX+1;i++)
                                for (int j=FillMinY;j<FillMaxY+1;j++)
                                        {
                                        if (tempFill->GetPixel(i,j)==1) Graphic4->SetPixel(i,j);
                                        }
                        DrawArea(FillMinX/8,FillMinY/8,(FillMaxX+7)/8,(FillMaxY+7)/8);
                        Rfrsh(dc,TRUE); // Temporal solution...
                        delete tempFill;
                        return;
                        break;
                        }
                case 9: {       // Paste
                        Current->flagsure=1;
                        int psizx,psizy,poffx,poffy;
                        int pastesize_x,pastesize_y;
                        int chrsizx_cp;

                        chrsizx_cp=copypastable->x_charsize;

                        int pfixx=0;
                        int pfixy=0;
                        int pcharfixx=0;
                        int pcharfixy=0;

                        psizx=copypastable->GetSizeX();
                        psizy=copypastable->GetSizeY();
                        poffx=copypastable->Propied1;
                        poffy=copypastable->Propied2;

                        pastesize_x=psizx-poffx;
                        pastesize_y=psizy-poffy;

                        if (x+pastesize_x>size_i) pfixx=x+pastesize_x-size_i;
                        if (y+pastesize_y>size_j) pfixy=y+pastesize_y-size_j;

                        for (int i=poffx;i<psizx-pfixx;i++)
                                {
                                for (int j=poffy;j<psizy-pfixy;j++)
                                        {
                                        if(copypastable->GetPixel(i,j)) Graphic4->SetPixel(x+i-poffx,y+j-poffy);
                                        else                            Graphic4->ResetPixel(x+i-poffx,y+j-poffy);
                                        }
                                }
                        int pchsx=copypastable->x_charsize;
                        int pchsy=copypastable->y_charsize;
                        if ((xc+pchsx)>sizechar_i) pcharfixx=xc+pchsx-sizechar_i;
                        if ((yc+pchsy)>sizechar_j) pcharfixy=yc+pchsy-sizechar_j;
                        if ((poffx==x%8)&&(poffy==y%8)&&(Mask_view==0)&&(copypastemask==0))
                                {
                                for (int i=0;i<(pchsx-pcharfixx);i++)
                                        {
                                        for (int j=0;j<(pchsy-pcharfixy);j++)
                                                {
                                                Graphic2->SetAttr((j+yc)*sizechar_i+i+xc,copypastable->GetAttr(j*chrsizx_cp+i));
                                                }
                                        }
                                }
                        DrawArea(xc,yc,xc+pchsx-pcharfixx,yc+pchsy-pcharfixy);
			Rfrsh(dc,TRUE); // Temporal solution...
                        return;
                        break;
                        }
                case 10:{       // Cancel paste or fill
                        switch (oldcursormode)
                                {
                                case 1: {
                                        cursormode=1;
                                        left_func=3;
                                        right_func=4;
                                        switch (sselect)
                                                {
                                                case 0: {
                                                        SetStatusText(_("Toggle Pixel/Select zone"),0);
                                                        break;
                                                        }
                                                case 1: {
                                                        SetStatusText(_("Toggle Pixel/Close selection"),0);
                                                        break;
                                                        }
                                                case 2: {
                                                        SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                                        break;
                                                        }
                                                }
                                        break;
                                        }
                                case 2: {
                                        cursormode=2;
                                        left_func=5;
                                        right_func=6;
                                        if ((Mask_use==1)&&(Mask_view==1))
                                                {
                                                SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                                }
                                        else
                                                {
                                                SetStatusText(_("Set Attributes/Get Attributes"),0);
                                                }
                                        break;
                                        }
                                case 3:
                                case 4:
                                case 5:
                                case 0: {
                                        cursormode=0;
                                        left_func=1;
                                        right_func=2;
                                        SetStatusText(_("Set Pixel/Reset Pixel"),0);
                                        break;
                                        }
                                }
                        return;
                        break;
                        }
                }
        if (Mask_view==0)       // Apply current colours to the character
                {               //  unless they're transparent, of course
                if (col_ink<8)          Graphic2->SetInk(xc,yc,col_ink);
                if (col_paper<8)        Graphic2->SetPaper(xc,yc,col_paper);
                if (col_bright<2)       Graphic2->SetBright(xc,yc,col_bright);
                if (col_flash<2)        Graphic2->SetFlash(xc,yc,col_flash);
                }
        if (Graphic2->GetAttr(xc+yc*sizechar_i)!=oldcol) // Attribute changed?
                {
                DrawArea(xc,yc,xc+1,yc+1);      // YES: repaint the char
                MiniRfrsh(dc,xc,yc,TRUE);
                }
        else
                {
                DrawCell(x,y);                  // NO: repaint the pixel
                MicroRfrsh(dc,x,y,TRUE);
                }
}

void TheFrame::OnActivate(wxActivateEvent &event)
{
        scrollsafe=1;
        event.Skip();
}


// Ask the "Are you sure?" question before closing the application, if there
//  are modified & unsaved open files
void TheFrame::OnClose(wxCloseEvent &event)
{
        wxMessageDialog dialog( this,_("All unsaved graphics will be lost\nAre you sure?"),_("Warning"),wxOK|wxCANCEL);
        dialog.CentreOnParent();
        if (Files_open==0) event.Skip();
        int j=0;
        for (int i=0;i<Files_open;i++)
                {
                j+=Files[i]->flagsure;
                }
        if ((j==0)||(Warn_exitprogram==0)) event.Skip();
        else
		if (dialog.ShowModal() == wxID_OK)
                {
                event.Skip();
                };
}

// Reset mouse state
void TheFrame::OnMouseEdit(wxMouseEvent& event)
{
        canvas2->m_status=0;
}

// Set window title with the name of current open graphic and frame number
void TheFrame::SetTit()
{
        wxString TheTitle;
        wxString Number0;
        wxString Number1;

        char num0=48+(Frame_current%10);
        char num1=48+((Frame_current/10)%10);
        char num2=48+(Frame_current/100);
        Number0=_("[");
        if (num2>48) Number0+=num2;
        if ((num1>48)||(num2>48)) Number0+=num1;
        Number0+=num0;

        char num3=48+(Frame_total%10);
        char num4=48+((Frame_total/10)%10);
        char num5=48+(Frame_total/100);
        Number1=_("/");
        if (num5>48) Number1+=num5;
        if ((num4>48)||(num5>48)) Number1+=num4;
        Number1+=num3;

        TheTitle=Current->OpenName.Left(Current->OpenName.Length()-1)+_(" - ")+Number0+Number1+_("]\0");

        if (Frame_total>1)      this->SetTitle(TheTitle);
        else                    this->SetTitle(Current->OpenName);
}

void TheFrame::OpenArrayFiles(const wxArrayString &filenames)
{
        for (unsigned int n=0;n<(filenames.GetCount());n++)
                {
                if (Files_open==12)
                        {
                        (void)wxMessageBox(_("Can't open more than 12 graphics"),_("SevenuP"));
                        break;
                        }
                Files[Files_open] = new class OpenFile(wc2s(filenames[n]));
                if (Files[Files_open]->OpenGraph->Propied1!=0)
                        {
                        Files_current=Files_open++;
                        if (sselect==2)
                                {
                                sselect=0;
                                DrawArea(sel_start_x/8,sel_start_y/8,(sel_end_x+7)/8,(sel_end_y+7)/8);
                                }
                        Current = Files[Files_current];
                        Graphic2 = Files[Files_current]->OpenGraph;
                        Frame_total=Current->OpenFrames;
                        wxChar num0=48+(Files_open%10);
                        wxString menunumber,menuname;
                        menunumber=num0;

                        // Conditional compilation code
#if defined(__WXGTK__) || defined (__WXMOTIF__) || defined (__WXMAC__)
                        if (Files_open>10)
                                {
                                menuname=_("1")+menunumber+_(" - ")+Current->OpenName;
                                }
                        else
                                {
                                if (Files_open==10)	menuname=_("1")+menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
				else			menuname=menunumber+_(" - ")+Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-")+menunumber+_("\0");
                                }
#else
                        if (Files_open>9)
                                {
                                menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F1")+menunumber+_("\0");
                                }
                        else
                                {
                                menuname=Current->OpenName.Left(Current->OpenName.Length()-1)+_("\tCtrl-F")+menunumber+_("\0");
                                }
#endif
                        menuFile->Append(FILE_1+Files_open-1, menuname);
                        size_i = Graphic2->GetSizeX();
                        size_j = Graphic2->GetSizeY();
                        sizechar_i= Graphic2->x_charsize;
                        sizechar_j= Graphic2->y_charsize;
                        if (AutoZoom)
                                {
                                int clientx,clienty,tryzoom;
                                canvas->GetClientSize(&clientx,&clienty);
                                menuBar->Check(ZOOM_1+zoom,FALSE);
                                if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,TRUE);
                                if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,TRUE);
                                for (tryzoom=0;tryzoom<ZOOMLEVELS;tryzoom++)
                                        {
                                        if ((size_i*ZoomValue[tryzoom]<clientx)&&(size_j*ZoomValue[tryzoom]<clienty))
                                                {
                                                zoom=tryzoom;
                                                }
                                        }
                                menuBar->Check(ZOOM_1+zoom,TRUE);
                                if (zoom==ZOOMLEVELS-1) toolBar->EnableTool(ZOOM_ZOOMIN,FALSE);
                                if (zoom==0) toolBar->EnableTool(ZOOM_ZOOMOUT,FALSE);
                                }
                        size_x = size_i*ZoomValue[zoom];
                        size_y = size_j*ZoomValue[zoom];
                        DrawArea(0,0,sizechar_i,sizechar_j);
                        }
                else
                        {
                        delete Files[Files_open];
                        }
                }
        if (Files_open==0) return;
        Mask_view=0;
        menuMask->Check(MASK_VIEW,FALSE);
        Mask_use=Current->OpenPropierties-1;
        menuMask->Check(MASK_USE,(Mask_use));
        Frame_current=1;
        sselect=0;
        sel_start_x=sel_end_x=0;
        sel_start_y=sel_end_y=0;
        SetTit();
        ClearUndo();
        disablezoom=1;
        canvas->Scroll(0,0);
        canvas->SetScrollbars(1,1,size_x,size_y);
        canvas2->SetSize(size_x,size_y);
        disablezoom=0;
        wxClientDC dc(canvas2);
        Rfrsh(dc,TRUE);
}

int TheFrame::GetIniValue(wxString IniFile,wxString VarName,int down,int up,int defaul)
        {
        if (ValidIni==0)
                {
                return defaul;
                }
        int varstart;
	char valchar;
        int value=0;
        if ((varstart=IniFile.Find(VarName))!=-1)
                {
                unsigned int vstart;
                vstart=varstart;
                vstart+=VarName.Len();
                while((valchar=IniFile[vstart++])!=';')
                        {
                        value=value*10+(valchar-48);
                        }
                }
        else return defaul;
        if ((value<down)||(value>up))
                {
                value=defaul;
                }
        return value;
        }

wxString TheFrame::GetIniValueS(wxString IniFile,wxString VarName,wxString defaul)
        {
        wxString value;
        if (ValidIni==0)
                {
                return defaul;
                }
        int varstart;
        if ((varstart=IniFile.Find(VarName))!=-1)
                {
                value=IniFile.Mid(varstart+VarName.Len()).BeforeFirst(';');
                return value;
                }
        else return defaul;
        }


// MORE FUNCTIONS END


// Event table -> Relates events with functions
BEGIN_EVENT_TABLE(TheFrame, wxFrame)
        EVT_MENU                (FILE_NEW,TheFrame::FileNew)
        EVT_MENU                (FILE_LOAD,TheFrame::FileLoad)
        EVT_MENU                (FILE_IMPORT,TheFrame::FileImport)
        EVT_MENU                (FILE_BINIMPORT,TheFrame::FileBinImport)
        EVT_MENU                (FILE_CLOSE,TheFrame::FileClose)
        EVT_MENU                (FILE_RENAME,TheFrame::FileRename)
        EVT_MENU                (FILE_FASTSAVE,TheFrame::FileFastSave)
        EVT_MENU                (FILE_SAVE,TheFrame::FileSave)
        EVT_MENU                (FILE_EXPORTDATA,TheFrame::FileExportData)
        EVT_MENU                (FILE_EXPORTIMAGE,TheFrame::FileExportImage)
        EVT_MENU                (FILE_SAVEOPT,TheFrame::FileSaveOPT)
        EVT_MENU                (FILE_ASMSELECT_0,TheFrame::FileSaveSelAsm0)
        EVT_MENU                (FILE_ASMSELECT_1,TheFrame::FileSaveSelAsm1)
        EVT_MENU                (FILE_ASMSELECT_2,TheFrame::FileSaveSelAsm2)
        EVT_MENU                (FILE_ASMSELECT_3,TheFrame::FileSaveSelAsm3)
        EVT_MENU                (FILE_ASMSELECT_4,TheFrame::FileSaveSelAsm4)
        EVT_MENU                (FILE_ASMSELECT_5,TheFrame::FileSaveSelAsm5)
        EVT_MENU                (FILE_ASMSELECT_6,TheFrame::FileSaveSelAsm6)
        EVT_MENU                (FILE_ASMSELECT_7,TheFrame::FileSaveSelAsm7)
        EVT_MENU                (FILE_QUIT,TheFrame::FileQuit)
        EVT_MENU                (FILE_1,TheFrame::FileSelect)
        EVT_MENU                (FILE_2,TheFrame::FileSelect)
        EVT_MENU                (FILE_3,TheFrame::FileSelect)
        EVT_MENU                (FILE_4,TheFrame::FileSelect)
        EVT_MENU                (FILE_5,TheFrame::FileSelect)
        EVT_MENU                (FILE_6,TheFrame::FileSelect)
        EVT_MENU                (FILE_7,TheFrame::FileSelect)
        EVT_MENU                (FILE_8,TheFrame::FileSelect)
        EVT_MENU                (FILE_9,TheFrame::FileSelect)
        EVT_MENU                (FILE_10,TheFrame::FileSelect)
        EVT_MENU                (FILE_11,TheFrame::FileSelect)
        EVT_MENU                (FILE_12,TheFrame::FileSelect)
        EVT_MENU                (EDIT_UNDO, TheFrame::EditUndo)
        EVT_MENU                (EDIT_REDO, TheFrame::EditRedo)
        EVT_MENU                (EDIT_COPY, TheFrame::EditCopy)
        EVT_MENU                (EDIT_PASTE, TheFrame::EditPaste)
        EVT_MENU                (FILL_SOLID, TheFrame::FillSolid)
        EVT_MENU                (FILL_PATTERN, TheFrame::FillPattern)
        EVT_MENU                (EFF_INV,TheFrame::EffInvert)
        EVT_MENU                (EFF_FLIP_X,TheFrame::EffFlipX)
        EVT_MENU                (EFF_FLIP_Y,TheFrame::EffFlipY)
        EVT_MENU                (EFF_ROT_LEFT,TheFrame::EffRot_left)
        EVT_MENU                (EFF_ROT_RIGHT,TheFrame::EffRot_right)
        EVT_MENU                (EFF_ROT_UP,TheFrame::EffRot_up)
        EVT_MENU                (EFF_ROT_DOWN,TheFrame::EffRot_down)
        EVT_MENU                (EFF_ROT_CLOCK,TheFrame::EffRot_clock)
        EVT_MENU                (EFF_ROT_ANTICLOCK,TheFrame::EffRot_anticlock)
        EVT_MENU                (EFF_OPT,TheFrame::EffOpt)
        EVT_MENU                (MASK_USE,TheFrame::MaskUse)
        EVT_MENU                (MASK_VIEW,TheFrame::MaskView)
        EVT_MENU                (MASK_AUTO,TheFrame::MaskAuto)
        EVT_MENU                (SPR_INS_NEXT,TheFrame::SpriteInsertNext)
        EVT_MENU                (SPR_INS_PREV,TheFrame::SpriteInsertPrev)
        EVT_MENU                (SPR_MOVE,TheFrame::SpriteMove)
        EVT_MENU                (SPR_REMOVE,TheFrame::SpriteRemove)
        EVT_MENU                (SPR_SEL_PREV,TheFrame::SpritePrev)
        EVT_MENU                (SPR_SEL_NEXT,TheFrame::SpriteNext)
        EVT_MENU                (SPR_SEL_GOTO,TheFrame::SpriteGoto)
        EVT_MENU                (GRID_PIXEL,TheFrame::GridPixel)
        EVT_MENU                (GRID_CHAR,TheFrame::GridChar)
        EVT_MENU                (ZOOM_AUTO,TheFrame::ZoomAuto)
        EVT_MENU                (ZOOM_1,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_2,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_3,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_4,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_5,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_6,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_7,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_8,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_9,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_10,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_11,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_12,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_13,TheFrame::SetZoom)
        EVT_MENU                (ZOOM_ZOOMIN,TheFrame::ZoomIn)
        EVT_MENU                (ZOOM_ZOOMOUT,TheFrame::ZoomOut)
        EVT_MENU                (INFO_ABOUT,TheFrame::InfoAbout)
        EVT_MENU                (TOOLBAR_CHANGE,TheFrame::ChangeCursor)
        EVT_MENU                (TOOLBAR_ERASE,TheFrame::Erase)
        EVT_ACTIVATE            (TheFrame::OnActivate)
        EVT_CLOSE               (TheFrame::OnClose)
        EVT_MOUSE_EVENTS        (TheFrame::OnMouseEdit)
        EVT_BUTTON              (ID_SAVEOPT_BUTTON_UP,TheFrame::MoveOptUP)
        EVT_BUTTON              (ID_SAVEOPT_BUTTON_DOWN,TheFrame::MoveOptDOWN)
        EVT_BUTTON              (ID_BITMAPBUTTONI0,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI1,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI2,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI3,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI4,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI5,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI6,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONI7,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONIT,TheFrame::ButtonINK)
        EVT_BUTTON              (ID_BITMAPBUTTONP0,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP1,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP2,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP3,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP4,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP5,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP6,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONP7,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONPT,TheFrame::ButtonPAPER)
        EVT_BUTTON              (ID_BITMAPBUTTONB0,TheFrame::ButtonBRIGHT)
        EVT_BUTTON              (ID_BITMAPBUTTONB1,TheFrame::ButtonBRIGHT)
        EVT_BUTTON              (ID_BITMAPBUTTONBT,TheFrame::ButtonBRIGHT)
        EVT_BUTTON              (ID_BITMAPBUTTONF0,TheFrame::ButtonFLASH)
        EVT_BUTTON              (ID_BITMAPBUTTONF1,TheFrame::ButtonFLASH)
        EVT_BUTTON              (ID_BITMAPBUTTONFT,TheFrame::ButtonFLASH)
END_EVENT_TABLE()

//--------------------------------------------------------------------------
//  DnDFile
//--------------------------------------------------------------------------

bool DnDFile::OnDropFiles(wxCoord,wxCoord,const wxArrayString& filenames)
{
        m_pOwner->OpenArrayFiles(filenames);
        return TRUE;
}


//--------------------------------------------------------------------------
//  MyWindow
//--------------------------------------------------------------------------


// Define a constructor for my canvas
MyWindow::MyWindow(wxScrolledWindow *frame, int x, int y, int w, int h, long style):
 wxWindow(frame, -1, wxPoint(x, y), wxSize(w, h), style)
{
}

MyWindow::~MyWindow()
{
        m_status=0;
}

// Repaint unless disablezoom==1, regenerating ONLY if necessary
void MyWindow::OnPaint(wxPaintEvent& WXUNUSED(event) )
{
        if (disablezoom==1) return;
        wxPaintDC dc(this);
        framee->canvas->GetViewStart(&startx,&starty);
        framee->canvas->GetClientSize(&sizx,&sizy);
        if (framee->Files_open==0)
                {
                framee->Rfrsh(dc,FALSE);
                return;
                }
        int zz=ZoomValue[framee->zoom];

        if (framee->zoom<ZOOMBITMAPS)
                {
                framee->Rfrsh(dc,FALSE);
                return;
                }
        if (((startx!=oldstartx)||(starty!=oldstarty))&&((sizx!=oldsizx)||(sizy!=oldsizy)))
                {
                framee->Rfrsh(dc,TRUE);
                return;
                }
#if defined (__WXMSW__)
        if (startx>oldstartx)
                {       // Scroll right
                if (scrollsafe==1)
                        {
                        scrollsafe=0;
                        framee->Rfrsh(dc,TRUE);
                        return;
                        }
                framee->RfScroll(dc);
                ssxc=(oldstartx+sizx)/zz;
                sexc=(startx+sizx)/zz;
                ssyc=starty/zz;
                seyc=(starty+sizy)/zz;
                sexc++;
                seyc++;
                if (sexc>framee->size_i) sexc=framee->size_i;
                if (seyc>framee->size_j) seyc=framee->size_j;
                framee->AreaRfrsh(dc,FALSE);
                framee->Rfrsh(dc,FALSE);
                return;
                }
        if (startx<oldstartx)
                {       // Scroll left
                if (scrollsafe==1)
                        {
                        framee->Rfrsh(dc,TRUE);
                        scrollsafe=0;
                        return;
                        }
                framee->RfScroll(dc);
                ssxc=startx/zz;
                sexc=oldstartx/zz;
                ssyc=starty/zz;
                seyc=(starty+sizy)/zz;
                sexc++;
                seyc++;
                framee->AreaRfrsh(dc,FALSE);
                framee->Rfrsh(dc,FALSE);
                return;
                }
        if (starty>oldstarty)
                {       // Scroll down
                if (scrollsafe==1)
                        {
                        framee->Rfrsh(dc,TRUE);
                        scrollsafe=0;
                        return;
                        }
                framee->RfScroll(dc);
                ssyc=(oldstarty+sizy)/zz;
                seyc=(starty+sizy)/zz;
                ssxc=startx/zz;
                sexc=(startx+sizx)/zz;
                sexc++;
                seyc++;
                if (sexc>framee->size_i) sexc=framee->size_i;
                if (seyc>framee->size_j) seyc=framee->size_j;
                framee->AreaRfrsh(dc,FALSE);
                framee->Rfrsh(dc,FALSE);
                return;
                }
        if (starty<oldstarty)
                {       // Scroll up
                if (scrollsafe==1)
                        {
                        framee->Rfrsh(dc,TRUE);
                        scrollsafe=0;
                        return;
                        }
                framee->RfScroll(dc);
                ssyc=starty/zz;
                seyc=oldstarty/zz;
                ssxc=startx/zz;
                sexc=(startx+sizx)/zz;
                sexc++;
                seyc++;
                framee->AreaRfrsh(dc,FALSE);
                framee->Rfrsh(dc,FALSE);
                return;
                }
        scrollsafe=0;
        if (sizx>oldsizx)
                {
                ssxc=(startx+oldsizx)/zz;
                ssyc=starty/zz;
                sexc=(startx+sizx)/zz;
                seyc=(starty+sizy)/zz;
                sexc++;
                seyc++;
                if (sexc>framee->size_i) sexc=framee->size_i;
                if (seyc>framee->size_j) seyc=framee->size_j;
                framee->AreaRfrsh(dc,FALSE);
                }
        if (sizy>oldsizy)
                {
                ssxc=startx/zz;
                ssyc=(starty+oldsizy)/zz;
                sexc=(startx+sizx)/zz;
                seyc=(starty+sizy)/zz;
                sexc++;
                seyc++;
                if (sexc>framee->size_i) sexc=framee->size_i;
                if (seyc>framee->size_j) seyc=framee->size_j;
                framee->AreaRfrsh(dc,FALSE);
                }
        if ((sizx>oldsizx)&&(sizy>oldsizy))
                {
                ssxc=(startx+oldsizx)/zz;
                ssyc=(starty+oldsizy)/zz;
                sexc=(startx+sizx)/zz;
                seyc=(starty+sizy)/zz;
                sexc++;
                seyc++;
                if (sexc>framee->size_i) sexc=framee->size_i;
                if (seyc>framee->size_j) seyc=framee->size_j;
                framee->AreaRfrsh(dc,FALSE);
                }
        framee->Rfrsh(dc,FALSE);
#else
        framee->Rfrsh(dc,TRUE);
#endif
}

void MyWindow::OnEraseBackground(wxEraseEvent& event)
{
        // do nothing
}

// Process mouse event
void MyWindow::OnMouse(wxMouseEvent& event)
{
        if (framee->Files_open==0) return;
        int x,y,z,xc,yc,col,colink,colpaper,colbright,colflash;
        wxString msg;
        z=framee->GetZoom();
        x=event.GetX()/ZoomValue[z];
        y=event.GetY()/ZoomValue[z];
        xc=x/8;
        yc=y/8;
        col=framee->Graphic2->GetAttr(xc+yc*framee->sizechar_i);
        colflash=col/128;
        colbright=(col-colflash*128)/64;
        colpaper=(col-colflash*128-colbright*64)/8;
        colink=col-colflash*128-colbright*64-colpaper*8;

        msg.Printf(_("P:(%d, %d) - C:(%d, %d) - Attr: %d (I:%d P:%d B:%d F:%d)"),x,y,xc,yc,col,colink,colpaper,colbright,colflash);
        framee->SetStatusText(msg,1);
        switch (framee->cursormode)
                {
                case 0: {
                        framee->SetStatusText(_("Set Pixel/Reset Pixel"),0);
                        break;
                        }
                case 1: {
                        switch (framee->sselect)
                                {
                                case 0: {
                                        framee->SetStatusText(_("Toggle Pixel/Select zone"),0);
                                        break;
                                        }
                                case 1: {
                                        framee->SetStatusText(_("Toggle Pixel/Close selection"),0);
                                        break;
                                        }
                                case 2: {
                                        framee->SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                        break;
                                        }
                                }
                        break;
                        }
                case 2: {
                        if ((framee->Mask_use==1)&&(framee->Mask_view==1))
                                {
                                framee->SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                }
                        else
                                {
                                framee->SetStatusText(_("Set Attributes/Get Attributes"),0);
                                }
                        break;
                        }
                case 3: {
                        framee->SetStatusText(_("Paste/Cancel paste"),0);
                        break;
                        }
                case 4: {
                        framee->SetStatusText(_("Fill/Cancel fill"),0);
                        break;
                        }
                case 5: {
                        framee->SetStatusText(_("Pattern Fill/Cancel fill"),0);
                        break;
                        }
                }

  // process left button
        if (event.LeftIsDown())
                {
                if ((event.LeftDown())||(event.LeftDClick()))
                        {
                        framee->SaveUndo();
                        framee->PixelAction(framee->left_func,x,y);
                        framee->lastx=x;
                        framee->lasty=y;
                        m_status=1;
                        }
                else if (((framee->lastx!=x)||(framee->lasty!=y))&&(framee->right_func!=10))
                        {
                        if (m_status==1)
                                {
                                framee->Bresenham(framee->left_func,x,y);
                                framee->lastx=x;
                                framee->lasty=y;
                                }
                        else
                                {
                                if (framee->lastx==-1)
                                        {
                                        framee->lastx=x;
                                        return;
                                        }
                                framee->PixelAction(framee->left_func,x,y);
                                framee->lastx=x;
                                framee->lasty=y;
                                m_status=1;
                                }
                        }
                }
        // process right button
        else if (event.RightIsDown())
                {
                if ((event.RightDown())||(event.RightDClick()))
                        {
                        if ((framee->right_func!=10)&&(framee->right_func!=4)&&((framee->right_func!=6)||(framee->Mask_view==1)))
                                {
                                framee->SaveUndo();
                                }
                        framee->PixelAction(framee->right_func,x,y);
                        framee->lastx=x;
                        framee->lasty=y;
                        m_status=2;
                        }
                else if (((framee->lastx!=x)||(framee->lasty!=y))&&(framee->right_func!=4)&&(framee->right_func!=10))
                        {
                        if (m_status==2)
                                {
                                framee->Bresenham(framee->right_func,x,y);
                                framee->lastx=x;
                                framee->lasty=y;
                                }
                        else
                                {
                                framee->PixelAction(framee->right_func,x,y);
                                framee->lastx=x;
                                framee->lasty=y;
                                m_status=2;
                                }
                        }
                }
        else    m_status=0;
        if (framee->lastx==-1)
                {
                framee->lastx=x;
                }
}

void MyWindow::OnKey(wxKeyEvent& event)
{
        int key;
        key=event.GetKeyCode();
        switch (key)
                {
                case '1': {
                        framee->cursormode=0;
                        framee->left_func=1;
                        framee->right_func=2;
                        framee->SetStatusText(_("Set Pixel/Reset Pixel"),0);
                        break;
                        }
                case '2': {
                        framee->cursormode=1;
                        framee->left_func=3;
                        framee->right_func=4;
                        switch (framee->sselect)
                                {
                                case 0: {
                                        framee->SetStatusText(_("Toggle Pixel/Select zone"),0);
                                        break;
                                        }
                                case 1: {
                                        framee->SetStatusText(_("Toggle Pixel/Close selection"),0);
                                        break;
                                        }
                                case 2: {
                                        framee->SetStatusText(_("Toggle Pixel/Discard selection"),0);
                                        break;
                                        }
                                };
                        break;
                        }
                case '3': {
                        framee->cursormode=2;
                        framee->left_func=5;
                        framee->right_func=6;
                        if ((framee->Mask_use==1)&&(framee->Mask_view==1))
                                {
                                framee->SetStatusText(_("Toggle mask/Toggle graphic"),0);
                                }
                        else
                                {
                                framee->SetStatusText(_("Set Attributes/Get Attributes"),0);
                                }
                        break;
                        }
		 case 43:
		case 392:{
			framee->DoZoomIn();
			break;
			}
		 case 45:
		case 394:{
			framee->DoZoomOut();
			break;
			}

                }
        event.Skip();
}


// An event table for MyWindow class
BEGIN_EVENT_TABLE(MyWindow, wxWindow)
        EVT_PAINT               (MyWindow::OnPaint)
        EVT_MOTION              (MyWindow::OnMouse)
        EVT_LEFT_DOWN           (MyWindow::OnMouse)
        EVT_RIGHT_DOWN          (MyWindow::OnMouse)
        EVT_LEFT_DCLICK         (MyWindow::OnMouse)
        EVT_RIGHT_DCLICK        (MyWindow::OnMouse)
        EVT_ERASE_BACKGROUND    (MyWindow::OnEraseBackground)
        EVT_KEY_DOWN            (MyWindow::OnKey)
END_EVENT_TABLE()
