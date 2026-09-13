	                **********************************
	                *SevenuP - v1.21WIP - Source code*
	                **********************************



Introduction
************

 Hi! This is the source code for SevenuP, my spectrum graphic editor. If you
prefer a binary distribution, go to http://www.speccy.org/metalbrain to get
it (win32, Linux and MacOS X versions available).



Quick compile instructions
**************************

 First, go to http://www.wxwidgets.org, download and install an appropiate
version of the library. This version has been compiled with version 2.4.2,
earlier 2.4.x versions have some bugs, and even earlier versions (such as
2.2.9) won't work. Newest versions (2.6.x) also work.

 For Windows, IFF support must be enabled manually modifying the file
setup.h (located at lib\msw\wx on the wxWidgets 2.4.2 directory, or 
lib\gcc_lib\msw\wx on version 2.6.x) before you start compiling the
library. You may use the file "setup.h" included in directory wx, to
get a smaller library size. A different file is needed for versions
2.4.2 or 2.6.x.

 For MacOS X, you should apply the patch for this bug if you're using
version 2.6.3 or lower:
http://sourceforge.net/tracker/index.php?func=detail&aid=1473745&group_id=9863&atid=109863
If you have a higher version of wxWidgets, this won't be necessary.

 Then try to compile the samples & demos provided with wxWidgets.

 If you succeed, you're ready to compile SevenuP.

 Follow the appropiate instructions below:



Windows notes
*************

 I use the mingw32 C++ compiler to produce the Windows executable of
SevenuP. If you prefer to use another compiler, such as Visual C++ or
Borland C++, you must make your own makefiles. In that case, don't
hesitate to contact me either to provide me the resulting makefile (to
be included in next releases) or to fix the problems that may arise.

 If you use mingw32 (or cygwin, AFAIK they're quite alike), check your
gcc version typing gcc -v.

 If it's a 2.95 series, be aware that gcc 2.95.3-5 is buggy, producing
faulty executables, so you must upgrade to 2.95.3-6. This is the compiler
I use for the windows executable, so there shouldn't be any problems.

 I've also tested compilation with gcc 3.2.3, and 3.4.3 and it does work
fine (but produces a much bigger executable).

 Anyway, to compile just type:

make -f makefile.g95 FINAL=1

 if you are using version 2.4.2 of wxWidgets, or

make -f makefile.gcc

 if you're using version 2.6.x of wxWidgets.

 You'll get a very big file (over 2Mb, nearly 3Mb and a half if compiled
with gcc 3.x.y series, and also bigger (over 5Mb) if you use wxWidgets
2.6.x because it depends on library and compiler versions and configuration
options), but don't worry, once you type:

strip SevenuP.exe

 the size will be cutted to around 1'5Mb (2Mb for GCC 3.x.y, over 3Mb for
wx2.6.x). It's still a big file, but you can get Upack from
http://wex.cn/dwing/ to compress it.



Unix notes
**********

 Both 2.4.2 and 2.6.x versions are fine. If you are already using one of
them (e.g. 2.6.x for aMule), keep using that one to avoid problems.

 Once you have installed wxWidgets, you must be able to run a utility
called wx-config.

 Now, just use the makefile with make:

make -f makefile.unx

 and you're done. After the compilation, you may strip the file
 
strip SevenuP

 and use UPX from http://upx.sourceforge.net to compress the binary file.


FreeBSD notes
*************

 Emilio Florido has created a script to install SevenuP, just type:

compileSevenuPFreeBSD

(giving execution permission if the file doesn't have it) and compilation
will start. If you lack the library, it will tell you how to install it.


Macintosh notes (by James Weatherley)
***************

· Put the SevenuP source folder in the same folder as the wxWidgets source
	folder.
· Build wxWidgets as described in the wxWidgets readme.
· Copy 'libwx_mac-2.x.x.dylib'  to '/usr/local/lib' (replace x.x with the library version)
· Open 'SevenuP.pbproj'
· Build!

ie:

/sources
  |
  \SevenuP-vx.xx-src Folder
  | \SRC
  |  `*.cpp, *.h, etc.
  \wxWidgets-2.x.x
   \macbuild
     \includes etc.
     
Macintosh Notes (by Peter Hull)
***************

As an alternative, you can build SevenuP from the command line, using
the supplied makefile.osx.

Download and build wxWidgets (I used 2.6.3), and install it. Then
change to the SevenuP source directory and type make -f
makefile.osx. This should compile and build the Application bundle.

By default, this will use the dynamic wxWidgets libraries that you
installed. If you want to make a self-contained application for
distribution, make sure that you configure wxWidgets with
--disable-shared. This is explained in the file docs/mac/install.txt
that comes with wxWidgets.