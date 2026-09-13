// OpenFile.cpp
//
// part of SevenuP 1.21 - a Spectrum graphic editor
//
// File management - code
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
  #include "wx/wx.h"
  #include "wx/event.h"
  #include "wx/gdicmn.h"
  #include "wx/scrolwin.h"
  #include "wx/dcclient.h"
  #include "wx/dc.h"
#endif

#include "OpenFile.h"
#include <stdlib.h>

BYTE RPal[16]={28,28,200,200,28,28,200,200,28,28,248,248,28,28,248,248};
BYTE GPal[16]={28,28,28,28,200,200,200,200,28,28,28,28,248,248,248,248};
BYTE BPal[16]={28,200,28,200,28,200,28,200,28,248,28,248,28,248,28,248};

// //Time measurement variables (optimization tests only)
//      wxDateTime Time3;
//      wxDateTime Time4;
//      wxTimeSpan TimeDiff2;

OpenFile::OpenFile()    // NULL constructor
        {
        OpenFrames = 0;
        OpenName = _("");
        OpenPath = _("");
        OpenPropierties=0;
        OpenGraph=NULL;
        flagsure = 0;
        }

// Constructor for new graphs
OpenFile::OpenFile(int x_si, int y_si, bool masked, int newnumber)
        {
        OpenFrames = 1;
        OpenGraph = new SP_Graph(x_si, y_si, TRUE);
        OpenName = _("NEW");
        char ch2='0'+newnumber/10;
        char ch1='0'+newnumber%10;
        char ch0='\0';
        OpenName+=ch2;
        OpenName+=ch1;
        OpenName+=ch0;
        OpenPath = _("");
        flagsure=0;
        OpenPropierties = 1;
        if (masked)
                {
                OpenGraph->Mask = new SP_Graph (x_si, y_si, TRUE);
                OpenPropierties = 2;
                }
        }

OpenFile::OpenFile(std::string path)      // Constructor for graph loading
        {
        char LoadBuff[45000];
        std::ifstream file(s2ws(path).mb_str(wxConvLocal),std::ios::binary);
	int posn; // streampos posn;
        wxString ext;

        ext = s2ws(path).Right(3);
        ext = ext.Upper();
        OpenPath = s2ws(path);

        int searchindxwin=OpenPath.rfind(_("\\"));
        int searchindxunix=OpenPath.rfind(_("/"));
        int namestart=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
        if (namestart>0)        namestart++;
        else                    namestart=0;
        int nameend=OpenPath.Length();
        OpenName = OpenPath.Mid(namestart,nameend-4-namestart); // remove extension
        OpenName+=_('\0');
        OpenGraph=new SP_Graph(0,0);
        if (file.fail())
                {
                wxString strg=s2ws(path)+_(" doesn't exist!");
                (void)wxMessageBox(strg, _("Warning!"));
                OpenGraph->Propied1=0;
                return;
                }
        if (ext==_("SCR"))
                {               // Load .SCR file
                file.read(LoadBuff,6913);
                if(!file.eof())
                        {
                        file.close();
                        wxString strg;
                        strg=s2ws(path)+_(" is an invalid screen file!\n\nPerhaps a Windows screensaver?");
                        (void)wxMessageBox(strg, _("Warning!"));
                        OpenGraph->Propied1=0;
                        }
                else
                        {
			  int highthird,highattr;
			  posn = file.tellg();
			  if ( posn == 2048 ) {
			    highthird = 1; highattr = 0;
			  } else if ( posn == 2048 + 256 ) {
 			    highthird = 1; highattr = 256;
			  } else if ( posn == 4096 ) {
			    highthird = 2; highattr = 0;
			  } else if ( posn == 4096 + 512 ) {
			    highthird = 2; highattr = 512;
			  } else if ( posn == 6144 ) {
			    highthird = 3; highattr = 0;
			  } else  {
			    highthird = 3; highattr = 768;
			  }
                        file.close();
                        flagsure=0;
                        delete OpenGraph;
                        OpenGraph= new SP_Graph(256,192,TRUE);
                        OpenGraph->Propied1=1;
                        OpenGraph->Propied2=0;
                        int parser=0;
                        for (int third=0;third<highthird;third++)
                                for (int charline=0;charline<8;charline++)
                                        for (int line=0;line<8;line++)
                                                for (int character=0;character<32;character++)
                                                        {
                                                        OpenGraph->SetByte((third<<11)+(line<<8)+(character<<3)+charline,LoadBuff[parser++]);
                                                        }
                        for(int character=0;character<highattr;character++)
                                {			
				  if ( character < highattr )
				    OpenGraph->SetAttr(character,LoadBuff[parser++]);
				  else
				    OpenGraph->SetAttr(character,56);
                                }
                        }
                OpenPropierties = OpenGraph->Propied1;
		OpenFrames = (OpenGraph->Propied2)+1;
                }
        else if (ext==_("SEV"))
                {               // Load .SEV file
                file.read(LoadBuff,45000);
                file.close();
                flagsure=0;
                if(LoadBuff[0]!='S'||LoadBuff[1]!='e'||LoadBuff[2]!='v'||LoadBuff[3]!='\0')
                        {
                        wxString strg;
                        strg=s2ws(path)+_(" is an invalid file!");
                        (void)wxMessageBox(strg, _("Warning!"));
                        OpenGraph->Propied1=0;
                        return;
                        }
                else
                        {
                        if (LoadBuff[4]==0&&LoadBuff[5]==0)
                                {       // v0.0 format
                                delete OpenGraph;
                                int P1=(unsigned char)LoadBuff[6]+256*(unsigned char)LoadBuff[7];
                                int P2=(unsigned char)LoadBuff[8]+256*(unsigned char)LoadBuff[9];
                                int SX=(unsigned char)LoadBuff[10]+256*(unsigned char)LoadBuff[11];
                                int SY=(unsigned char)LoadBuff[12]+256*(unsigned char)LoadBuff[13];
                                if ((P1!=1)||(P2!=0)||(SX>256)||(SY>192))
                                        {
                                        wxString strg;
                                        strg=s2ws(path)+_(" is an invalid file!");
                                        (void)wxMessageBox(strg, _("Warning!"));
                                        OpenGraph->Propied1=0;
                                        return;
                                        }
                                OpenGraph= new SP_Graph(SX,SY,TRUE);
                                OpenGraph->Propied1=P1;
                                OpenGraph->Propied2=P2;
                                char *parser=&LoadBuff[14];
                                int nonattrpointer=0;
                                int C_SX=(SX/8)+(SX%8!=0);
                                int C_SY=(SY/8)+(SY%8!=0);
                                for (int i=0;i<C_SX*C_SY;i++)
                                        {
                                        for (int j=0;j<8;j++)
                                                {
                                                OpenGraph->SetByte(nonattrpointer++,*parser++);
                                                };
                                        OpenGraph->SetAttr(i,*parser++);
                                        };
                                }
                        else if (LoadBuff[4]==0&&(LoadBuff[5]==6||LoadBuff[5]==8))
                                {       // v0.6 or v0.8 format
                                delete OpenGraph;
                                SP_Graph *LoadingGraph;
                                int P1=(unsigned char)LoadBuff[6]+256*(unsigned char)LoadBuff[7];
                                int P2=(unsigned char)LoadBuff[8]+256*(unsigned char)LoadBuff[9];
                                int SX=(unsigned char)LoadBuff[10]+256*(unsigned char)LoadBuff[11];
                                int SY=(unsigned char)LoadBuff[12]+256*(unsigned char)LoadBuff[13];
                                if ((P1>2)||(P2>31)||(SX>256)||(SY>192))
                                        {
                                        wxString strg;
                                        strg=s2ws(path)+_(" is an invalid file!");
                                        (void)wxMessageBox(strg, _("Warning!"));
                                        OpenGraph->Propied1=0;
                                        return;
                                        }
                                OpenGraph= new SP_Graph(SX,SY,TRUE);
                                LoadingGraph=OpenGraph;
                                char *parser=&LoadBuff[14];
                                int C_SX=(SX/8)+(SX%8!=0);
                                int C_SY=(SY/8)+(SY%8!=0);
                                for (int nfram=0;nfram<(P2+1);nfram++)
                                        {
                                        int nonattrpointer=0;
                                        LoadingGraph->Propied1=P1;
                                        LoadingGraph->Propied2=P2;
                                        for (int i=0;i<C_SX*C_SY;i++)
                                                {
                                                for (int j=0;j<8;j++)
                                                        {
                                                        LoadingGraph->SetByte(nonattrpointer++,*parser++);
                                                        };
                                                LoadingGraph->SetAttr(i,*parser++);
                                                };
                                        nonattrpointer=0;
                                        if (P1==2)      // 2 means masked
                                                {
                                                LoadingGraph->Mask = new SP_Graph(SX,SY,TRUE);
                                                for (int i=0;i<C_SX*C_SY;i++)
                                                        {
                                                        for (int j=0;j<8;j++)
                                                                {
                                                                LoadingGraph->Mask->SetByte(nonattrpointer++,*parser++);
                                                                };
                                                        };
                                                }
                                        LoadingGraph->NextGraph = new SP_Graph(SX,SY,TRUE);
                                        LoadingGraph = LoadingGraph->NextGraph;
                                        }
                                }
                        else
                                {
                                wxString strg;
                                strg=s2ws(path)+_(" has an invalid version!");
                                (void)wxMessageBox(strg, _("Warning!"));
                                OpenGraph->Propied1=0;
                                }
                        }
                OpenPropierties = OpenGraph->Propied1;
                OpenFrames = (OpenGraph->Propied2)+1;
                }
	else if (ext==_("BMP")||ext==_("GIF")||ext==_("JPG")||ext==_("PNG")||ext==_("PCX")||ext==_("TIF")||ext==_("IFF")||ext==_("XPM"))
                {
                OpenPath=OpenPath.Mid(0,nameend-4);
                OpenPath+=_(".sev\0");
                file.close();
                wxImage *Import;
                Import = new wxImage(s2ws(path));
                if (Import->Ok())
                        {
                        int imagesizex,imagesizey,speccysizex,speccysizey;
                        int xchar,ychar;
                        delete OpenGraph;
                        speccysizex=imagesizex=Import->GetWidth();
                        speccysizey=imagesizey=Import->GetHeight();
                        if (imagesizex>256)
                                {
                                speccysizex=256;
                                speccysizey=(imagesizey*256)/imagesizex;
                                if (speccysizey>192)
                                        {
                                        speccysizey=192;
                                        speccysizex=(imagesizex*192)/imagesizey;
                                        }
                                Import->Rescale(speccysizex,speccysizey);
                                }
                        else if (speccysizey>192)
                                {
                                speccysizey=192;
                                speccysizex=(imagesizex*192)/imagesizey;
                                Import->Rescale(speccysizex,speccysizey);
                                }
                        imagesizex=(speccysizex+7)/8;
                        imagesizey=(speccysizey+7)/8;
                        OpenGraph = new SP_Graph(speccysizex,speccysizey,TRUE);
                        OpenGraph->Propied1=1;
                        OpenGraph->Propied2=0;
                        OpenPropierties = OpenGraph->Propied1;
                        OpenFrames = (OpenGraph->Propied2)+1;
//      Time3=wxDateTime::UNow();
                      for (xchar=0;xchar<imagesizex;xchar++)
                       {
                       for (ychar=0;ychar<imagesizey;ychar++)
                        {
                        // step 1: ANALYSIS
                        int near_[8]={0,0,0,0,0,0,0,0};
                        int near_bright=0;
                        int near_best=0;
                        int near_win=0;
                        int addbright=0;
                        int r,g,b;
                        int diff2=0;
                        int ink=0;
                        int search_best,search_curr,paper=0;
                        int total_r=0,total_g=0,total_b=0;
                        int div4_r[4],div4_g[4],div4_b[4];
                        int div16_r[16],div16_g[16],div16_b[16];
                        int div64_r[64],div64_g[64],div64_b[64];
                        int kkk,ml4y,ml4x,ml2y,ml2x,ml1x,ml1y,spcol; // Bucles

                        for (kkk=0;kkk<16;kkk++)
                                {
                                div4_r[kkk/4]=0;
                                div4_g[kkk/4]=0;
                                div4_b[kkk/4]=0;
                                div16_r[kkk]=0;
                                div16_g[kkk]=0;
                                div16_b[kkk]=0;
                                }

                        for (ml4y=0;ml4y<2;ml4y++)
                                {
                                for (ml4x=0;ml4x<2;ml4x++)
                                        {
                                        for (ml2y=0;ml2y<2;ml2y++)
                                                {
                                                for (ml2x=0;ml2x<2;ml2x++)
                                                        {
                                                        for (ml1y=0;ml1y<2;ml1y++)
                                                                {
                                                                for (ml1x=0;ml1x<2;ml1x++)
                                                                        {
                                                                        if ((8*xchar+4*ml4x+2*ml2x+ml1x<speccysizex)&&(8*ychar+4*ml4y+2*ml2y+ml1y<speccysizey))
                                                                                {
                                                                                r=Import->GetRed(8*xchar+4*ml4x+2*ml2x+ml1x,8*ychar+4*ml4y+2*ml2y+ml1y);
                                                                                g=Import->GetGreen(8*xchar+4*ml4x+2*ml2x+ml1x,8*ychar+4*ml4y+2*ml2y+ml1y);
                                                                                b=Import->GetBlue(8*xchar+4*ml4x+2*ml2x+ml1x,8*ychar+4*ml4y+2*ml2y+ml1y);
                                                                                if (r<28) r=28;
                                                                                if (g<28) g=28;
                                                                                if (b<28) b=28;
                                                                                }
                                                                        div64_r[32*ml4y+8*ml2y+2*ml1y+16*ml4x+4*ml2x+ml1x]=r;
                                                                        div64_g[32*ml4y+8*ml2y+2*ml1y+16*ml4x+4*ml2x+ml1x]=g;
                                                                        div64_b[32*ml4y+8*ml2y+2*ml1y+16*ml4x+4*ml2x+ml1x]=b;
                                                                        div16_r[8*ml4y+4*ml4x+2*ml2y+ml2x]+=r;
                                                                        div16_g[8*ml4y+4*ml4x+2*ml2y+ml2x]+=g;
                                                                        div16_b[8*ml4y+4*ml4x+2*ml2y+ml2x]+=b;
                                                                        search_best=16500;
                                                                        for (spcol=1;spcol<16;spcol++)
                                                                                {
                                                                                search_curr=(r-RPal[spcol])*(r-RPal[spcol])+(g-GPal[spcol])*(g-GPal[spcol])+(b-BPal[spcol])*(b-BPal[spcol]);
                                                                                if (search_curr<search_best)
                                                                                        {
                                                                                        search_best=search_curr;
                                                                                        near_best=spcol;
                                                                                        }
                                                                                }
                                                                        near_[near_best%8]++;
                                                                        if (near_[near_best%8]>near_win)
                                                                                {
                                                                                paper=near_best%8;
                                                                                near_win++;
                                                                                }
                                                                        near_bright+=(near_best/8);
                                                                        diff2+=search_best;
                                                                        }
                                                                }
                                                                div4_r[2*ml4y+ml4x]+=div16_r[8*ml4y+4*ml4x+2*ml2y+ml2x];
                                                                div4_g[2*ml4y+ml4x]+=div16_g[8*ml4y+4*ml4x+2*ml2y+ml2x];
                                                                div4_b[2*ml4y+ml4x]+=div16_b[8*ml4y+4*ml4x+2*ml2y+ml2x];
                                                        }
                                                }
                                                total_r+=div4_r[2*ml4y+ml4x];
                                                total_g+=div4_g[2*ml4y+ml4x];
                                                total_b+=div4_b[2*ml4y+ml4x];
                                        }
                                }
                        if  ((near_bright-near_[0])>((64-near_[0])/2)) addbright=8;
                        paper+=addbright;

                        ink=addbright;
                        int minnear = 0;
                        int c,p;  // Bucles

                        for (c=0;c<8;c++)
                                {
                                if ((near_[c]>minnear)&&(c!=(paper%8)))
                                        {
                                        ink=c+addbright;
                                        minnear=near_[c];
                                        }
                                }

                        if ((diff2>15000)||((near_[paper%8]+minnear)!=64))
                        {
                        // End of step 1: Analysis
                        // START STEP 2: Choose colors

                        int best_so_far=800000001; // Step 2
                        int best_p=65;
                        int best_this_col,best_this_p;
                        int red_ap,blue_ap,green_ap;
                        int this_ap;

                        if (minnear <12 ) minnear = 0;

                        ink=17;
                        for (c=addbright;c<(8+addbright);c++)
                                {
                                if (c!=paper)
                                        {
                                        best_this_col=800000000;
                                        best_this_p=65;
                                        if (near_[c%8]>(minnear-1))
                                                {
                                                for (p=minnear;p<65;p++)
                                                        {
                                                        red_ap=RPal[paper]*(64-p)+RPal[c]*p;
                                                        blue_ap=BPal[paper]*(64-p)+BPal[c]*p;
                                                        green_ap=GPal[paper]*(64-p)+GPal[c]*p;
                                                        this_ap=(red_ap-total_r)*(red_ap-total_r)+(blue_ap-total_b)*(blue_ap-total_b)+(green_ap-total_g)*(green_ap-total_g);
                                                        if (this_ap<best_this_col)
                                                                {
                                                                best_this_col=this_ap;
                                                                best_this_p=p;
                                                                }
                                                        }
                                                }
                                        if (best_this_col<best_so_far)
                                                {
                                                best_so_far=best_this_col;
                                                best_p=best_this_p;
                                                ink=c;
                                                }
                                        }
                                }
                        if (paper>ink)
                                {
                                int temp=ink;
                                ink=paper;
                                paper=temp;
                                best_p=64-best_p;
                                }
                        OpenGraph->SetAttr(xchar+ychar*imagesizex,paper*8+ink%8);

                        // End of STEP 2
                        // SO FAR:      paper_color=paper
                        //              ink_color=ink
                        //              number of paper pixels=best_p
                        // STEP 3

                        // DISTRIBUTION - FIRST STEP

                        int prop[12];
                        int p1[4];

                        prop[0] = div4_r[0];
                        prop[1] = div4_r[1];
                        prop[2] = div4_r[2];
                        prop[3] = div4_r[3];
                        prop[4] = div4_g[0];
                        prop[5] = div4_g[1];
                        prop[6] = div4_g[2];
                        prop[7] = div4_g[3];
                        prop[8] = div4_b[0];
                        prop[9] = div4_b[1];
                        prop[10] = div4_b[2];
                        prop[11] = div4_b[3];
                        
                        Distribute(prop,best_p,16,paper,ink,p1);

                        for (int secondpass=0;secondpass<4;secondpass++)
                                {
                                int pp[4];
                                prop[0] = div16_r[4*secondpass+0];
                                prop[1] = div16_r[4*secondpass+1];
                                prop[2] = div16_r[4*secondpass+2];
                                prop[3] = div16_r[4*secondpass+3];
                                prop[4] = div16_g[4*secondpass+0];
                                prop[5] = div16_g[4*secondpass+1];
                                prop[6] = div16_g[4*secondpass+2];
                                prop[7] = div16_g[4*secondpass+3];
                                prop[8] = div16_b[4*secondpass+0];
                                prop[9] = div16_b[4*secondpass+1];
                                prop[10] = div16_b[4*secondpass+2];
                                prop[11] = div16_b[4*secondpass+3];
                                Distribute(prop,p1[secondpass],4,paper,ink,pp);
                                for (int thirdpass=0;thirdpass<4;thirdpass++)
                                {
                                        int ppp[4];
                                        prop[0] = div64_r[16*secondpass+4*thirdpass+0];
                                        prop[1] = div64_r[16*secondpass+4*thirdpass+1];
                                        prop[2] = div64_r[16*secondpass+4*thirdpass+2];
                                        prop[3] = div64_r[16*secondpass+4*thirdpass+3];
                                        prop[4] = div64_g[16*secondpass+4*thirdpass+0];
                                        prop[5] = div64_g[16*secondpass+4*thirdpass+1];
                                        prop[6] = div64_g[16*secondpass+4*thirdpass+2];
                                        prop[7] = div64_g[16*secondpass+4*thirdpass+3];
                                        prop[8] = div64_b[16*secondpass+4*thirdpass+0];
                                        prop[9] = div64_b[16*secondpass+4*thirdpass+1];
                                        prop[10] = div64_b[16*secondpass+4*thirdpass+2];
                                        prop[11] = div64_b[16*secondpass+4*thirdpass+3];
  
                                        Distribute(prop,pp[thirdpass],1,paper,ink,ppp);
                                        for (int ppixels=0;ppixels<4;ppixels++)
                                                {
                                                int pixi = 8*xchar+4*(secondpass%2)+2*(thirdpass%2)+ppixels%2;
                                                int pixj = 8*ychar+4*(secondpass/2)+2*(thirdpass/2)+ppixels/2;
                                                if (ppp[ppixels]==1) OpenGraph->SetPixel(pixi,pixj);
                                                }
                                        }
                                }
                        }
                        else
                        {
                        if (paper>ink)
                                {
                                int temp=ink;
                                ink=paper;
                                paper=temp;
                                }

                        OpenGraph->SetAttr(xchar+ychar*imagesizex,paper*8+ink%8);
                        for (int secondpass=0;secondpass<4;secondpass++)
                                for (int thirdpass=0;thirdpass<4;thirdpass++)
                                        for (int pp=0;pp<4;pp++)
                                                {
                                                int near_pap=(div64_r[16*secondpass+4*thirdpass+pp]-RPal[paper])*(div64_r[16*secondpass+4*thirdpass+pp]-RPal[paper])+(div64_g[16*secondpass+4*thirdpass+pp]-GPal[paper])*(div64_g[16*secondpass+4*thirdpass+pp]-GPal[paper])+(div64_b[16*secondpass+4*thirdpass+pp]-BPal[paper])*(div64_b[16*secondpass+4*thirdpass+pp]-BPal[paper]);
                                                int near_ink=(div64_r[16*secondpass+4*thirdpass+pp]-RPal[ink])*(div64_r[16*secondpass+4*thirdpass+pp]-RPal[ink])+(div64_g[16*secondpass+4*thirdpass+pp]-GPal[ink])*(div64_g[16*secondpass+4*thirdpass+pp]-GPal[ink])+(div64_b[16*secondpass+4*thirdpass+pp]-BPal[ink])*(div64_b[16*secondpass+4*thirdpass+pp]-BPal[ink]);
                                                if (near_ink<near_pap)
                                                        {
                                                        int pixi = 8*xchar+4*(secondpass%2)+2*(thirdpass%2)+pp%2;
                                                        int pixj = 8*ychar+4*(secondpass/2)+2*(thirdpass/2)+pp/2;
                                                        OpenGraph->SetPixel(pixi,pixj);
                                                        }
                                                }
                        }

                        }
                       }
//      Time4=wxDateTime::UNow();
//      TimeDiff2=Time4-Time3;
//      long k=TimeDiff2.GetMilliseconds().ToLong();
//      std::cout<<_("Conversion time = ")<<k<<std::endl<<std::flush;
                        }
                else
                        {
                        wxString strg;
                        strg=s2ws(path)+_(" is an invalid file!");
                        (void)wxMessageBox(strg, _("Warning!"));
                        OpenGraph->Propied1=0;
                        return;
                        }
                }
        else
                {
                file.close();
                wxString strg;
                strg=s2ws(path)+_(" is an invalid file!");
                (void)wxMessageBox(strg, _("Warning!"));
                OpenGraph->Propied1=0;
                }
        }

OpenFile::~OpenFile()   // Destructor
        {
        if (OpenGraph != NULL) delete OpenGraph;
        }

void OpenFile::Distribute(int* prop,int pixels,int cellsize,int paper,int ink,int* p)
{
        unsigned int best_this_col=3200000001u;
        int threshold=2000;
        int i0,i1,i2,i3;
        int red_ap0,green_ap0,blue_ap0;
        int red_ap1,green_ap1,blue_ap1;
        int red_ap2,green_ap2,blue_ap2;
        int red_ap3,green_ap3,blue_ap3;
        int this_ap_r,this_ap_g,this_ap_b;
        unsigned int this_ap;

        p[0]=0;
        p[1]=0;
        p[2]=0;
        p[3]=0;

        if (pixels==0) return;
	if (pixels==(cellsize*4))
		{
        	p[0]=cellsize;
        	p[1]=cellsize;
        	p[2]=cellsize;
        	p[3]=cellsize;
        	return;
		}
        for (i2=0;(i2<(pixels+1))&&(i2<=cellsize);i2++)
                {
                red_ap2=RPal[paper]*(cellsize-i2)+RPal[ink]*i2;
                green_ap2=GPal[paper]*(cellsize-i2)+GPal[ink]*i2;
                blue_ap2=BPal[paper]*(cellsize-i2)+BPal[ink]*i2;
                for (i1=0;i1<((pixels-i2+1))&&(i1<=cellsize);i1++)
                        {
                        red_ap1=RPal[paper]*(cellsize-i1)+RPal[ink]*i1;
                        green_ap1=GPal[paper]*(cellsize-i1)+GPal[ink]*i1;
                        blue_ap1=BPal[paper]*(cellsize-i1)+BPal[ink]*i1;
                        for (i3=0;(i3<(pixels-i2-i1+1))&&(i3<=cellsize);i3++)
                                {
                                i0 = pixels-i3-i1-i2;
                                if (i0<=cellsize)
                                	{
                                	red_ap0=RPal[paper]*(cellsize-i0)+RPal[ink]*i0;
                                	green_ap0=GPal[paper]*(cellsize-i0)+GPal[ink]*i0;
                                	blue_ap0=BPal[paper]*(cellsize-i0)+BPal[ink]*i0;
                                	red_ap3=RPal[paper]*(cellsize-i3)+RPal[ink]*i3;
                                	green_ap3=GPal[paper]*(cellsize-i3)+GPal[ink]*i3;
                                	blue_ap3=BPal[paper]*(cellsize-i3)+BPal[ink]*i3;
                                	this_ap_r=(red_ap0-prop[0])*(red_ap0-prop[0])+(red_ap1-prop[1])*(red_ap1-prop[1])+(red_ap2-prop[2])*(red_ap2-prop[2])+(red_ap3-prop[3])*(red_ap3-prop[3]);
                                	this_ap_g=(green_ap0-prop[4])*(green_ap0-prop[4])+(green_ap1-prop[5])*(green_ap1-prop[5])+(green_ap2-prop[6])*(green_ap2-prop[6])+(green_ap3-prop[7])*(green_ap3-prop[7]);
                                	this_ap_b=(blue_ap0-prop[8])*(blue_ap0-prop[8])+(blue_ap1-prop[9])*(blue_ap1-prop[9])+(blue_ap2-prop[10])*(blue_ap2-prop[10])+(blue_ap3-prop[11])*(blue_ap3-prop[11]);
                                	this_ap=this_ap_r+this_ap_g+this_ap_b;
                                	if ((this_ap+threshold)<best_this_col)
	                                        {
                                        	best_this_col=this_ap;
                                        	p[0]=i0;
                                        	p[1]=i1;
                                        	p[2]=i2;
                                        	p[3]=i3;
                                        	}
					}
                                }
                        }
                }
}

void OpenFile::Export(std::string filenam,int framenumber)   // Save image from graphic
        {
        wxImage Export;
        SP_Graph *Graphic3;

        Graphic3= OpenGraph;
        for (int frms=0;frms<(framenumber-1);frms++)
                {
                Graphic3=Graphic3->NextGraph;
                }
        Export = Graphic3->SP_Screen[0]->ConvertToImage();
        if (Export.SaveFile(s2ws(filenam))!=TRUE)
                {
                wxString strg=_("Saving ")+s2ws(filenam)+_(" failed!");
                (void)wxMessageBox(strg, _("Warning!"));
                }
        }

void OpenFile::Save(std::string filenam)   // Save .SEV (or .SCR) graphic
        {
        wxString ext;                   //  The SCR case checked here
        ext = s2ws(filenam).Right(3);         // happens when we save a screen
        ext = ext.Upper();              // using the menu option "Save"
        if (ext==_("SCR"))                 // instead of the usual "Save as"
                {
                SaveSCR(filenam);
                return;
                }

        char SaveBuff[45000];
        char *parser=SaveBuff;

        SP_Graph *SavingGraph;
        SavingGraph=OpenGraph;

        std::ofstream file;
        OpenGraph->Propied1=OpenPropierties;
        OpenGraph->Propied2=OpenFrames-1;
        *parser++='S';
        *parser++='e';
        *parser++='v';
        *parser++='\0';
        *parser++=0;
        *parser++=8;
        *parser++=OpenPropierties%256;
        *parser++=OpenPropierties/256;
        *parser++=(OpenFrames-1)%256;
        *parser++=(OpenFrames-1)/256;
        *parser++=OpenGraph->GetSizeX()%256;
        *parser++=OpenGraph->GetSizeX()/256;
        *parser++=OpenGraph->GetSizeY()%256;
        *parser++=OpenGraph->GetSizeY()/256;

        int size=14;
        for (int nfram=0;nfram<OpenFrames;nfram++)
                {
                int nonattrpointer=0;
                for (int i=0;i<OpenGraph->x_charsize*OpenGraph->y_charsize;i++)
                        {
                        for (int j=0;j<8;j++)
                                {
                                *parser++=SavingGraph->GetByte(nonattrpointer++);
                                }
                        *parser++=SavingGraph->GetAttr(i);
                        }
                size+=(OpenGraph->x_charsize*OpenGraph->y_charsize*9);
                if (OpenGraph->Propied1==2)     // Masked?
                        {
                        nonattrpointer=0;
                        for (int i=0;i<OpenGraph->x_charsize*OpenGraph->y_charsize;i++)
                                {
                                for (int j=0;j<8;j++)
                                        {
                                        *parser++=SavingGraph->Mask->GetByte(nonattrpointer++);
                                        }
                                }
                        size+=(OpenGraph->x_charsize*OpenGraph->y_charsize*8);
                        }
                SavingGraph=SavingGraph->NextGraph;
                }
        file.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary);
        file.write(SaveBuff,size);
        file.close();
        int searchindxwin=s2ws(filenam).rfind(_("\\"));
        int searchindxunix=s2ws(filenam).rfind(_("/"));
        int namestart=(searchindxwin>searchindxunix)?searchindxwin:searchindxunix;
        namestart++;
        int nameend=s2ws(filenam).Length();
        OpenName = s2ws(filenam).Mid(namestart,nameend-4-namestart); // remove extension
        OpenName+= _('\0');
        flagsure = 0;
        }

void OpenFile::SaveSCR(std::string filenam)        // Save .SCR screen
        {
        char SaveBuff[6912];
        char *parser=SaveBuff;
        std::ofstream file;

        for(int third=0;third<3;third++)
                for (int charline=0;charline<8;charline++)
                        for (int line=0;line<8;line++)
                                for (int character=0;character<32;character++)
                                        {
                                        *parser++=OpenGraph->GetByte((third<<11)+(line<<8)+(character<<3)+charline);
                                        }
        for(int character=0;character<768;character++)
                {
                *parser++=OpenGraph->GetAttr(character);
                }
        file.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary);
        file.write(SaveBuff,6912);
        file.close();
        flagsure = 0;
        }

// Save .BIN file
void OpenFile::SaveBIN(std::string filenam,int opts,int p0,int p1,int p2,int p3,int p4,int maskrev,int appendfile, int zigzag,int AttrMask,int AttrMaskValue,int interleave,int z88dk)
        {
        int pr[6];      // Propierties
        int rp[6];      // Reverse priorities
        SP_Graph *GraphSave[2];
        SP_Graph *GraphFrame;

        pr[0]=p0;               // Assign priorities
        pr[1]=p1;
        pr[2]=p2;
        pr[3]=p3;
        pr[4]=p4;
        pr[5]=5;
	rp[5]=5;
        for (int i=0;i<5;i++)   // Get reverse priorities
                {
                for (int k=0;k<5;k++)
                        {
                        if (pr[i]==k) rp[k]=i;
                        }
                }

        int lim[6]={OpenGraph->x_charsize,8,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1}; // Graphic limits
        int atlim[6]={OpenGraph->x_charsize,1,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1};    // Attribute limits

        int lop[5]; // Loop variables

	char GraphBuff[40000];
	char AttrBuff[5000];
        char SaveBuff[45000];
        char SizeBuff[2];
        char *parserGraph=GraphBuff;
        char *parserAttr=AttrBuff;
        char *parserSize=SizeBuff;
        char *parser=SaveBuff;

        std::ofstream binfile;
        int saisGr=0;
        int saisAt=0;
	int sais=0;
        for (lop[4]=0;lop[4]<lim[pr[4]];lop[4]++)
                {
                for (lop[3]=0;lop[3]<lim[pr[3]];lop[3]++)
                        {
                        for (lop[2]=0;lop[2]<lim[pr[2]];lop[2]++)
                                {
                                for (lop[1]=0;lop[1]<lim[pr[1]];lop[1]++)
                                        {
                                        for (lop[0]=0;lop[0]<lim[pr[0]];lop[0]++)
                                                {
                                                GraphFrame=OpenGraph;
                                                for (int i=0;i<lop[rp[4]];i++)
                                                	{
                                                        GraphFrame=GraphFrame->NextGraph;
                                                        }
						if (OpenPropierties==2)
                                                	{
                                                        GraphSave[maskrev]=GraphFrame;
                                                        GraphSave[1-maskrev]=GraphFrame->Mask;
                                                        }
						else	{
                                                	GraphSave[0]=GraphFrame;
                                                        }
						int horiz=lop[rp[0]];
						if ((zigzag==1)&&(rp[1]>rp[0])&&((lop[rp[1]]%2)==1))
							{
							horiz=OpenGraph->x_charsize-horiz-1;
							}
						*parserGraph++=GraphSave[lop[rp[3]]]->GetByte(horiz+OpenGraph->x_charsize*lop[rp[2]],lop[rp[1]]);
                                                saisGr++;
                                                }
                                        }
                                }
                        }
                }

	for (lop[4]=0;lop[4]<atlim[pr[4]];lop[4]++)
        	{
                for (lop[3]=0;lop[3]<atlim[pr[3]];lop[3]++)
                	{
                        for (lop[2]=0;lop[2]<atlim[pr[2]];lop[2]++)
                        	{
                                for (lop[1]=0;lop[1]<atlim[pr[1]];lop[1]++)
                                	{
                                        for (lop[0]=0;lop[0]<atlim[pr[0]];lop[0]++)
                                        	{
                                                GraphFrame=OpenGraph;
                                                for (int i=0;i<lop[rp[4]];i++)
                                                	{
                                                        GraphFrame=GraphFrame->NextGraph;
                                                        }
						if (OpenPropierties==1)
	                                        	{
							*parserAttr++=GraphFrame->GetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]]);
                                                        saisAt++;
							}
						else 	{
							if (maskrev==lop[rp[3]])
								{
								*parserAttr++=GraphFrame->GetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]]);
                                                       		saisAt++;
								}
							else	{
								if (AttrMask==1)
									{
									*parserAttr++=AttrMaskValue;
									saisAt++;
									}
								}
							}
                                                }
					}
                                }
                        }
                }
                
	if (appendfile) binfile.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary|std::ios::app);
	else binfile.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary);
	if (z88dk==1)
		{
		*parserSize++=OpenGraph->GetSizeY();
		*parserSize++=OpenGraph->GetSizeX()%256;
		binfile.write(SizeBuff,2);
		}

	// Mixer
	parserGraph-=saisGr;
	parserAttr-=saisAt;
	switch(opts)
		{
		case 0:	// Graph+Attr
		case 1: // Attr+Graph
			{
			sais=saisGr+saisAt;
			// Calculate attribute interleaving point
			if (rp[interleave]<rp[1])
				{
				interleave=1;
				}
			int multGr=1;
			for (int i=0;i<=rp[interleave];i++)
				{
				multGr*=lim[pr[i]];
				}
			int multAt=multGr/8;
			int multMask=0;
			int steps=saisGr/multGr;
			if (saisGr/saisAt==16)
				{
				if (rp[3]<rp[interleave])
					{
					multAt/=2;
					}
				else	{
					multMask=1;
					for (int i=(rp[interleave]+1);i<rp[3];i++)
						{
						multMask*=lim[pr[i]];
						}
					}
				}
			int idx=0;
			for(int i=0;i<steps;i++)
				{
				if (opts==0)
					{
					for(int j=0;j<multGr;j++)
						{
						*parser++=*parserGraph++;
						}
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parser++=*parserAttr++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parser++=*parserAttr++;
								}
							}
						idx++;
						}
					}
				else	{
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parser++=*parserAttr++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parser++=*parserAttr++;
								}
							}
						idx++;
						}
					for(int j=0;j<multGr;j++)
						{
						*parser++=*parserGraph++;
						}
					}
				}
        		binfile.write(SaveBuff,sais);
			break;
			}
		case 2: // Graph
			{
			// BUGGG
        		binfile.write(GraphBuff,saisGr);
			break;
			}
		case 3: // Attr
			{
        		binfile.write(AttrBuff,saisAt);
			break;
			}
		}
        binfile.close();
        }
        
void OpenFile::SaveASM(std::string filenam,int opts,int p0,int p1,int p2,int p3,int p4,int maskrev,int asmtype,int appendfile,int zigzag,int AttrMask,int AttrMaskValue,int interleave,int z88dk,int nolabel)
        {
        int outlinelim=8;
        int bytewidth=3;

        wxString HexPref=_("");
        wxString DefbStrg=_("\tDEFB\t");
	wxString Label1=OpenName;
	wxString Label2=_(":");
        wxString strs1_file[] = {_("Gfx+Attr"),_("Attr+Gfx"),_("Gfx"),_("Attr")};
        wxString strs2_file[] = {_("X char"),_("Char line"),_("Y char"),_("Mask"),_("Frame number"),_("Sprite")};
        wxString strs3_file[] = {_("Line"),_("Character"),_("Column"),_(""),_("Frames"),_("Sprite")};
        wxString strs4_file[] = {_("No"),_("Yes"),_("Yes, before graphic"),_("Yes, including attributes"),_("Yes, including attributes and before graphic")};

        int pr[6];      // Propierties
        int rp[6];      // Reverse priorities
        SP_Graph *GraphSave[2];
        SP_Graph *GraphFrame;

        pr[0]=p0;               // Assign priorities
        pr[1]=p1;
        pr[2]=p2;
        pr[3]=p3;
        pr[4]=p4;
	pr[5]=5;
	rp[5]=5;
        for (int i=0;i<5;i++)   // Get reverse priorities
                {
                for (int k=0;k<5;k++)
                        {
                        if (pr[i]==k) rp[k]=i;
                        }
                }

        int lim[6]={OpenGraph->x_charsize,8,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1}; // Graphic limits
        int atlim[6]={OpenGraph->x_charsize,1,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1};    // Attribute limits
        int lop[5]; // Loop variables


        std::ofstream asmfile;
        if (appendfile)
	    asmfile.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary|std::ios::app);
	else
	    asmfile.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary);
        asmfile << "; ASM source file created by SevenuP v1.21"<<std::endl;
        asmfile << "; SevenuP (C) Copyright 2002-2007 by Jaime Tejedor Gomez, aka Metalbrain"<<std::endl;

        switch (asmtype)
                {
                case 0: break;
                case 1: {
                        asmfile << "; Target Assembler: TASM"<<std::endl;
                        DefbStrg=_("\t.BYTE\t");
                        break;
                        }
                case 2: {
                        asmfile << "; Target Assembler: The E-Z80 Way"<<std::endl;
                        DefbStrg=_("              DEFB ");
                        outlinelim=1;
			Label1=OpenName;
			if (Label1.Len()>13)
				{
				Label1=Label1.Left(13);
				}
			Label2=_("");
                        break;
                        }
                case 3: {
                        asmfile << "; Target Assembler: tniASM"<<std::endl;
                        DefbStrg=_("\tDB\t");
                        break;
                        }
                case 4: {
                        HexPref=_("$");
                        bytewidth=2;
                        break;
                        }
                case 5: {
                        HexPref=_("0x");
                        bytewidth=2;
                        break;
                        }
                case 6: {
                        HexPref=_("$");
                        bytewidth=2;
                        asmfile << "; Target Assembler: TASM"<<std::endl;
                        DefbStrg=_("\t.BYTE\t");
                        break;
                        }
                case 7: {
                        HexPref=_("$");
                        bytewidth=2;
                        asmfile << "; Target Assembler: tniASM"<<std::endl;
                        DefbStrg=_("\tDB\t");
                        break;
                        }
                }

        asmfile << std::endl;
        asmfile << ";GRAPHIC DATA:"<<std::endl;
        asmfile << ";Pixel Size:      ("<<std::setw(3)<<OpenGraph->GetSizeX()<<", "<<std::setw(3)<<OpenGraph->GetSizeY()<<")"<<std::endl;
        asmfile << ";Char Size:       ("<<std::setw(3)<<OpenGraph->x_charsize<<", "<<std::setw(3)<<OpenGraph->y_charsize<<")"<<std::endl;
        if (OpenFrames>1)       asmfile << ";Frames:           "<<std::setw(3)<<OpenFrames<<std::endl;

        int sortpoutputted=0;
        int atsortpoutputted=0;
        for (int sortpout=0;sortpout<5;sortpout++)
                {
                if (lim[pr[sortpout]]!=1)
                        {
                        if (sortpoutputted==0)
                                {
                                asmfile << ";Sort Priorities: ";
                                }
                        else
                                {
                                asmfile << ", ";
                                }
                        asmfile<<ws2s(strs2_file[pr[sortpout]]);
//                        ent[sortpout]=sortpoutputted;
                        sortpoutputted++;
                        }
                if (atlim[pr[sortpout]]!=1)
                        {
//                        atent[sortpout]=atsortpoutputted;
                        atsortpoutputted++;
                        }
                }
        asmfile <<std::endl;
        asmfile << ";Data Outputted:  "<<ws2s(strs1_file[opts])<<std::endl;
        asmfile << ";Interleave:      "<<ws2s(strs3_file[interleave])<<std::endl;
        asmfile << ";Mask:            "<<ws2s(strs4_file[OpenPropierties-1+maskrev*(OpenPropierties-1)+2*AttrMask*(OpenPropierties-1)])<<std::endl;
        if (AttrMask==1)
		asmfile << ";Attribute Mask:  "<<std::setw(3)<<AttrMaskValue<<std::endl;
	if ((zigzag==1)&&(rp[1]>rp[0]))
		asmfile << ";Zigzag:          Horizontal"<<std::endl;
	if (z88dk==1)
		asmfile << ";First two bytes are y and x pixel size, for z88dk libraries"<<std::endl;
        asmfile << std::endl;

	if (nolabel==0)
		{
        	asmfile <<ws2s(Label1)<<ws2s(Label2)<<std::endl;
        	}

        if (asmtype>3)
                {
                asmfile <<std::setbase(16);
                asmfile <<std::setiosflags(std::ios::uppercase);
                asmfile <<std::setfill('0');
                }

	// Generate binary output
	unsigned char GraphBuff[40000];
	unsigned char AttrBuff[5000];
        unsigned char SaveBuff[45000];
        unsigned char *parserGraph=GraphBuff;
        unsigned char *parserAttr=AttrBuff;
        unsigned char *parser=SaveBuff;
        unsigned char *parserfinal=SaveBuff;
        
        std::ofstream binfile;
        int saisGr=0;
        int saisAt=0;
	int sais=0;
        for (lop[4]=0;lop[4]<lim[pr[4]];lop[4]++)
                {
                for (lop[3]=0;lop[3]<lim[pr[3]];lop[3]++)
                        {
                        for (lop[2]=0;lop[2]<lim[pr[2]];lop[2]++)
                                {
                                for (lop[1]=0;lop[1]<lim[pr[1]];lop[1]++)
                                        {
                                        for (lop[0]=0;lop[0]<lim[pr[0]];lop[0]++)
                                                {
                                                GraphFrame=OpenGraph;
                                                for (int i=0;i<lop[rp[4]];i++)
                                                	{
                                                        GraphFrame=GraphFrame->NextGraph;
                                                        }
						if (OpenPropierties==2)
                                                	{
                                                        GraphSave[maskrev]=GraphFrame;
                                                        GraphSave[1-maskrev]=GraphFrame->Mask;
                                                        }
						else	{
                                                	GraphSave[0]=GraphFrame;
                                                        }
						int horiz=lop[rp[0]];
						if ((zigzag==1)&&(rp[1]>rp[0])&&((lop[rp[1]]%2)==1))
							{
							horiz=OpenGraph->x_charsize-horiz-1;
							}
						*parserGraph++=GraphSave[lop[rp[3]]]->GetByte(horiz+OpenGraph->x_charsize*lop[rp[2]],lop[rp[1]]);
                                                saisGr++;
                                                }
                                        }
                                }
                        }
                }
	for (lop[4]=0;lop[4]<atlim[pr[4]];lop[4]++)
        	{
                for (lop[3]=0;lop[3]<atlim[pr[3]];lop[3]++)
                	{
                        for (lop[2]=0;lop[2]<atlim[pr[2]];lop[2]++)
                        	{
                                for (lop[1]=0;lop[1]<atlim[pr[1]];lop[1]++)
                                	{
                                        for (lop[0]=0;lop[0]<atlim[pr[0]];lop[0]++)
                                        	{
                                                GraphFrame=OpenGraph;
                                                for (int i=0;i<lop[rp[4]];i++)
                                                	{
                                                        GraphFrame=GraphFrame->NextGraph;
                                                        }
						if (OpenPropierties==1)
	                                        	{
							*parserAttr++=GraphFrame->GetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]]);
                                                        saisAt++;
							}
						else 	{
							if (maskrev==lop[rp[3]])
								{
								*parserAttr++=GraphFrame->GetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]]);
                                                       		saisAt++;
								}
							else	{
								if (AttrMask==1)
									{
									*parserAttr++=AttrMaskValue;
									saisAt++;
									}
								}
							}
                                                }
					}
                                }
                        }
                }
	// Mixer
	parserGraph-=saisGr;
	parserAttr-=saisAt;

	switch(opts)
		{
		case 0:	// Graph+Attr
		case 1: // Attr+Graph
			{
			sais=saisGr+saisAt;
			// Calculate attribute interleaving point
			if (rp[interleave]<rp[1])
				{
				interleave=1;
				}
			int multGr=1;
			for (int i=0;i<=rp[interleave];i++)
				{
				multGr*=lim[pr[i]];
				}
			int multAt=multGr/8;
			int multMask=0;
			int steps=saisGr/multGr;
			if (saisGr/saisAt==16)
				{
				if (rp[3]<rp[interleave])
					{
					multAt/=2;
					}
				else	{
					multMask=1;
					for (int i=(rp[interleave]+1);i<rp[3];i++)
						{
						multMask*=lim[pr[i]];
						}
					}
				}
			int idx=0;
			for(int i=0;i<steps;i++)
				{
				if (opts==0)
					{
					for(int j=0;j<multGr;j++)
						{
						*parser++=*parserGraph++;
						}
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parser++=*parserAttr++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parser++=*parserAttr++;
								}
							}
						idx++;
						}
					}
				else	{
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parser++=*parserAttr++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parser++=*parserAttr++;
								}
							}
						idx++;
						}
					for(int j=0;j<multGr;j++)
						{
						*parser++=*parserGraph++;
						}
					}
				}
			parser=parserfinal;
			break;
			}
		case 2: // Graph
			{
			parser=parserGraph;
			sais=saisGr;
			break;
			}
		case 3: // Attr
			{
			parser=parserAttr;
			sais=saisAt;
			break;
			}
		}
	if (z88dk==1)
		{
		asmfile<<ws2s(DefbStrg);
		asmfile << ws2s(HexPref);
		if (asmtype!=2) asmfile<<std::setw(bytewidth);
		asmfile<<OpenGraph->GetSizeY();
		asmfile<<",";
		asmfile<<ws2s(HexPref);
		if (asmtype!=2) asmfile<<std::setw(bytewidth);
		asmfile<<OpenGraph->GetSizeX()%256;
                asmfile<<std::endl;		
		}

        int alreadyinline=0;
	for (int i=0;i<sais;i++)
		{
		if (alreadyinline==outlinelim)
                	{
                        asmfile<<std::endl;
                        alreadyinline=0;
                        }
                if (alreadyinline==0) asmfile<<ws2s(DefbStrg);
                else asmfile<<",";
		asmfile << ws2s(HexPref);
		if (asmtype!=2) asmfile<<std::setw(bytewidth);
                asmfile<<(int)parser[i];
		alreadyinline++;
		}

        if (asmtype>3) asmfile << std::setbase(10);
        if (asmtype>3) asmfile << std::setfill(' ');
        asmfile.close();
        }

// Save .C source
void	OpenFile::SaveC(std::string filenam,int opts,int p0,int p1,int p2,int p3,int p4,int maskrev,int appendfile, int zigzag,int AttrMask,int AttrMaskValue,int interleave,int z88dk)
        {
        wxString strs1_file[] = {_("Gfx+Attr"),_("Attr+Gfx"),_("Gfx"),_("Attr")};
        wxString strs2_file[] = {_("X char"), _("Char line"), _("Y char"), _("Mask"), _("Frame number"),_("Sprite")};
        wxString strs3_file[] = {_("Line"),_("Character"),_("Column"),_(""),_("Frames"),_("Sprite")};
        wxString strs4_file[] = {_("No"),_("Yes"),_("Yes, before graphic"),_("Yes, including attributes"),_("Yes, including attributes and before graphic")};

        int pr[6];      // Propierties
        int rp[6];      // Reverse priorities
        SP_Graph *GraphSave[2];
        SP_Graph *GraphFrame;

        pr[0]=p0;               // Assign priorities
        pr[1]=p1;
        pr[2]=p2;
        pr[3]=p3;
        pr[4]=p4;
	pr[5]=5;
	rp[5]=5;
	
        for (int i=0;i<5;i++)   // Get reverse priorities
                {
                for (int k=0;k<5;k++)
                        {
                        if (pr[i]==k) rp[k]=i;
                        }
                }

        int lim[6]={OpenGraph->x_charsize,8,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1}; // Loop limits
        int atlim[6]={OpenGraph->x_charsize,1,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1};      // Attribute at the end loop limits
//        int ent[5]={0,0,0,0,0}; // Enter flags
//	  int atent[5]={0,0,0,0,0};
        int lop[5]; // Loop variables

        std::ofstream cfile;
	if (appendfile) cfile.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary|std::ios::app);
	else cfile.open(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary);
        cfile << "/* C source file created by SevenuP v1.21                                */"<<std::endl;
        cfile << "/* SevenuP (C) Copyright 2002-2007 by Jaime Tejedor Gomez, aka Metalbrain*/"<<std::endl;
        cfile << std::endl;
        cfile << "/*"<<std::endl;
        cfile << "GRAPHIC DATA:"<<std::endl;
        cfile << "Pixel Size:      ("<<std::setw(3)<<OpenGraph->GetSizeX()<<", "<<std::setw(3)<<OpenGraph->GetSizeY()<<")"<<std::endl;
        cfile << "Char Size:       ("<<std::setw(3)<<OpenGraph->x_charsize<<", "<<std::setw(3)<<OpenGraph->y_charsize<<")"<<std::endl;
        if (OpenFrames>1) cfile << "Frames:           "<<std::setw(3)<<OpenFrames<<std::endl;
        int sortpoutputted=0;
        int atsortpoutputted=0;
        for (int sortpout=0;sortpout<5;sortpout++)
                {
                if (lim[pr[sortpout]]!=1)
                        {
                        if (sortpoutputted==0)
                                {
                                cfile << "Sort Priorities: ";
                                }
                        else
                                {
                                cfile << ", ";
                                }
                        cfile<<ws2s(strs2_file[pr[sortpout]]);
//                        ent[sortpout]=sortpoutputted;
                        sortpoutputted++;
                        }
                if (atlim[pr[sortpout]]!=1)
                        {
//                        atent[sortpout]=atsortpoutputted;
                        atsortpoutputted++;
                        }
                }
        cfile <<std::endl;
        cfile << "Data Outputted:  "<<ws2s(strs1_file[opts])<<std::endl;
        cfile << "Interleave:      "<<ws2s(strs3_file[interleave])<<std::endl;
        cfile << "Mask:            "<<ws2s(strs4_file[OpenPropierties-1+maskrev*(OpenPropierties-1)+2*AttrMask*(OpenPropierties-1)])<<std::endl;
        if (AttrMask==1)
		cfile << "Attribute Mask:  "<<std::setw(3)<<AttrMaskValue<<std::endl;
	if ((zigzag==1)&&(rp[1]>rp[0]))
		cfile << "Zigzag:          Horizontal"<<std::endl;
	if (z88dk==1)
		cfile << "First two bytes are y and x pixel size, for z88dk libraries"<<std::endl;

        cfile << "*/"<<std::endl;
        cfile << std::endl;


	// Generate binary output
	unsigned char GraphBuff[40000];
	unsigned char AttrBuff[5000];
        unsigned char SaveBuff[45000];
        unsigned char *parserGraph=GraphBuff;
        unsigned char *parserAttr=AttrBuff;
        unsigned char *parser=SaveBuff;
        unsigned char *parserfinal=SaveBuff;
        std::ofstream binfile;
        int saisGr=0;
        int saisAt=0;
	int sais=0;
        for (lop[4]=0;lop[4]<lim[pr[4]];lop[4]++)
                {
                for (lop[3]=0;lop[3]<lim[pr[3]];lop[3]++)
                        {
                        for (lop[2]=0;lop[2]<lim[pr[2]];lop[2]++)
                                {
                                for (lop[1]=0;lop[1]<lim[pr[1]];lop[1]++)
                                        {
                                        for (lop[0]=0;lop[0]<lim[pr[0]];lop[0]++)
                                                {
                                                GraphFrame=OpenGraph;
                                                for (int i=0;i<lop[rp[4]];i++)
                                                	{
                                                        GraphFrame=GraphFrame->NextGraph;
                                                        }
						if (OpenPropierties==2)
                                                	{
                                                        GraphSave[maskrev]=GraphFrame;
                                                        GraphSave[1-maskrev]=GraphFrame->Mask;
                                                        }
						else	{
                                                	GraphSave[0]=GraphFrame;
                                                        }
						int horiz=lop[rp[0]];
						if ((zigzag==1)&&(rp[1]>rp[0])&&((lop[rp[1]]%2)==1))
							{
							horiz=OpenGraph->x_charsize-horiz-1;
							}
						*parserGraph++=GraphSave[lop[rp[3]]]->GetByte(horiz+OpenGraph->x_charsize*lop[rp[2]],lop[rp[1]]);
                                                saisGr++;
                                                }
                                        }
                                }
                        }
                }

	for (lop[4]=0;lop[4]<atlim[pr[4]];lop[4]++)
        	{
                for (lop[3]=0;lop[3]<atlim[pr[3]];lop[3]++)
                	{
                        for (lop[2]=0;lop[2]<atlim[pr[2]];lop[2]++)
                        	{
                                for (lop[1]=0;lop[1]<atlim[pr[1]];lop[1]++)
                                	{
                                        for (lop[0]=0;lop[0]<atlim[pr[0]];lop[0]++)
                                        	{
                                                GraphFrame=OpenGraph;
                                                for (int i=0;i<lop[rp[4]];i++)
                                                	{
                                                        GraphFrame=GraphFrame->NextGraph;
                                                        }
						if (OpenPropierties==1)
	                                        	{
							*parserAttr++=GraphFrame->GetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]]);
                                                        saisAt++;
							}
						else 	{
							if (maskrev==lop[rp[3]])
								{
								*parserAttr++=GraphFrame->GetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]]);
                                                       		saisAt++;
								}
							else	{
								if (AttrMask==1)
									{
									*parserAttr++=AttrMaskValue;
									saisAt++;
									}
								}
							}
                                                }
					}
                                }
                        }
                }
                
	// Mixer
	parserGraph-=saisGr;
	parserAttr-=saisAt;

	switch(opts)
		{
		case 0:	// Graph+Attr
		case 1: // Attr+Graph
			{
			sais=saisGr+saisAt;
			// Calculate attribute interleaving point
			if (rp[interleave]<rp[1])
				{
				interleave=1;
				}
			int multGr=1;
			for (int i=0;i<=rp[interleave];i++)
				{
				multGr*=lim[pr[i]];
				}
			int multAt=multGr/8;
			int multMask=0;
			int steps=saisGr/multGr;
			if (saisGr/saisAt==16)
				{
				if (rp[3]<rp[interleave])
					{
					multAt/=2;
					}
				else	{
					multMask=1;
					for (int i=(rp[interleave]+1);i<rp[3];i++)
						{
						multMask*=lim[pr[i]];
						}
					}
				}
			int idx=0;
			for(int i=0;i<steps;i++)
				{
				if (opts==0)
					{
					for(int j=0;j<multGr;j++)
						{
						*parser++=*parserGraph++;
						}
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parser++=*parserAttr++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parser++=*parserAttr++;
								}
							}
						idx++;
						}
					}
				else	{
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parser++=*parserAttr++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parser++=*parserAttr++;
								}
							}
						idx++;
						}
					for(int j=0;j<multGr;j++)
						{
						*parser++=*parserGraph++;
						}
					}
				}
			parser=parserfinal;
			break;
			}
		case 2: // Graph
			{
			parser=parserGraph;
			sais=saisGr;
			break;
			}
		case 3: // Attr
			{
			parser=parserAttr;
			sais=saisAt;
			break;
			}
		}

        int alreadyinline=0;

        cfile << "unsigned char "<<ws2s(OpenName)<<"["<<sais+2*z88dk<<"] = {"<<std::endl;
        // Meter código z88dk
        if (z88dk==1)
        	{
        	cfile<<std::setw(3)<<OpenGraph->GetSizeY()<<", "<<std::setw(3)<<OpenGraph->GetSizeX()%256<<", "<<std::endl;
        	}

	int idx=0;
	while (sais>0)
		{
		if (alreadyinline==8)
                	{
                        cfile<<std::endl;
                        alreadyinline=0;
                        }
		cfile<<std::setw(3)<<(unsigned int)parser[idx++];
		if (--sais) cfile<<",";
		alreadyinline++;
		}
        cfile << "};"<<std::endl;
        cfile.close();
        }

void    OpenFile::ExportData(std::string filenam,int opts,int p0,int p1,int p2,int p3,int p4,int maskrev,int asmtype,int appendfile, int zigzag, int AttrMask, int AttrMaskValue, int interleave, int z88dk, int nolabel)
        {
        wxString ext;
        ext=s2ws(filenam).Right(3).Upper();
        if (ext==_("ASM")) SaveASM(filenam,opts,p0,p1,p2,p3,p4,maskrev,asmtype,appendfile,zigzag,AttrMask,AttrMaskValue,interleave,z88dk,nolabel);
        else if (ext==_("BIN")) SaveBIN(filenam,opts,p0,p1,p2,p3,p4,maskrev,appendfile,zigzag,AttrMask,AttrMaskValue,interleave,z88dk);
        else SaveC(filenam,opts,p0,p1,p2,p3,p4,maskrev,appendfile,zigzag,AttrMask,AttrMaskValue,interleave,z88dk);
        }

void 	OpenFile::ImportBIN(std::string filenam,int opts,int p0,int p1,int p2,int p3,int p4,int maskrev,int zigzag,int AttrMask,int AttrMaskValue,int interleave,int z88dk)
        {
        // std::ifstream::pos_type tamano;
        int tamano;
        int pr[6];      // Propierties
        int rp[6];      // Reverse priorities
        int expectedsize;
        SP_Graph *GraphSave[2];
        SP_Graph *GraphFrame[OpenFrames];
        
        std::ifstream binfile(s2ws(filenam).mb_str(wxConvLocal),std::ios::binary|std::ios::ate);
	tamano=(int)binfile.tellg();
	binfile.seekg (0, std::ios::beg);
	expectedsize=(((8*OpenPropierties)*(opts!=3)+(1+AttrMask*(OpenPropierties-1))*(opts!=2))*OpenGraph->y_charsize*OpenGraph->x_charsize*OpenFrames)+2*z88dk;

	if (tamano!=expectedsize)
		{
		wxString strg;
      		strg.Printf(_("Size: %d, expected: %d"),tamano,expectedsize);
        	(void)wxMessageBox(strg,_("Warning!"));
                return;
        	}

	GraphFrame[0]=OpenGraph;
        for (int i=1;i<OpenFrames;i++)
        	{
		GraphFrame[i]=GraphFrame[i-1]->NextGraph;
                }

        pr[0]=p0;               // Assign priorities
        pr[1]=p1;
        pr[2]=p2;
        pr[3]=p3;
        pr[4]=p4;
        pr[5]=5;
	rp[5]=5;
        for (int i=0;i<5;i++)   // Get reverse priorities
                {
                for (int k=0;k<5;k++)
                        {
                        if (pr[i]==k) rp[k]=i;
                        }
                }

        int lim[6]={OpenGraph->x_charsize,8,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1}; // Graphic limits
        int atlim[6]={OpenGraph->x_charsize,1,OpenGraph->y_charsize,OpenPropierties,OpenFrames,1};    // Attribute limits

        int lop[5]; // Loop variables

        char LoadBuff[45000];
	char GraphBuff[40000];
	char AttrBuff[5000];
        char SizeBuff[2];
        char *parserGraph=GraphBuff;
        char *parserAttr=AttrBuff;
        char *parserSize=SizeBuff;
        char *parser=LoadBuff;
        int saisGr=0;
        int saisAt=0;
	int sais=0;
	
	if (z88dk==1)
		{
		binfile.read(SizeBuff,2);
		}

	// DeMixer

	saisGr=OpenGraph->x_charsize*8*OpenGraph->y_charsize*OpenPropierties*OpenFrames;
	saisAt=OpenGraph->x_charsize*OpenGraph->y_charsize*OpenFrames;
	if ((OpenPropierties==2)&&(AttrMask==1))
		{
		saisAt*=2;
		}

	switch(opts)
		{
		case 0:	// Graph+Attr
		case 1: // Attr+Graph
			{
        		binfile.read(LoadBuff,45000);
			sais=saisGr+saisAt;
			// Calculate attribute interleaving point
			if (rp[interleave]<rp[1])
				{
				interleave=1;
				}
			int multGr=1;
			for (int i=0;i<=rp[interleave];i++)
				{
				multGr*=lim[pr[i]];
				}
			int multAt=multGr/8;
			int multMask=0;
			int steps=saisGr/multGr;
			if (saisGr/saisAt==16)
				{
				if (rp[3]<rp[interleave])
					{
					multAt/=2;
					}
				else	{
					multMask=1;
					for (int i=(rp[interleave]+1);i<rp[3];i++)
						{
						multMask*=lim[pr[i]];
						}
					}
				}
			int idx=0;
			for(int i=0;i<steps;i++)
				{
				if (opts==0)
					{
					for(int j=0;j<multGr;j++)
						{
						*parserGraph++=*parser++;
						}
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parserAttr++=*parser++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parserAttr++=*parser++;
								}
							}
						idx++;
						}
					}
				else	{
					if (multMask==0)
						{
						for(int j=0;j<multAt;j++)
							{
							*parserAttr++=*parser++;
							}
						}
					else	{
						if((idx/multMask)%2==maskrev)
							{
							for(int j=0;j<multAt;j++)
								{
								*parserAttr++=*parser++;
								}
							}
						idx++;
						}
					for(int j=0;j<multGr;j++)
						{
						*parserGraph++=*parser++;
						}
					}
				}
			parserGraph-=saisGr;
			parserAttr-=saisAt;
			break;
			}
		case 2: // Graph
			{
			// BUGGG
        		binfile.read(GraphBuff,40000);
			break;
			}
		case 3: // Attr
			{
        		binfile.read(AttrBuff,5000);
			break;
			}
		}
        binfile.close();

        for (lop[4]=0;lop[4]<lim[pr[4]];lop[4]++)
                {
                for (lop[3]=0;lop[3]<lim[pr[3]];lop[3]++)
                        {
                        for (lop[2]=0;lop[2]<lim[pr[2]];lop[2]++)
                                {
                                for (lop[1]=0;lop[1]<lim[pr[1]];lop[1]++)
                                        {
                                        for (lop[0]=0;lop[0]<lim[pr[0]];lop[0]++)
                                                {
						int horiz=lop[rp[0]];
						if ((zigzag==1)&&(rp[1]>rp[0])&&((lop[rp[1]]%2)==1))
							{
							horiz=OpenGraph->x_charsize-horiz-1;
							}
						if (OpenPropierties==2)
                                                	{
                                                        GraphSave[maskrev]=GraphFrame[lop[rp[4]]];
                                                        GraphSave[1-maskrev]=GraphFrame[lop[rp[4]]]->Mask;
                                                        }
						else	{
                                                	GraphSave[0]=GraphFrame[lop[rp[4]]];
                                                        }
						GraphSave[lop[rp[3]]]->SetByte(8*(horiz+OpenGraph->x_charsize*lop[rp[2]])+lop[rp[1]],*parserGraph++);
        					}
                                        }
                                }
                        }
                }

	for (lop[4]=0;lop[4]<atlim[pr[4]];lop[4]++)
        	{
                for (lop[3]=0;lop[3]<atlim[pr[3]];lop[3]++)
                	{
                        for (lop[2]=0;lop[2]<atlim[pr[2]];lop[2]++)
                        	{
                                for (lop[1]=0;lop[1]<atlim[pr[1]];lop[1]++)
                                	{
                                        for (lop[0]=0;lop[0]<atlim[pr[0]];lop[0]++)
                                        	{
						if (OpenPropierties==1)
	                                        	{
	                                        	if(opts==2)
	                                        		{
								GraphFrame[lop[rp[4]]]->SetAttr(56,*parserAttr++);
	                                        		}
							else	{
								GraphFrame[lop[rp[4]]]->SetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]],*parserAttr++);
								}						
							}
						else 	{
							if (maskrev==lop[rp[3]])
								{
	                                        		if(opts==2)
		                                        		{
									GraphFrame[lop[rp[4]]]->SetAttr(56,*parserAttr++);
	                                        			}
								else	{								
									GraphFrame[lop[rp[4]]]->SetAttr(lop[rp[0]]+OpenGraph->x_charsize*lop[rp[2]],*parserAttr++);
									}
								}
							else	{
								if (AttrMask==1)
									{
									AttrMaskValue=*parserAttr++;
									}
								}
							}
                                                }
					}
                                }
                        }
                }
        }
