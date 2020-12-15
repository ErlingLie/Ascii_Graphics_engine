# Directories, include, lib,
!if defined(DEBUG_BUILD)
LIB_TYPE = Debug
EXECUTABLE = debug
BUILDDIR = debug
!else
LIB_TYPE = Release
EXECUTABLE = release
BUILDDIR = release
!endif


WIN_LIBRARIES = wsock32.lib comctl32.lib kernel32.lib user32.lib gdi32.lib \
				winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib \
				oleaut32.lib uuid.lib odbc32.lib odbccp32.lib

# Compiler and linker config
CXX = clang-cl
CXXFLAGS = /std:c++17 -TP -m64 -fdiagnostics-absolute-paths /W4 /GR \
			/DEBUG /INCREMENTAL /nologo /EHsc

DEBUG = /Od /Ob0 -DWIN32 /MDd /Zi /RTC1
RELEASE = /O2 /Ob2 -DNDEBUG -DWIN32_LEAN_AND_MEAN /MD
MACROS = -D_WINDOWS -DNOGDI -D_CRT_SECURE_NO_WARNINGS
DEBUGLD = /DEBUG /INCREMENTAL /Zi
RELEASELD = /INCREMENTAL:NO
LDFLAGS = -m64 $(WIN_LIBRARIES)
LINK = $(CXX)

!if defined(DEBUG_BUILD)
CXXFLAGS = $(CXXFLAGS) $(DEBUG)
LDFLAGS = $(LDFLAGS) $(DEBUGLD)
!else
CXXFLAGS = $(CXXFLAGS) $(RELEASE)
LDFLAGS = $(LDFLAGS) $(RELEASELD)
!endif

!ifndef EXECUTABLE
EXECUTABLE = program
!endif

SOURCES = *.cpp
OBJECTS = $(BUILDDIR)/$(SOURCES:.cpp=.obj)

# Inference rules, etc.
.SUFFIXES: .obj .cpp

{.}.cpp{$(BUILDDIR)}.obj:
	if not exist "$(BUILDDIR)" mkdir $(BUILDDIR)
	$(CXX) $(CXXFLAGS) /c $< /Fo$(@D)/

$(BUILDDIR)/$(EXECUTABLE).exe: $(OBJECTS)
	$(LINK) $(LDFLAGS) $(OBJECTS) /Fe$@
	@echo ...
	@echo Successfully built ./$@

$(EXECUTABLE).exe: $(BUILDDIR)/$(EXECUTABLE).exe

# Catch-all, but can interfere with student's own files in build directory
#rmdir /s /q $(BUILDDIR)
clean:
	@echo Cleaning the output files in $(BUILDDIR)
	-del $(BUILDDIR)\*.obj
	-del $(BUILDDIR)\$(EXECUTABLE).exe
	-del $(BUILDDIR)\$(EXECUTABLE).pdb
	-del $(BUILDDIR)\$(EXECUTABLE).ilk