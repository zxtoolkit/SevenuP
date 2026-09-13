// TheFrame.h
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// Main Frame - headers
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

#ifndef _THEFRAME_H
#define _THEFRAME_H


#include <wx/wxprec.h>

#ifndef WX_PRECOMP
  #include <wx/wx.h>
  #include <wx/bmpbuttn.h>
  #include <wx/choicdlg.h>
  #include <wx/datetime.h>
  #include <wx/dc.h>
  #include <wx/dcclient.h>
  #include <wx/dcscreen.h>
  #include <wx/dnd.h>
  #include <wx/filedlg.h>
  #include <wx/event.h>
  #include <wx/gdicmn.h>
  #include <wx/image.h>
  #include <wx/numdlg.h>  
  #include <wx/scrolwin.h>
  #include <wx/spinctrl.h>
  #include <wx/wfstream.h>
#endif

#include "SP_Graph.h"
#include "OpenFile.h"

class MyWindow;

class TheFrame : public wxFrame
{
        private:

        wxToolBar *toolBar;
        enum                    // Constants
        {
        FILE_NEW = 2001,
        FILE_LOAD,
        FILE_IMPORT,
        FILE_BINIMPORT,
        FILE_EXPORTDATA,
        FILE_EXPORTIMAGE,
        FILE_CLOSE,
        FILE_RENAME,
        FILE_FASTSAVE,
        FILE_SAVE,
        FILE_SAVEOPT,
        FILE_ASMSELECT,
        FILE_ASMSELECT_0,
        FILE_ASMSELECT_1,
        FILE_ASMSELECT_2,
        FILE_ASMSELECT_3,
        FILE_ASMSELECT_4,
        FILE_ASMSELECT_5,
        FILE_ASMSELECT_6,
        FILE_ASMSELECT_7,
        FILE_QUIT,
        FILE_1,
        FILE_2,
        FILE_3,
        FILE_4,
        FILE_5,
        FILE_6,
        FILE_7,
        FILE_8,
        FILE_9,
        FILE_10,
        FILE_11,
        FILE_12,
        EDIT_UNDO,
        EDIT_REDO,
        EDIT_COPY,
        EDIT_PASTE,
        FILL_SOLID,
        FILL_PATTERN,
        EFF_INV,
        EFF_FLIP_X,
        EFF_FLIP_Y,
        EFF_ROT_LEFT,
        EFF_ROT_RIGHT,
        EFF_ROT_UP,
        EFF_ROT_DOWN,
        EFF_ROT_CLOCK,
        EFF_ROT_ANTICLOCK,
        EFF_OPT,
        MASK_USE,
        MASK_VIEW,
        MASK_AUTO,
        SPR_INS_NEXT,
        SPR_INS_PREV,
        SPR_MOVE,
        SPR_REMOVE,
        SPR_SEL_PREV,
        SPR_SEL_NEXT,
        SPR_SEL_GOTO,
        GRID_PIXEL,
        GRID_CHAR,
        ZOOM_1,
        ZOOM_2,
        ZOOM_3,
        ZOOM_4,
        ZOOM_5,
        ZOOM_6,
        ZOOM_7,
        ZOOM_8,
        ZOOM_9,
        ZOOM_10,
        ZOOM_11,
        ZOOM_12,
        ZOOM_13,
        ZOOM_ZOOMIN,
        ZOOM_ZOOMOUT,
        ZOOM_AUTO,
        INFO_ABOUT,
        TOOLBAR_ERASE,
        TOOLBAR_CHANGE,
        ID_PANEL,
        ID_LINE,
        ID_TEXT,
        ID_NEW_RADIOBOX,
        ID_SAVEOPT_RADIOBOX,
        ID_SAVEOPT_LISTBOX,
        ID_SAVEOPT_BUTTON_UP,
        ID_SAVEOPT_BUTTON_DOWN,
        ID_SAVEOPT_MASK,
        ID_SAVEOPT_APPEND,
        ID_SAVEOPT_INTERLEAVE,
        ID_SAVEOPT_ZIGZAG,
        ID_SAVEOPT_ATMASK,
        ID_SAVEOPT_ATMASKI,
        ID_SAVEOPT_ATMASKP,
        ID_SAVEOPT_ATMASKB,
        ID_SAVEOPT_ATMASKF,
        ID_SAVEOPT_Z88DK,
        ID_SAVEOPT_NOLABEL,
        ID_RADIOBOX_EFF,
        ID_CHECKBOX_EFF1,
        ID_CHECKBOX_EFF2,
        ID_NEW_SPINCTRL1,
        ID_NEW_SPINCTRL2,
        ID_BITMAPBUTTON,
        ID_BITMAPBUTTONI0,
        ID_BITMAPBUTTONI1,
        ID_BITMAPBUTTONI2,
        ID_BITMAPBUTTONI3,
        ID_BITMAPBUTTONI4,
        ID_BITMAPBUTTONI5,
        ID_BITMAPBUTTONI6,
        ID_BITMAPBUTTONI7,
        ID_BITMAPBUTTONIT,
        ID_BITMAPBUTTONP0,
        ID_BITMAPBUTTONP1,
        ID_BITMAPBUTTONP2,
        ID_BITMAPBUTTONP3,
        ID_BITMAPBUTTONP4,
        ID_BITMAPBUTTONP5,
        ID_BITMAPBUTTONP6,
        ID_BITMAPBUTTONP7,
        ID_BITMAPBUTTONPT,
        ID_BITMAPBUTTONB0,
        ID_BITMAPBUTTONB1,
        ID_BITMAPBUTTONBT,
        ID_BITMAPBUTTONF0,
        ID_BITMAPBUTTONF1,
        ID_BITMAPBUTTONFT
        };

        int     size_wx, size_wy; // Size of current window
        int     col_ink, col_paper, col_bright, col_flash; // selected colours
        int     priorities[5];  // Sort priorities
        int	interleave;	// Interleave option
        int     maskfirst;      // Mask comes first flag
        int     AttrMask;      // There are attribute masks
	int 	AttrMaskInk;	// Ink Attr Mask is marked
	int 	AttrMaskPaper;	// Paper Attr Mask is marked
	int 	AttrMaskBright;	// Bright Attr Mask is marked
	int 	AttrMaskFlash;	// Flash Attr Mask is marked
	int	zigzag;		// Horizontal zigzag enabled
	int	z88dksize;	// Add size in two first bytes as needed by z88dk functions
	int	nolabel;	// Don't output label on ASM files
	
        int     appendfile;     // Append instead of overwriting old file
        int     optradioboxdef; // Current radiobox selected option in Output
                                //  Options menu (at the end, no attr or 9th)
        int     effradioboxdef; // Current radiobox selected option in Effect
                                //  Options menu (Both, pixels or attributes)
        int     asmtype;        // Selected assembler type
	int	defaultexport;  // Default extension when exporting data

        int     newfiles;       // New files counter

        int     newdefaultx, newdefaulty; // Default size for new graphics

        int     eff_attr;       // Do effects affect attributes?
        int     eff_pix;        // Do effects affect pixels?
        bool    eff_arr_shift;  // Shift instead of rotation?
        bool    eff_arr_char;   // Character rotation/shift?

	int	Warn_closefile;	// Warning when closing a modified file?
	int	Warn_exitprogram;// Warning when exiting the program with modified files?
	int	Warn_spriteremove; // Warning on sprite frame deletion?
	int	Save_showdialog; // Show dialog after using Save?

        bool    RememberOpenFilePath;    // Remember Open last path?
        bool    RememberImportImagePath; // Remember Import Image last path?
        bool    RememberSavePath;        // Remember Save last path?
        bool    RememberExportDataPath;  // Remember Export Data last path?
        bool    RememberExportImagePath; // Remember Export Image last path?

        wxString OpenFilePath;          // Open last path
        wxString ImportImagePath;       // Import Image last path
        wxString SavePath;              // Save last path
        wxString ExportDataPath;        // Data last path
        wxString ExportImagePath;       // Export Image path

	wxBitmapButton *Butt_ink[9];    // The ink colour buttons
        wxBitmapButton *Butt_paper[9];  // The paper colour buttons
        wxBitmapButton *Butt_bright[3]; // The bright colour buttons
        wxBitmapButton *Butt_flash[3];  // The flash colour buttons

        wxBoxSizer *mainbox;            // Elements of frame layout
        wxBoxSizer *boxink;
        wxStaticText *boxinktext;
        wxBoxSizer *boxpaper;
        wxStaticText *boxpapertext;
        wxStaticText *boxbriflatext1;
        wxStaticText *boxbriflatext2;

        wxMenuBar *menuBar;             // Menu elements
        wxMenu *menuFile;
        wxMenu *menuFile_SelAsm;
        wxMenu *menuEdit;
        wxMenu *menuFill;
        wxMenu *menuMask;
        wxMenu *menuSprite;
        wxMenu *menuGrid;
        wxMenu *menuEffects;
        wxMenu *menuZoom;
        wxMenu *menuInfo;

        wxBoxSizer *newitem0;           // Elements of new file dialog
        wxBoxSizer *newitem1;
        wxSpinCtrl *newitem5;
        wxSpinCtrl *newitem7;

        wxBoxSizer *optitem0;           // Elements of output options dialog
        wxBoxSizer *optitem1;
        wxRadioBox *optitem2;
        wxBoxSizer *optitem3;
        wxBoxSizer *optitem6;
        wxListBox  *optitem9;
        wxBoxSizer *optitem10;
        wxCheckBox *optitem13;
        wxCheckBox *optitem14;
        wxRadioBox *optitem15;
        wxCheckBox *optitem16;
        wxCheckBox *optitem17;
        wxCheckBox *optitem18;
        wxCheckBox *optitem19;
        wxCheckBox *optitem20;
        wxCheckBox *optitem21;
        wxCheckBox *optitem22;
        wxBoxSizer *optitem23;
        wxCheckBox *optitem24;

        wxBoxSizer *effitem0;           // Elements of effect options dialog
        wxRadioBox *effitem1;
        wxCheckBox *effitem2;
        wxCheckBox *effitem3;
        wxBoxSizer *effitem4;

protected:
        DECLARE_EVENT_TABLE()

public:
                                        // Frame Constructor
        TheFrame(int xpos, int ypos, int width, int height, int argc, wxChar **argv);
        ~TheFrame();

        wxScrolledWindow *canvas; // I want the canvas to be scrollable
        MyWindow *canvas2;      // The canvas for the graphic

        class OpenFile *Files[12];    // Array of OpenFiles
        class OpenFile *Current;      // Current file in use
        SP_Graph *Graphic2;     // Current graphic in use

        SP_Graph *GraphUndo;    // Next undo graphic
        SP_Graph *GraphRedo;    // Next redo graphic
        SP_Graph *Aux;          // Auxiliar graphic

        int     Files_open;     // Number of files opened so far
        int     Files_current;  // Number of file being shown and used right now

        int     Mask_view;      // Are we editing the mask (1) or graphic (0)
        int     Mask_use;       // Do we have a mask anyway?

        int     Frame_current;  // Number of current selected frame
        int     Frame_total;    // Total number of frames

        int     Undo_total;     // Number of undo levels used
        int     Redo_total;     // Number of undo levels used

        int     cursormode, oldcursormode;   // Cursor mode, current & backup
        int     left_func, right_func;  // Funcions associated with
                                        //  left & right buttons of the mouse
        int     lastx, lasty;       // Last position where the mouse was seen
        bool    gridpix,gridchr;        // Grids enabled or not
        int     sselect;                // Selection state:
                                        //  0 = no selection
                                        //  1 = selected first point, awaiting
                                        //       second to close selection
                                        //  2 = area selected, determinated
                                        //      by sel_start_x, sel_end_x,
                                        //      sel_start_y and sel_end_y

        int     sizechar_i, sizechar_j; // Current graphic size in chars

        int     sel_start_x;    // Starting x position of selection
        int     sel_start_y;    // Starting y position of selection
        int     sel_end_x;      // Ending x position of selection
        int     sel_end_y;      // Ending y position of selection
        int     zoom;           // Zoom level
        bool    AutoZoom;       // Rezoom behaviour
        int     size_i, size_j; // Size of current graphic
        int     size_x, size_y; // Size of current internal window



        wxDialog dialogopt;             // output options dialog
        wxDialog effectopt;             // effects options dialog

        wxBitmap bigescreen;            // Default bitmap for big zooms
        wxBitmap scrollscreen;          // Scroll bitmap for big zooms

        void FileNew (wxCommandEvent &event);   // Create a new graph
        void FileLoad (wxCommandEvent &event);  // Load a .SEV or .SCR file
        void FileImport (wxCommandEvent &event);// Import an image
        void FileBinImport (wxCommandEvent &event); // Import from binary
        void FileFastSave (wxCommandEvent &event);  // Save graph
        void FileSave (wxCommandEvent &event); // Save graph giving name

        void FileExportData (wxCommandEvent &event); // Save as binary or source
        void FileExportImage (wxCommandEvent &event);// Export to an image

        void FileClose (wxCommandEvent &event); // Close graph
        void FileRename (wxCommandEvent &event); // Rename graph
        void FileSaveOPT (wxCommandEvent &event); // Save options dialog
        void FileSaveSelAsm0 (wxCommandEvent &event); // Select generic,dec
        void FileSaveSelAsm1 (wxCommandEvent &event); // Select TASM,dec
        void FileSaveSelAsm2 (wxCommandEvent &event); // Select E-Z80 Way,dec
        void FileSaveSelAsm3 (wxCommandEvent &event); // Select tniASM,dec
        void FileSaveSelAsm4 (wxCommandEvent &event); // Select generic,hex $
        void FileSaveSelAsm5 (wxCommandEvent &event); // Select generic,hex 0x
        void FileSaveSelAsm6 (wxCommandEvent &event); // Select TASM,hex
        void FileSaveSelAsm7 (wxCommandEvent &event); // Select tniASM,hex
        void FileQuit (wxCommandEvent &event);  // Quit program
        void FileSelect (wxCommandEvent &event); // Select an open file
        void EditUndo (wxCommandEvent &event); // Undo
        void EditRedo (wxCommandEvent &event); // Redo
        void EditCopy (wxCommandEvent &event); // Copy area or graphic
        void EditPaste (wxCommandEvent &event); // Paste
        void FillSolid (wxCommandEvent &event); // Fill
        void FillPattern (wxCommandEvent &event); // Fill with Pattern
        void EffInvert (wxCommandEvent &event); // Invert effect
        void EffFlipX (wxCommandEvent &event);  // X Flip effect
        void EffFlipY (wxCommandEvent &event); // Y Flip effect
        void EffRot_left (wxCommandEvent &event); // Left rotation / shift
        void EffRot_right (wxCommandEvent &event); // Right rotation / shift
        void EffRot_up (wxCommandEvent &event); // Up rotation / shift
        void EffRot_down (wxCommandEvent &event); // Down rotation / shift
        void EffRot_clock (wxCommandEvent &event); // Clockwise rotation
        void EffRot_anticlock (wxCommandEvent &event);//Anticlockwise rotation
        void EffOpt (wxCommandEvent &event); // Effect options dialog
        void MaskUse (wxCommandEvent &event); // Toggle mask usage
        void MaskView (wxCommandEvent &event); // Toggle mask show & edition
        void MaskAuto (wxCommandEvent &event); // Automatic mask creation
        void SpriteInsertNext (wxCommandEvent &event); // Insert next frame
        void SpriteInsertPrev (wxCommandEvent &event); //Insert previous frame
        void SpriteMove (wxCommandEvent &event); // Move frame position
        void SpriteRemove (wxCommandEvent &event); // Remove current frame
        void SpritePrev (wxCommandEvent &event); // Select previous frame
        void SpriteNext (wxCommandEvent &event); // Select next frame
        void SpriteGoto (wxCommandEvent &event); // Select frame number
        void GridChar (wxCommandEvent &event); // Toggle char grid usage
        void GridPixel (wxCommandEvent &event); // Toggle pixel grid usage
        void InfoAbout (wxCommandEvent &event); // Show the About dialog

        void ZoomIn (wxCommandEvent &event); // Zoom in
        void DoZoomIn (void);
        void ZoomOut (wxCommandEvent &event); // Zoom out
        void DoZoomOut (void);
        void ZoomAuto (wxCommandEvent &event); // Toggle AutoZoom usage
        void Erase (wxCommandEvent &event);  // Erase
        void ChangeCursor (wxCommandEvent &event); // Change cursor mode

        void SetSelectInk(int c);       // Select a certain ink value
        void SetSelectPaper(int c);     // Select a certain paper value
        void SetSelectBright(int c);    // Select a certain bright value
        void SetSelectFlash(int c);     // Select a certain flash value

        void SetTit();  // Set window title

        void SaveUndo(bool cl=TRUE);  // Save state to undo stack and
                                      //  maybe clear redo stack
        void SaveRedo();  // Save state to redo stack
        void ClearUndo();  // Clears undo & redo stacks

                                        // Open files passed on array
        void OpenArrayFiles(const wxArrayString &filenames);
        void OpenArrayFiles2(char **filenames,int num);

        int GetIniValue(wxString IniFile,wxString VarName,int down,int up,int defaul);
        wxString GetIniValueS(wxString IniFile,wxString VarName,wxString defaul);

        void OnActivate (wxActivateEvent &event); // Process activation
        void OnMouseEdit (wxMouseEvent& event); // Process mouse events
        void OnClose (wxCloseEvent& event); // Intercept close event

        void RfScroll(wxDC& dc); // Update bigescreen on scroll
        void Rfrsh(wxDC& dc, bool draw_bitmaps = TRUE); // Refresh window
        void AreaRfrsh(wxDC& dc, bool draw_bitmaps = TRUE); // Refresh area
        void MacroRfrsh(wxDC& dc, bool draw_bitmaps = TRUE); // Refresh affected area
        void MiniRfrsh(wxDC& dc, int xc, int yc, bool draw_bitmaps = TRUE); // Refresh a character
        void MicroRfrsh(wxDC& dc, int x, int y, bool draw_bitmaps = TRUE); // Refresh a pixel
        void DrawCell(int i, int j);    // Draw a pixel in small bitmaps
        void DrawArea(int i, int j, int i2, int j2); // Draw an area in small bitmaps
        void DrawBigCell(int i, int j); // Draw a pixel in big zoom modes
        void PixelAction(int action, int x, int y); // Make an action on a certain pixel of a char
        void Bresenham (int action, int x, int y); // Draw a line between two points
        void PushQueue (int x, int y); // Insert a pair of values in the fill queue
        bool PopQueue (int& x, int& y); // Take a pair of values of the fill queue, return FALSE is queue is void

        void GetOutputOptions();        // Display the output options dialog
        void GetEffectsOptions();       // Display the effects options dialog

        int GetZoom() {return zoom;}; // Get current zoom

        void ButtonINK (wxCommandEvent &event);    // Set ink from button
        void ButtonPAPER (wxCommandEvent &event);  // Set paper from button 
        void ButtonBRIGHT (wxCommandEvent &event); // Set bright from button
        void ButtonFLASH (wxCommandEvent &event);  // Set down from button
        void MoveOptUP (wxCommandEvent &event);   // Move up a sort priority option
        void MoveOptDOWN (wxCommandEvent &event); // Move down a sort priority option
        void SetZoom (wxCommandEvent &event); // Set a certain zoom
};

// Define a new canvas which can receive some events
class MyWindow: public wxWindow
{
public:
    MyWindow(wxScrolledWindow *frame, int x, int y, int w, int h, long style = wxRETAINED);
    ~MyWindow();
    void OnPaint(wxPaintEvent& event);  // This event is called when the
                                        //  canvas must be repainted
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouse(wxMouseEvent& event);  // All mouse events over the canvas
                                        //  are processed here
    void OnKey(wxKeyEvent& event);      // Get keyboard events
    TheFrame *framee;                   // A pointer to the frame
    int m_status;                       // current mouse status

private:
    DECLARE_EVENT_TABLE()

};

class DnDFile : public wxFileDropTarget
{
public:
        DnDFile(TheFrame *pOwner) { m_pOwner = pOwner; }
        virtual bool OnDropFiles(wxCoord x, wxCoord y,const wxArrayString& filenames);
        TheFrame *m_pOwner;
private:
};

#endif // _THEFRAME_H
