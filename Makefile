## Automation of building executable using Nmake with the MSVC compiler [v1.0] by Oscar Milstein.

## Path variables
INCLUDES = Includes\\
BUILD = Build\\
BIN = Bin\\
DEBUG = Debug\\
EXECUTABLE = $(BIN)Archaeus.exe
SOURCE = Source\\

## Compile command variables
CFLAGS =-c -permissive- -std:c17 -TC -W4 -nologo -IIncludes -D"COBJMACROS" -D"WIN32_LEAN_AND_MEAN" -Fd$(DEBUG)
LFLAGS =-nologo -SUBSYSTEM:WINDOWS $(LIBS)
LIBS =User32.lib d3d11.lib dxgi.lib d3dcompiler.lib dxguid.lib Winmm.lib

all:
!IF DEFINED(debug)
   $(CC) $(CFLAGS) -MP -Zi -MTd -Od -FC -D"DEBUG" $(SOURCE)*.c -Fo:$(BUILD)
   LINK $(LFLAGS) -DEBUG:FASTLINK $(BUILD)*.obj -OUT:$(EXECUTABLE)
!ELSE
   $(CC) $(CFLAGS) -MP -O2 -D"NDEBUG" $(SOURCE)*.c -Fo:$(BUILD)
   LINK $(LFLAGS) $(BUILD)*.obj -OUT:$(EXECUTABLE)
!ENDIF
   @echo Build complete!

clean:
   -del $(BIN)*.ilk $(BIN)*.pdb $(BIN)*.exe $(BUILD)*.obj $(DEBUG)*.pdb
   @echo Clean complete!