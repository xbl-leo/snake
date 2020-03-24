# Microsoft Developer Studio Generated NMAKE File, Based on Tetris.dsp
!IF "$(CFG)" == ""
CFG=Tetris - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Tetris - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Tetris - Win32 Release" && "$(CFG)" != "Tetris - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tetris.mak" CFG="Tetris - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tetris - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Tetris - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tetris - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Tetris.exe"


CLEAN :
	-@erase "$(INTDIR)\Tetris.obj"
	-@erase "$(INTDIR)\Tetris.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Tetris.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Tetris.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Tetris.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tetris.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Tetris.pdb" /machine:I386 /out:"$(OUTDIR)\Tetris.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Tetris.obj" \
	"$(INTDIR)\Tetris.res"

"$(OUTDIR)\Tetris.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tetris - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Tetris.exe"


CLEAN :
	-@erase "$(INTDIR)\Tetris.obj"
	-@erase "$(INTDIR)\Tetris.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Tetris.exe"
	-@erase "$(OUTDIR)\Tetris.ilk"
	-@erase "$(OUTDIR)\Tetris.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Tetris.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\Tetris.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Tetris.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Tetris.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Tetris.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Tetris.obj" \
	"$(INTDIR)\Tetris.res"

"$(OUTDIR)\Tetris.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Tetris.dep")
!INCLUDE "Tetris.dep"
!ELSE 
!MESSAGE Warning: cannot find "Tetris.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Tetris - Win32 Release" || "$(CFG)" == "Tetris - Win32 Debug"
SOURCE=.\Tetris.cpp

"$(INTDIR)\Tetris.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\Resource\Tetris.rc

!IF  "$(CFG)" == "Tetris - Win32 Release"


"$(INTDIR)\Tetris.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x804 /fo"$(INTDIR)\Tetris.res" /i "Resource" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "Tetris - Win32 Debug"


"$(INTDIR)\Tetris.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x804 /fo"$(INTDIR)\Tetris.res" /i "Resource" /d "_DEBUG" $(SOURCE)


!ENDIF 


!ENDIF 

